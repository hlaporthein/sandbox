#ifndef MY_LIB_H
#define MY_LIB_H

#ifdef _UNICODE

typedef unsigned short wchar_t;
typedef wchar_t TCHAR;

#define TEXT(str) L##str
#define FTEXT(func) _w ## func
#define _tfopen _wfopen
#define _tprintf wprintf

#else

typedef char TCHAR;

#define TEXT(str) str
#define FTEXT(func) func
#define _tfopen fopen
#define _tprintf printf

#endif //_UNICODE

typedef const TCHAR *LPCTSTR;

#define _T(str) TEXT(str)


#ifdef _UNICODE
#define do_things _wdo_things
#endif //_UNICODE

int do_things(LPCTSTR s);

#endif // MY_LIB_H