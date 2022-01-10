//
//  main.cpp
//  LibMsg
//
//  Created by Rajesh Patel on 8/9/16.
//

#include <iostream>
#include "TestMsgThread.h"

TestMsgThread g_rMessageThread;

int main(int argc, const char * argv[]) {
  std::cout << "Welcome to the world of Asynchronous Programming!!" << std::endl;
  
  g_rMessageThread.Start();
  g_rMessageThread.PostMessage(cInitialize, 0, 0);

  return 0;
}
