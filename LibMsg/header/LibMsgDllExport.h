//
//  LibMsgDllExport.h
//  LibMsg
//
//  Created by Rajesh Patel on 8/9/18.
//

#ifndef Libmsgdllexport_h
#define Libmsgdllexport_h

#ifdef LIBMSG_DLL_EXPORT
  // You can make this library a dll by defining exports in Windows
  #ifndef LibMsgDllExport
    #ifdef _WIN32
      #define LibMsgDllExport  __declspec( dllexport )
      #define LibMsgDllExportPrefix  __declspec( dllexport )
      #define LibMsgDllExportSuffix
    #else
      #define LibMsgDllExport  __attribute__ ((visibility ("default")))
      #define LibMsgDllExportPrefix
      #define LibMsgDllExportSuffix  __attribute__ ((visibility ("default")))
    #endif
  #endif // LibMsgDllExport
#else // LIBMSG_DLL_EXPORT
  #ifndef LibMsgDllExport
    #define LibMsgDllExport
    #define LibMsgDllExportPrefix
    #define LibMsgDllExportSuffix
  #endif // LibMsgDllExport
#endif // LIBMSG_DLL_EXPORT

#endif // Libmsgdllexport_h
