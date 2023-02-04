
#include "initializer.h"

Initializer::Initializer(InstanceHandle instance)
{
	platfrom = std::make_unique<Platform>(instance);
	ticker = std::make_unique<SystemTicker>();
	profiler = std::make_unique<SystemProfiler>();
}

Initializer::~Initializer()
{
}
