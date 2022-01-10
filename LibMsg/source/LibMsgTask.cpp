//
//  LibMsgTask.cpp
//  LibMsg
//
//  Created by Rajesh Patel on 8/9/18.
//

#include "stdafx.h"
#include "LibMsgTask.h"
#include <boost/date_time/posix_time/posix_time.hpp>

LibMsgTask::LibMsgTask
(
  LibMsgThread* pLibMsgThread,
  int           nMsgId,
  void*         wParam,
  void*         lParam)
  :
  m_pLibMsgThread(pLibMsgThread),
  m_rTimer(pLibMsgThread->GetASIOService()),
  m_tDiff(0),
  m_nMsgId(nMsgId),
  m_wparam(wParam),
  m_lparam(lParam),
  m_bCanceled(false)
{
}

LibMsgTask::~LibMsgTask() {
  Suspend();
}

void LibMsgTask::Execute(const boost::system::error_code& ec) {
  if (ec.value() == 0 && !m_bCanceled) {
    if (m_pLibMsgThread) {
      m_pLibMsgThread->PostMessage(
        m_nMsgId,
        (LM_WPARAM) m_wparam,
        (LM_LPARAM) m_lparam);
    }

    if ( !m_bRunOnce )
      SetIntervalFromNow(m_tDiff);
  }
}

void LibMsgTask::Suspend() {
  // Just make it fire after infinite time. Cancel does not always work in XP
  // and deleting causes crash. So just keep it around and suspend it
  m_rTimer.expires_at(boost::posix_time::pos_infin);
}

void LibMsgTask::Cancel() {
  m_bCanceled = true;
  // Added suspend here. If you just cancel and reset the pointer, smart pointer stays around and it keeps firing
  Suspend();
}

void LibMsgTask::SetIntervalFromNow(const long tDiff, bool bRunOnce) {
  m_bCanceled = false;
  m_bRunOnce = bRunOnce;
  if (tDiff < 0) {
    Suspend();
  } else {
    boost::posix_time::ptime pt(boost::posix_time::second_clock::local_time());
    m_TriggerTime = pt;
    m_TriggerTime += boost::posix_time::seconds(tDiff);

    m_tDiff = tDiff;
    m_rTimer.expires_from_now(boost::posix_time::seconds(tDiff));
    m_rTimer.async_wait(boost::bind(&LibMsgTask::Execute, shared_from_this(), boost::asio::placeholders::error));
  }
}
