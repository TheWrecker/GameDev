
#pragma once

#ifdef _WINDOWS

#include <Windows.h>

#include "win_init.h"

#define InstanceHandle HINSTANCE
#define PlatformImpl WindowsInitializer
#define SysTickValue LARGE_INTEGER

#endif // _WINDOWS

//TODO: Add other platforms