
#include"defs_platform.h"
#include <Psapi.h>
#include <TCHAR.h>

#include "sys_profiler.h"

SystemProfiler::SystemProfiler()
	:cpu_load(0), cpu_consumption(0), ram_load(0), physical_memory_used(0), physical_memory_available(0),
	virtual_memory_used(0), virtual_memory_available(0)
{
}

SystemProfiler::~SystemProfiler()
{
	#ifdef _WINDOWS

		PdhRemoveCounter(cpu_total);
		PdhCloseQuery(cpu_query);

	#endif // _WINDOWS
}

bool SystemProfiler::Initialize()
{
	#ifdef _WINDOWS

		GetSystemTimeAsFileTime(&ftime);
		memcpy(&last_CPU, &ftime, sizeof(FILETIME));

		this_process = GetCurrentProcess();
		GetProcessTimes(this_process, &ftime, &ftime, &fsys, &fuser);
		memcpy(&last_sys_CPU, &fsys, sizeof(FILETIME));
		memcpy(&last_user_CPU, &fuser, sizeof(FILETIME));

		PdhOpenQuery(NULL, NULL, &cpu_query);
		PdhAddEnglishCounter(cpu_query, L"\\Processor(_Total)\\% Processor Time", NULL, &cpu_total);
		PdhCollectQueryData(cpu_query);

	#endif // _WINDOWS

	return true;
}

void SystemProfiler::Update()
{
}

std::uint64_t SystemProfiler::GetCPUConsumption()
{
	return cpu_consumption;
}

std::uint64_t SystemProfiler::GetCPULoad()
{
	return cpu_load;
}

std::uint64_t SystemProfiler::GetPhysMemUsed()
{
	return physical_memory_used;
}

std::uint64_t SystemProfiler::GetPhysMemAvailable()
{
	return physical_memory_available;
}

std::uint64_t SystemProfiler::GetPhysMemTotal()
{
	return physical_memory_total;
}

void SystemProfiler::QueryRAMCapacity(uint64_t& physicalTotal, uint64_t& virtualTotal)
{
	#ifdef _WINDOWS

		MEMORYSTATUSEX _mem_info = {};
		_mem_info.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&_mem_info);
		physicalTotal = _mem_info.ullTotalPhys;
		virtualTotal = _mem_info.ullTotalPageFile - _mem_info.ullTotalPhys;

	#endif // _WINDOWS
}

void SystemProfiler::QueryCPUStats(uint64_t& gameConsumption, uint64_t& totalConsumption)
{
	#ifdef _WINDOWS


	#endif // _WINDOWS
}

void SystemProfiler::QueryRAMStats(uint64_t& physicalUsed, uint64_t& virtualUsed)
{
	#ifdef _WINDOWS

		//current process memory
		PROCESS_MEMORY_COUNTERS_EX _pmc = {};
		GetProcessMemoryInfo(this_process, (PROCESS_MEMORY_COUNTERS*)&_pmc, sizeof(_pmc));
		virtual_memory_used = _pmc.PrivateUsage;
		physical_memory_used = _pmc.WorkingSetSize;

		//global memory
		MEMORYSTATUSEX _memInfo = {};
		_memInfo.dwLength = sizeof(MEMORYSTATUSEX);
		GlobalMemoryStatusEx(&_memInfo);
		ram_load = (std::uint8_t)(_memInfo.ullTotalPhys - _memInfo.ullAvailPhys) * 100 / (_memInfo.ullTotalPhys);
		physical_memory_available = _memInfo.ullAvailPhys;

	#endif // _WINDOWS
}
