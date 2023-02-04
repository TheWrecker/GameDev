
#include "platform.h"

#include SysPlatformInitializerHeader

Platform::Platform(InstanceHandle handle)
{
	impl = std::make_unique<PlatformImpl>(handle);
}

Platform::~Platform()
{
}
