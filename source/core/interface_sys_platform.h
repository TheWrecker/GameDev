
#ifndef ISYSTEM_PLATFORM_H
	#define ISYSTEM_PLATFORM_H

	class ISystemPlatform
	{
	public:
		virtual bool ProcessPlatformMessages() = 0;
		virtual void* GetWindowHandle() = 0;
	};

#endif // !ISYSTEM_PLATFORM_H
