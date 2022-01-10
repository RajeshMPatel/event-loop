//
//  LibMsgThread.h
//  LibMsg
//
//  Created by Rajesh Patel on 8/9/18.
//

#ifndef Libmsgthread_h
#define Libmsgthread_h

using namespace std;

#include "LibMsgDllExport.h"
#include <list>
#include "LibMsgHandler.h"
#include "boost/asio.hpp"
#include <boost/thread/thread.hpp>

class LibMsgSynchronizer;
typedef shared_ptr<LibMsgSynchronizer> LibMsgSynchronizerPtr;
const unsigned k_INFINITE = 0xFFFFFFFF;

class LibMsgThread : public LibMsgHandler
{
public:

  LibMsgDllExport  LibMsgThread();
  LibMsgDllExport  virtual ~LibMsgThread();

  LibMsgDllExport  int Start();
  LibMsgDllExport  int Stop();
  LibMsgDllExport  virtual void PreShutdown() { };

  LibMsgDllExport  int PostMessage(const int nMsgId, LM_WPARAM wParam, LM_LPARAM lParam);
  LibMsgDllExport  int SendMessage1(const int nMsgId, LM_WPARAM wParam, LM_LPARAM lParam, unsigned int msTimeout=k_INFINITE, int line=__LINE__);
  #define SendMsg(a,b,c) SendMessage1(a,b,c,k_INFINITE,__LINE__)
  template<typename T0, typename T1>
  int PostMessageT(const int nMsgId, T0 t0, T1 t1);

  LibMsgDllExport  boost::asio::io_service&    GetASIOService() { return m_io_service; }

  DECLARE_LIBMSG_MAP_EXPORT()

protected:
  bool                            m_bIsRunning;
  std::list<boost::thread::id>    m_lstThreadIds;
  std::list<boost::thread*>       m_lstThreadPtrs;

  boost::asio::io_service         m_io_service;
  boost::thread_group             m_threads;
  boost::asio::io_service::work   m_work;
  boost::asio::io_service::strand m_strand;

private:
  void LibMsgHandler(const int nMsgId, LM_WPARAM wParam, LM_LPARAM lParam);

  template<typename T0, typename T1>
  void LibMsgHandlerT(const int nMsgId, T0 t0, T1 t1);

  // The following two are used to synchronize SendMessage. We don't want the thread to continue
  // until the handler finishes. So we use condition object to synchronize (if it is not part of the thread pool)
  void LibMsgHandlerEx(const int nMsgId, LM_WPARAM wParam, LM_LPARAM lParam, LibMsgSynchronizerPtr pCond, int cookie=0);
  bool IsPartOfMsgThreadPool();

  // platform specific
  int SendMessageInternal(const int nMessage, LM_WPARAM wParam, LM_LPARAM lParam, unsigned int msTimeout=k_INFINITE, int cookie=0);
};

template<typename T0, typename T1>
int LibMsgThread::PostMessageT(const int nMsgId, T0 t0, T1 t1) {
  m_io_service.post(bind(&LibMsgThread::LibMsgHandlerT<T0, T1>, this, nMsgId, t0, t1));
  return 0;
}

template<typename T0, typename T1>
void LibMsgThread::LibMsgHandlerT(const int nMsgId, T0 t0, T1 t1) {
  if (!m_bIsRunning)
    return;
  OnMessageT(nMsgId, t0, t1);
}

#endif /* Libmsgthread_h */
