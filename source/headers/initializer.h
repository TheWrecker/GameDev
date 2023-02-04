
#pragma once

#include <memory>
#include <Windows.h>

#include "platfrom_defs.h"
#include "sys_ticker.h"
#include "sys_profiler.h"

#include "platform.h"

class Initializer
{

public:

	Initializer(InstanceHandle instance);
	~Initializer();

private:

	std::unique_ptr<Platform> platfrom;
	std::unique_ptr<SystemTicker> ticker;
	std::unique_ptr<SystemProfiler> profiler;
};