
#include "defs_platform.h"
#include "sys_win_profiler.h"
#include "sys_profiler.h"

SystemProfiler::SystemProfiler()
	:cpu_load(0), cpu_consumption(0), ram_load(0), physical_memory_used(0), physical_memory_available(0), virtual_memory_used(0), virtual_memory_available(0)
{
	impl = std::make_unique<SysProfilerImpl>();
	impl->QueryRAMCapacity(physical_memory_total, virtual_memory_total);
	cpu_load = 0;
}

SystemProfiler::~SystemProfiler()
{
}

void SystemProfiler::Update()
{
}

std::size_t SystemProfiler::GetCPUConsumption()
{
	return cpu_consumption;
}

std::size_t SystemProfiler::GetCPULoad()
{
	return cpu_load;
}

std::size_t SystemProfiler::GetPhysMemUsed()
{
	return physical_memory_used;
}

std::size_t SystemProfiler::GetPhysMemAvailable()
{
	return physical_memory_available;
}

std::size_t SystemProfiler::GetPhysMemTotal()
{
	return physical_memory_total;
}
