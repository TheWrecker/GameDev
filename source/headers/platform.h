
#pragma once

#include <memory>

#include "platfrom_defs.h"

class Platform
{
public:

	Platform(InstanceHandle handle);
	~Platform();

private:

	std::unique_ptr<PlatformImpl> impl;

};
