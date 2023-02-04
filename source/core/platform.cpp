
#include "platform.h"

#ifdef _WINDOWS

#include "win_init.h"

#endif // _WINDOWS

Platform::Platform(InstanceHandle handle)
{
	impl = std::make_unique<PlatformImpl>(handle);
}

Platform::~Platform()
{
}
