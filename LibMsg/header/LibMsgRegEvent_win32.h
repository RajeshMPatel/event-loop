#ifndef libmsg_reg_event_h_
#define libmsg_reg_event_h_

#ifdef WIN32
#include "LibMsgDllExport.h"
#include "LibMsgThread.h"
#include "LibMsgEvent.h"

class LibMsgRegEvent : 
  public LibMsgEvent,
  public boost::enable_shared_from_this<LibMsgRegEvent>
{
public:
  LibMsgDllExport  LibMsgRegEvent(LibMsgThread* pMsgThread, const int nMsgId);
  LibMsgDllExport  virtual ~LibMsgRegEvent();
  LibMsgDllExport  void Clear();

LibMsgDllExport  int RegisterNotificationW(
  HKEY         hRoot,
  const WCHAR* pszPath, 
  const WCHAR* pszName, 
  const BOOL   bNotifySubkey,
  const DWORD  nNotifyFilter = (REG_NOTIFY_CHANGE_NAME|REG_NOTIFY_CHANGE_LAST_SET), 
  const BOOL   bIsAsync = TRUE);

LibMsgDllExport  int RegisterNotificationA(
  HKEY        hRoot,
  const CHAR* pszPath, 
  const CHAR* pszName, 
  const BOOL  bNotifySubkey,
  const DWORD nNotifyFilter = (REG_NOTIFY_CHANGE_NAME|REG_NOTIFY_CHANGE_LAST_SET), 
  const BOOL  bIsAsync = TRUE);

protected:
  virtual void HandleEvent(const boost::system::error_code& error);
  virtual void OnRegistryChange() {}

private:
    bool EnableNotification();

protected:
    HKEY         m_hRoot;
    HKEY         m_hKey;
    std::wstring m_strPath;
    std::wstring m_strKeyName;
    BOOL         m_bNotifySubkey;
    DWORD        m_nNotifyFilter;
    BOOL         m_bIsAsync;
};

#endif // WIN32
#endif // libmsg_reg_event_h_

