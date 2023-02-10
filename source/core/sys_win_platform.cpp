
#ifdef _WINDOWS

#include <assert.h>
#include <Windows.h>

#include "directxtk/Mouse.h"
#include "directxtk/Keyboard.h"
#include "../External/ImGui/imgui.h"
#include "../External/ImGui/imgui_impl_win32.h"

#include "sys_win_platform.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
		break;
	}
	case WM_ACTIVATEAPP:
	{
		DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
		DirectX::Mouse::ProcessMessage(message, wParam, lParam);
		break;
	}
	case WM_MOUSEACTIVATE:
	{
		return MA_ACTIVATEANDEAT;
		break;
	}
	//TODO: Reset keyboard and mouse trackers on activation
	case WM_ACTIVATE:
	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
	{
		auto& io = ImGui::GetIO();
		if (io.WantCaptureMouse)
			return ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
		else
			DirectX::Mouse::ProcessMessage(message, wParam, lParam);
		break;
	}
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		auto& io = ImGui::GetIO();
		if (io.WantCaptureKeyboard)
			return ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
		else
			DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
		break;
	}
	case WM_SYSKEYDOWN:
	{
		auto& io = ImGui::GetIO();
		if (io.WantCaptureKeyboard)
			return ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);
		else
			DirectX::Mouse::ProcessMessage(message, wParam, lParam);
		break;
		if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
		{
			//...
		}
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	case  WM_CLOSE:
	{
		PostQuitMessage(0);
		break;
	}
	default:
	{
		break;
	}
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

void* WindowsInitializer::GetWindowHandle()
{
	return static_cast<void*>(window_handle);
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
