#ifndef MY_TCHAR_H
#define MY_TCHAR_H


#ifdef _MY_UNICODE

#define MY_TEXT(str) L##str
#define FTEXT(func) _w ## func

#else

#define MY_TEXT(str) str
#define FTEXT(func) func

#endif //_MY_UNICODE

#define _FT(func) FTEXT(func)

#if !defined(_WINDOWS_H) && !defined(_WINDOWS_)
#ifdef _MY_UNICODE

typedef unsigned short wchar_t;

#endif //_MY_UNICODE

#define _T(str) MY_TEXT(str)

#endif // _WINDOWS_H

#ifdef _MY_UNICODE

typedef wchar_t MY_TCHAR;

#else

typedef char MY_TCHAR;

#endif //_MY_UNICODE

typedef const MY_TCHAR *MY_LPCTSTR;

// Function definition
#ifdef _MY_UNICODE

#define vsnprintf _vsnwprintf
#define _tfopen _wfopen
#define _tprintf wprintf
#define _tfprintf fwprintf

#define strerror _wcserror

#else

#define _tfopen fopen
#define _tprintf printf
#define _tfprintf fprintf

#endif //_MY_UNICODE

#endif // MY_TCHAR_H