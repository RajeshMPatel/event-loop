#include "stdafx.h"
#include "LibMsgNetworkChange.h"

LibMsgNetworkChange::LibMsgNetworkChange(AfMsgThread* pMsgThread, const int nMsgId)
  :
  LibMsgEvent(pMsgThread, nMsgId),
  m_bEnabled(false) {
}

LibMsgNetworkChange::~LibMsgNetworkChange()
  if (m_pObj) {
    delete m_pObj;
    m_pObj = nullptr;
  }
}

void LibMsgNetworkChange::UnRegisterNotification() {
  if (m_bEnabled) {
    if (m_pObj) {
      boost::asio::windows::object_handle *pObj = m_pObj;
      m_pObj = nullptr;
      delete pObj;
    }

    closesocket((SOCKET)m_Socket);
    m_Socket = (LM_SOCKET) INVALID_SOCKET;
    WSACleanup();
  }
  m_bEnabled = false;
}

bool LibMsgNetworkChange::RegisterNotification() {
  if (m_bEnabled)
    return true;

  WSADATA wsaData;
  int iResult;
  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0)
    return false;

  m_Socket = (LM_SOCKET) socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);
  if (m_Socket == (LM_SOCKET) INVALID_SOCKET) {
    WSACleanup();
    return false;
  }

  WSAEVENT hEvent = WSACreateEvent();
  m_pObj = new boost::asio::windows::object_handle(m_pMsgThread->GetASIOService(), hEvent);
  if (!EnableNotification()) {
    WSACleanup();
    return false;
  }

  m_bEnabled = true;
  return true;
}

void LibMsgNetworkChange::HandleEvent(const boost::system::error_code& error) {
  if (!error) {
    if (!m_bEnabled)
      return;

    WSANETWORKEVENTS info;
    if (WSAEnumNetworkEvents((SOCKET) m_Socket, m_pObj->native_handle(), &info) != 0)
      return;

    if ( ((info.lNetworkEvents & FD_ADDRESS_LIST_CHANGE) != 0) ||
        ((info.lNetworkEvents & FD_ROUTING_INTERFACE_CHANGE) != 0) )
    {
      // Routing table changed. Do something
      if (m_pMsgThread && (m_nMsgId != 0))
        m_pMsgThread->PostMessage(m_nMsgId, 0, 0);
    }

    if (!EnableNotification())
      return;
  }
  else
    std::cout << "Error: "<< error.message().c_str() << endl;

}

bool LibMsgNetworkChange::EnableNotification() {
  if (!m_pObj)
    return false;

  WSAEVENT hEvent = m_pObj->native_handle();
  if (hEvent == WSA_INVALID_EVENT) {
    closesocket((SOCKET) m_Socket);
    m_Socket = (LM_SOCKET) INVALID_SOCKET;
    return false;
  }

  if (WSAEventSelect((SOCKET) m_Socket, hEvent,
                      FD_ADDRESS_LIST_CHANGE | FD_ROUTING_INTERFACE_CHANGE) == SOCKET_ERROR)
  {
    closesocket((SOCKET) m_Socket);
    m_Socket = (LM_SOCKET) INVALID_SOCKET;
    return false;
  }

  ResetEvents();
  m_pObj->async_wait(boost::bind(&LibMsgNetworkChange::HandleEvent, shared_from_this(), boost::asio::placeholders::error));
  return true;
}


void LibMsgNetworkChange::ResetEvents()
{
  if ( !m_pObj )
    return;

  ResetEvent(m_pObj->native_handle());
  DWORD dwRetCount;
  WSAIoctl((SOCKET) m_Socket, SIO_ADDRESS_LIST_CHANGE, nullptr, 0, nullptr, 
            0, &dwRetCount, nullptr, nullptr);

  // enable notification of address list
  sockaddr_in addr;
  addr.sin_family      = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;

  WSAIoctl((SOCKET) m_Socket, SIO_ROUTING_INTERFACE_CHANGE, &addr, sizeof(addr), nullptr, 
            0, &dwRetCount, nullptr, nullptr);
}

