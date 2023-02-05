
#include "platform.h"

#include SysPlatformInitializerHeader

Platform::Platform(InstanceHandle handle)
{
	impl = std::make_unique<PlatformImpl>(handle);
	int i = 0;
}

Platform::~Platform()
{
}

bool Platform::ProcessPlatfromMessages()
{
	return impl->ProcessPlatformMessages();
}
