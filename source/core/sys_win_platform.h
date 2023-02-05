
#pragma once

#include "interface_sys_platform.h"

#ifdef _WINDOWS

#include <Windows.h>

class WindowsInitializer : public ISystemPlatform
{
public:
	WindowsInitializer(HINSTANCE instance);
	~WindowsInitializer();

	void* GetWindowHandle() override;
	bool ProcessPlatformMessages() override;

private:

	HINSTANCE instance_handle;
	HWND window_handle;
	WNDCLASSEX window_class;

};


#endif // _WINDOWS