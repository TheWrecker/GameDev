
#pragma once

#ifdef _WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define SysPlatformInitializerHeader "sys_win_platform.h"
#define SysProfilerHeader "sys_win_profiler.h"
#define SysProfilerImpl WindowsProfiler
#define SysWindowHandle HWND
#define PlatformImpl WindowsInitializer
#define InstanceHandle HINSTANCE
#define SysTickValue LARGE_INTEGER

#endif // _WINDOWS

//TODO: Add other platforms