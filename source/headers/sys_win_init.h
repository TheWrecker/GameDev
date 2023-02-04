
#pragma once

#ifdef _WINDOWS

#include <Windows.h>

class WindowsInitializer
{
public:
	WindowsInitializer(HINSTANCE instance);
	~WindowsInitializer();

private:

	HINSTANCE instance_handle;
	HWND window_handle;
	WNDCLASSEX window_class;

};


#endif // _WINDOWS