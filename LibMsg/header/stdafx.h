//
//  LibMsgDllExport.h
//  LibMsg
//
//  Created by Rajesh Patel on 8/9/18.
//

// Following Microsoft's precompiled header convention

#ifndef _libmsg_stdafx_h_
#define _libmsg_stdafx_h_

#ifdef _WIN32
  #define _WIN32_WINNT 0x0A00 // only support Windows 10 and up
  #include <tchar.h>
#endif

#include <stdio.h>
#include <vector>
#include "boost/asio.hpp"
#include <boost/thread/thread.hpp>

#endif // _libmsg_stdafx_h_
