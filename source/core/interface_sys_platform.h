
#pragma once

class ISystemPlatform
{
public:
	virtual bool ProcessPlatformMessages() = 0;
	virtual void* GetWindowHandle() = 0;
};
