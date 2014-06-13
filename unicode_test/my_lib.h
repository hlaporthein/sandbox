#ifndef MY_LIB_H
#define MY_LIB_H
#include <tchar.h>
#include <windows.h>

#ifdef _UNICODE
#define do_things _wdo_things
#endif //_UNICODE

#ifdef _UNICODE
#define FTEXT(func) _w ## func
#else
#define FTEXT(func) func
#endif // _UNICODE

int do_things(LPCTSTR s);

#endif // MY_LIB_H