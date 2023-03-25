
#include "platform.h"
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

SysWindowHandle Platform::GetWindowHandle()
{
	return static_cast<SysWindowHandle>(impl->GetWindowHandle());
}

bool Platform::ProcessPlatformMessages()
{
	return impl->ProcessPlatformMessages();
}
