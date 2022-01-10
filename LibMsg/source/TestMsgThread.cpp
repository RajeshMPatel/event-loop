//
//  LibMsgDllExport.h
//  LibMsg
//
//  Created by Rajesh Patel on 8/9/18.
//

#include "stdafx.h"
#include "TestMsgThread.h"
#include <iostream>

BEGIN_LIBMSG_MAP(TestMsgThread, LibMsgThread)
  ON_MESSAGE(cInitialize, TestMsgThread::OnInitialize)
  ON_MESSAGE(cCleanUp, TestMsgThread::OnCleanUp)
END_LIBMSG_MAP()

TestMsgThread::TestMsgThread() {
}

void TestMsgThread::OnInitialize(LM_WPARAM wParam, LM_LPARAM lParam) {
  std::cout << "==>OnInitialize" << endl;
}

void TestMsgThread::OnCleanUp(LM_WPARAM wParam, LM_LPARAM lParam) {
  std::cout << "==>OnCleanUp\n" << endl;
}
