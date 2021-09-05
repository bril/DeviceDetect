#ifndef WIN32TYPES_H_
#define WIN32TYPES_H_

#define  STDCALL  __stdcall
#define VOLATILE  volatile

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>
#include <string>
#include <wchar.h>

typedef ::HANDLE     Handle;
typedef ::HMODULE    Hmodule;
typedef ::SYSTEMTIME SystemTime;
typedef ::FILETIME   FileTime;
typedef ::FARPROC    FarProc;
typedef ::HWND       HWnd;
typedef ::HRSRC		 HResource;

typedef ::LONG Long;


typedef __int64 int64;
typedef unsigned __int64 uint64;
typedef __int32 int32;
typedef unsigned __int32 uint32;


#endif /*WIN32TYPES_*/
