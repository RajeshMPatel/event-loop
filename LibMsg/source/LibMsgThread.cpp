//
//  LibMsgThread.h
//  LibMsg
//
//  Created by Rajesh Patel on 8/9/18.
//

#include "stdafx.h"
#include "LibMsgThread.h"
#include <iostream>

using namespace std;

BEGIN_LIBMSG_MAP(LibMsgThread, LibMsgHandler)
END_LIBMSG_MAP()

LibMsgThread::LibMsgThread()
  : m_work(m_io_service),
    m_strand(m_io_service),
    m_bIsRunning(false) {
}

LibMsgThread::~LibMsgThread() {
}

void LibMsgThread::LibMsgHandler(const int nMsgId, LM_WPARAM wParam, LM_LPARAM lParam) {
  if (!m_bIsRunning)
  {
    return;
  }
  OnMessage(nMsgId, (LM_WPARAM)wParam, (LM_LPARAM)lParam);
}

int LibMsgThread::Start() {
  cout << "==>LibMsgThread::Start" << endl;
  if (m_bIsRunning)
    return -1;

  int tNumber = 1;
  for(int i = 0; i < tNumber; ++i)
  {
    boost::thread* pThread = m_threads.create_thread(boost::bind(&boost::asio::io_service::run, &m_io_service));
    
    cout << "Thread id = " << pThread->get_id() << endl;
    m_lstThreadIds.push_back(pThread->get_id());
    m_lstThreadPtrs.push_back(pThread);
  }

  m_bIsRunning = true;
  return 0;
}

int LibMsgThread::Stop() {
  if (!m_bIsRunning)
    return -1;

  m_bIsRunning = 0;
  m_io_service.stop();
  m_threads.join_all();
  m_lstThreadPtrs.clear();

  return 0;
}

int LibMsgThread::PostMessage(const int nMessage, LM_WPARAM wParam, LM_LPARAM lParam) {
  m_io_service.post(
    boost::bind(&LibMsgThread::LibMsgHandler, this, nMessage, wParam, lParam));
  return 0;
}

int LibMsgThread::SendMessage1(const int nMessage, LM_WPARAM wParam, LM_LPARAM lParam, unsigned int msTimeout, int line) {
  if (IsPartOfMsgThreadPool())
  {
    if (m_lstThreadIds.size() == 1)
    {
      LibMsgHandler(nMessage, wParam, lParam);
    }
    else
    {
      m_io_service.dispatch(
        m_strand.wrap(boost::bind(&LibMsgThread::LibMsgHandler, this, nMessage, wParam, lParam)));
    }
    return 0;
  }

  return SendMessageInternal(nMessage, wParam, lParam, msTimeout, 0);
}

bool LibMsgThread::IsPartOfMsgThreadPool() {
  boost::thread::id curid = boost::this_thread::get_id();

  list<boost::thread::id>::iterator it;
  for (auto it=m_lstThreadIds.begin(); it!=m_lstThreadIds.end(); ++it)
  {
    boost::thread::id id = *it;
    if (curid == id)
      return true;
  }
  return false;
}

int LibMsgThread::SendMessageInternal(const int nMessage, LM_WPARAM wParam, LM_LPARAM lParam, unsigned int msTimeout, int cookie) {
#if 0
#pragma error "This still needs to be verified"
  m_io_service.dispatch(boost::bind(&LibMsgThread::LibMsgHandler, this, nMessage, wParam, lParam));
#endif
  return 0;
}
