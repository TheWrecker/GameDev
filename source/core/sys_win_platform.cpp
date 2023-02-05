
#ifdef _WINDOWS

#include <assert.h>

#include "sys_win_platform.h"


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;
	case  WM_CLOSE:
	{
		PostQuitMessage(0);
	}
	break;
	default:
	break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

WindowsInitializer::WindowsInitializer(HINSTANCE instance)
	:instance_handle(instance)
{
	//TODO: less magic values
	ZeroMemory(&window_class, sizeof(window_class));
	window_class.cbSize = sizeof(WNDCLASSEX);
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = WindowProc;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = instance_handle;
	window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
	window_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	window_class.lpszClassName = L"GameDev";
	RegisterClassEx(&window_class);

	//TODO: less magic values
	auto flags = WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU;
	window_handle = CreateWindow(window_class.lpszClassName, window_class.lpszClassName, flags, 100, 100, 800, 600, nullptr, nullptr, instance_handle, nullptr);

	assert(window_handle);

	ShowWindow(window_handle, 1);
	UpdateWindow(window_handle);
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

}

WindowsInitializer::~WindowsInitializer()
{
	DestroyWindow(window_handle);
}

bool WindowsInitializer::ProcessPlatformMessages()
{
	bool _should_Shutdown = false;
	MSG _msg;
	ZeroMemory(&_msg, sizeof(_msg));
	while (PeekMessage(&_msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&_msg);
		DispatchMessage(&_msg);
		if (_msg.message == WM_QUIT)
		{
			_should_Shutdown = true;
			break;
		}
	}
	return _should_Shutdown;
}

#endif // _WINDOWS
