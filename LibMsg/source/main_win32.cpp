// main_win32.cpp : Defines the entry point for the console application.


#include "stdafx.h"
#include "TestMsgThread.h"

TestMsgThread g_rMesgThread;

int main(int argc, char* argv[]) {
  std::cout << "Welcome to the world of Asynchronous Programming!!" << std::endl;
  // We start our message thread. This is separate from the main application thread.
  // All our processing is done in this thread
  // This also allows us to register it as a Windows Service
  g_rMesgThread.Start();
  g_rMesgThread.PostMessage(cInitialize, 0, 0);

  SetConsoleCtrlHandler(
    (PHANDLER_ROUTINE) CtrlHandler,
    TRUE);

  while (1)
  {
    sleep(1);
  }
  return 0;
}

BOOL CtrlHandler(unsigned long fdwCtrlType) {
  switch( fdwCtrlType )
  {
    // Handle the CTRL-C signal.
    case CTRL_C_EVENT:
      g_rMesgThread.SendMessage(cCleanUp, 0, 0);
      g_rMesgThread.Stop();
      return( FALSE );

    default:
      return FALSE;
  }
}
