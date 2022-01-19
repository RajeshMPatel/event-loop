
#ifndef libmsg_networkchange_h_
#define libmsg_networkchange_h_

#include "LibMsgDllExport.h"
#include "LibMsgThread.h"
#include "LibMsgEvent.h"

typedef ptrdiff_t LM_SOCKET;

class LibMsgNetworkChange 
 : public LibMsgEvent,
   public boost::enable_shared_from_this<LibMsgNetworkChange>
{
public:
  LibMsgDllExport LibMsgNetworkChange();
  LibMsgDllExport LibMsgNetworkChange(LibMsgThread* pMsgThread, const int nMsgId);
  LibMsgDllExport virtual ~LibMsgNetworkChange();

  LibMsgDllExport bool RegisterNotification();
  LibMsgDllExport void UnRegisterNotification();
  LibMsgDllExport virtual void HandleEvent(const boost::system::error_code& error);

protected:
  bool EnableNotification();

private:

  LM_SOCKET m_Socket;
  bool m_bEnabled;

private:
  void ResetEvents();
};

#endif // libmsg_networkchange_h_
