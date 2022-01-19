#ifndef libmsg_event_h_
#define libmsg_event_h_

#include "LibMsgDllExport.h"
#include "LibMsgThread.h"
#include <boost/enable_shared_from_this.hpp>

#ifdef _WIN32
#include <boost/asio/windows/object_handle.hpp>
#endif

class LibMsgEvent
{
protected:
  LibMsgThread*   m_pMsgThread;
  int             m_nMsgId;
#ifdef _WIN32
  boost::asio::windows::object_handle *m_pObj;
#endif

public:
  LibMsgDllExport LibMsgEvent(LibMsgThread* pMsgThread, const int nMsgId);
  LibMsgDllExport virtual ~LibMsgEvent();

protected:
  virtual void HandleEvent(const boost::system::error_code& error) = 0;
};

#endif // libmsg_event_h_
