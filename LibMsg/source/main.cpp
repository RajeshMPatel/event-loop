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
  
  // We start our message thread. This is separate from the main application thread.
  // All our processing is done in this thread
  
  g_rMessageThread.Start();
  g_rMessageThread.PostMessage(cInitialize, 0, 0);

  return 0;
}
