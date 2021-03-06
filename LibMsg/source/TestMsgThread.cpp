//
//  TestMsgThread.cpp
//  LibMsg
//
//  Created by Rajesh Patel on 8/9/18.
//

#include "stdafx.h"
#include "TestMsgThread.h"
#include <iostream>
#include "LibMsgTask.h"
#include "LibMsgNetworkChange.h"


BEGIN_LIBMSG_MAP(TestMsgThread, LibMsgThread)
  ON_MESSAGE(cInitialize, TestMsgThread::OnInitialize)
  ON_MESSAGE(cCleanUp, TestMsgThread::OnCleanUp)
  ON_MESSAGE(cDoSomethingTask, TestMsgThread::OnDoSomethingTask)
  ON_MESSAGE(cNetworkChanged, TestMsgThread::OnNetworkChanged)
END_LIBMSG_MAP()

TestMsgThread::TestMsgThread() {
}

void TestMsgThread::OnInitialize(LM_WPARAM wParam, LM_LPARAM lParam) {
  std::cout << "==>OnInitialize. Time = " << time(nullptr) << endl;
  boost::shared_ptr<LibMsgTask> doSomethingTask = boost::shared_ptr<LibMsgTask>
    (new LibMsgTask(this, cDoSomethingTask));
  doSomethingTask->SetIntervalFromNow(5);
  
  boost::shared_ptr<LibMsgNetworkChange> networkChanged = boost::shared_ptr<LibMsgNetworkChange>
    (new LibMsgNetworkChange(this, cNetworkChanged));
  networkChanged->RegisterNotification();
}

void TestMsgThread::OnCleanUp(LM_WPARAM wParam, LM_LPARAM lParam) {
  std::cout << "==>OnCleanUp\n" << endl;
}

void TestMsgThread::OnDoSomethingTask(LM_WPARAM wParam, LM_LPARAM lParam) {
  std::cout << "==>OnDoSomethingTask. Time = " << time(nullptr) << " - Task Fired to do something" << endl;
}

void TestMsgThread::OnNetworkChanged(LM_WPARAM wParam, LM_LPARAM lParam) {
  std::cout << "==>OnNetworkChanged." << endl;
}
