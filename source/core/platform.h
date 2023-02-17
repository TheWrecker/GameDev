
#pragma once

#include <memory>

#include "interface_service.h"
#include "defs_platform.h"

#include SysPlatformInitializerHeader

class Platform : public IService
{
public:

	Platform(InstanceHandle handle);
	~Platform();

	SysWindowHandle GetWindowHandle();
	bool ProcessPlatfromMessages();

private:

	std::unique_ptr<PlatformImpl> impl;

};
