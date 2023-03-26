
#ifndef DEFS_PLATFORM_H
	#define DEFS_PLATFORM_H

	#ifdef _WINDOWS
	
		#define NOMINMAX
		#define WIN32_LEAN_AND_MEAN
		#include <Windows.h>

		#define SysWindowHandle HWND
		#define InstanceHandle HINSTANCE
		#define SysTickValue LARGE_INTEGER

	#endif // _WINDOWS

	//TODO: add other platforms

#endif // !DEFS_PLATFORM_H
