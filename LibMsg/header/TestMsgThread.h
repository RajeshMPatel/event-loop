//
//  TestMsgThread.h
//  LibMsg
//
//  Created by Rajesh Patel on 8/9/18.
//

#ifndef Testmsgthread_h
#define Testmsgthread_h

#include "LibMsgThread.h"
#include "MessageIds.h"

class TestMsgThread : public LibMsgThread
{
public:
  TestMsgThread();
  virtual ~TestMsgThread() {}

public:
  DECLARE_LIBMSG_MAP();

private:
  void OnInitialize(LM_WPARAM wParam, LM_LPARAM lParam);
  void OnCleanUp(LM_WPARAM wParam, LM_LPARAM lParam);
  void OnDoSomethingTask(LM_WPARAM wParam, LM_LPARAM lParam);
  void OnNetworkChanged(LM_WPARAM wParam, LM_LPARAM lParam);
};

#endif /* Testmsgthread_h */

