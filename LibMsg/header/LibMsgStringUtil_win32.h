#ifndef libmsg_stringutil_win32_h_
#define libmsg_stringutil_win32_h_

#ifdef WIN32
#include <string>

bool Utf8ToUnicode(const std::string& strUtf8, std::wstring& strUnicode);
bool UnicodeToUtf8(const std::wstring& strUnicode, std::string& strUtf8);
#endif // WIN32

#endif // libmsg_stringutil_win32_h_
