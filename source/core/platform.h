
#ifndef PLATFORM_H
	#define PLATFORM_H

	#ifdef _WINDOWS

		#include "defs_platform.h"

		#include "interface_service.h"

		class Platform : public IService
		{
		public:

			Platform(InstanceHandle handle);
			~Platform();

			//TODO: add more parameters
			void SetWindowsParameters(int width = 800, int height = 600);
			bool Initialize() override;
			bool ProcessPlatformMessages();

			HWND GetWindowHandle();
			int GetWindowHeight();
			int GetWindowWidth();

		private:
			HINSTANCE instance_handle;
			HWND window_handle;
			WNDCLASSEX window_class;
			int width, height;
		};

	#endif // _WINDOWS

	//TODO: Add other platforms

#endif // !PLATFORM_H
