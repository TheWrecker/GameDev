
#pragma once

#include <cstddef>
#include <memory>

#include "sys_interface_profiler.h"

class SystemProfiler
{
public:
	SystemProfiler();
	~SystemProfiler();

	void Update();

	std::size_t GetCPUConsumption();
	std::size_t GetCPULoad();
	std::size_t GetPhysMemUsed();
	std::size_t GetPhysMemAvailable();
	std::size_t GetPhysMemTotal();

private:

	std::unique_ptr<SystemProfilerInterface> impl;

	std::size_t
		cpu_consumption,
		cpu_load,
		ram_load,
		physical_memory_used,
		physical_memory_available,
		physical_memory_total,
		virtual_memory_used,
		virtual_memory_available,
		virtual_memory_total;

};
