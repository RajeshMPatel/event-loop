//
//  LibMsgTask.h
//  LibMsg
//
//  Created by Rajesh Patel on 8/9/18.
//

#ifndef Libmsgtask_h
#define Libmsgtask_h

#include "LibMsgDllExport.h"
#include "LibMsgThread.h"
#include <boost/enable_shared_from_this.hpp>

// This allows user to create timers. You can create a timeout event m_tdiff time from now
// When the timeout occurs, it will post a message with arguments that were passed during
// the constructor of the class. You can always change these by setting LPARAM and WPARAM
// After setting LPARAM and WPARAM, cal the SetIntervalFromNow again.

class LibMsgTask : public boost::enable_shared_from_this<LibMsgTask>
{
public:

  LibMsgDllExport LibMsgTask(
    LibMsgThread* pLibMsgThread,
    int    nMsgId,
    void*  wParam = nullptr,
    void*  lParam = nullptr);

  LibMsgDllExport  virtual ~LibMsgTask();
  LibMsgDllExport  virtual void Execute(const boost::system::error_code& error);
  LibMsgDllExport  void         SetIntervalFromNow(const long tDiff, bool bRunOnce=false);
  LibMsgDllExport  void         Suspend();
  LibMsgDllExport  void         Cancel();
  LibMsgDllExport  void         SetWParam(void* w) { m_wparam = w; }
  LibMsgDllExport  void         SetLParam(void* l) { m_lparam = l; }

  LibMsgDllExport  boost::posix_time::ptime& GetAbsoluteTriggerTime() { return m_TriggerTime; }
  LibMsgDllExport  long         GetInterval() { return m_tDiff; }

  bool GetRunOnce() const { return m_bRunOnce; };

private:

  boost::asio::deadline_timer m_rTimer;
  LibMsgThread* m_pLibMsgThread;
  long          m_tDiff;
  int           m_nMsgId;
  void*         m_wparam;
  void*         m_lparam;
  bool          m_bCanceled;
  bool          m_bRunOnce;

  boost::posix_time::ptime m_TriggerTime;
};

#endif // Libmsgtask_h
