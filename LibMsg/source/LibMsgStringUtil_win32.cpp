#ifdef WIN32
#include "stdafx.h"
#include "LibMsgStringUtil_win32.h"

// Just run of the mill conversion from ASCII TO UNICODE and Vice Versa
bool Utf8ToUnicode(const std::string& strUtf8, std::wstring& strUnicode) {
  int nRetSize = MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), static_cast<int>(strUtf8.length()), NULL, 0);
  if (nRetSize == 0)
    return false;

  wchar_t* pchWString = NULL;
  try
  {
    pchWString = new wchar_t[nRetSize + 1];
  } catch(...) {
    pchWString = NULL;
  }

  if (pchWString == NULL)
    return false;

  nRetSize = MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), static_cast<int>(strUtf8.length()), pchWString, nRetSize);
  if (nRetSize == 0) {
    delete [] pchWString;
    return false;
  }

  pchWString[nRetSize] = 0;
  strUnicode = pchWString;
  return true;
}

bool UnicodeToUtf8(const std::wstring& strUnicode, std::string& strUtf8) {    
  int nRetSize = WideCharToMultiByte(CP_UTF8, 0, strUnicode.c_str(), static_cast<int>(strUnicode.length()), NULL, 0, NULL, NULL);
  if (nRetSize == 0)
    return false;

  char* pchMbString = NULL;
  try
  {
    pchMbString = new char[nRetSize + 1];
  } catch(...) {
    pchMbString = NULL;
  }

  if (pchMbString == NULL)
    return false;

  nRetSize = WideCharToMultiByte(CP_UTF8, 0, strUnicode.c_str(), static_cast<int>(strUnicode.length()), pchMbString, nRetSize, NULL, NULL);
  if (nRetSize != 0) {
    delete [] pchMbString;
    return false;
  }

  pchMbString[nRetSize] = 0;
  strUtf8 = pchMbString;
  return true;
}
#endif // WIN32