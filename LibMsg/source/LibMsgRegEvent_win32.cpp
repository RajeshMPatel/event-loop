#ifdef WIN32

#include "stdafx.h"
#include "LibMsgRegEvent_win32.h"
#include "LibMsgStringUtil_win32.h"

LibMsgRegEvent::LibMsgRegEvent(LibMsgThread* pMsgThread, const int nMsgId)
  : LibMsgEvent(pMsgThread, nMsgId),
    m_hKey(NULL)
{
  Clear();
}

LibMsgRegEvent::~LibMsgRegEvent() {
  if (INVALID_HANDLE_VALUE != (HANDLE)m_hKey)
    RegCloseKey(m_hKey);
  delete m_pObj;
}

void LibMsgRegEvent::Clear() {
  if (m_hKey && (INVALID_HANDLE_VALUE != m_hKey))
    RegCloseKey(m_hKey);

  m_hRoot         = HKEY_LOCAL_MACHINE;
  m_hKey          = NULL;
  m_bNotifySubkey = FALSE;
  m_nNotifyFilter = REG_NOTIFY_CHANGE_NAME|REG_NOTIFY_CHANGE_LAST_SET;
  m_bIsAsync      = TRUE;

  m_strPath.clear();
  m_strKeyName.clear();
}

bool LibMsgRegEvent::EnableNotification() {
  HANDLE hEvent = m_pObj->native_handle();
  if (!hEvent || INVALID_HANDLE_VALUE == hEvent)
    return false;

  const DWORD nErr = RegNotifyChangeKeyValue(m_hKey, m_bNotifySubkey, m_nNotifyFilter, hEvent, m_bIsAsync);
  if (nErr != ERROR_SUCCESS)
    return false;

  m_pObj->async_wait(boost::bind(&LibMsgRegEvent::HandleEvent, shared_from_this(), boost::asio::placeholders::error));
  return true;
}

int LibMsgRegEvent::RegisterNotificationA(
  HKEY        hRoot,
  const CHAR* pszPath,
  const CHAR* pszName,
  const BOOL  bNotifySubkey,
  const DWORD nNotifyFilter,
  const BOOL  bIsAsync) {

  // Always convert to Unicode and then use it for W version of the API
  std::wstring strPathW, strKeyName;

  if (!Utf8ToUnicode(pszPath, strPath))
    return -1;
  if (!Utf8ToUnicode(pszName, strKeyName))
    return -1;

  // std string is utf-8. To call UNICODE function, convert to UNICODE first
  return RegisterNotificationW(hRoot, strPath.c_str(), strKeyName.c_str(), bNotifySubkey, nNotifyFilter, bIsAsync);
}

int LibMsgRegEvent::RegisterNotificationW (
  HKEY         hRoot,
  const WCHAR* pszPath,
  const WCHAR* pszName,
  const BOOL   bNotifySubkey,
  const DWORD  nNotifyFilter,
  const BOOL   bIsAsync) {

  Clear();

  if (pszPath)
    m_strPath = pszPath;

  if (pszName)
    m_strKeyName = pszName;

  m_bNotifySubkey = bNotifySubkey;
  m_nNotifyFilter = nNotifyFilter;
  m_bIsAsync = bIsAsync;

  HANDLE hEvent = m_pObj ? m_pObj->native_handle() : NULL;
  if (hEvent && (INVALID_HANDLE_VALUE != hEvent))
    ResetEvent(hEvent);
  else
  {
    hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (!hEvent)
      return -1;

    if (!m_pObj)
      m_pObj = new boost::asio::windows::object_handle(m_pMsgThread->GetASIOService(), hEvent);
  }

  int nErr = RegOpenKeyEx(hRoot, pszPath, 0, KEY_NOTIFY, &m_hKey);
  if (nErr != ERROR_SUCCESS)
    return -1;

  nErr = EnableNotification();
  return nErr;
}

void LibMsgRegEvent::HandleEvent(const boost::system::error_code& error) {
  if (error) {
    // throw an expection. Not implemented right now
    return;
  }

  if (m_pObj && m_pObj->native_handle() && (m_pObj->native_handle() != INVALID_HANDLE_VALUE)) {
    HANDLE hEvent = m_pObj->native_handle();
    ResetEvent(hEvent);
    EnableNotification();
    m_pMsgThread->PostMessage(m_nMsgId, 0, 0);
  }
}

#endif // WIN32