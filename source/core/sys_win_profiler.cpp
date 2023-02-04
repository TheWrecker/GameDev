
#ifdef _WINDOWS

#include "sys_win_profiler.h"

WindowsProfiler::WindowsProfiler()
{
	GetSystemTimeAsFileTime(&ftime);
	memcpy(&last_CPU, &ftime, sizeof(FILETIME));

	this_process = GetCurrentProcess();
	GetProcessTimes(this_process, &ftime, &ftime, &fsys, &fuser);
	memcpy(&last_sys_CPU, &fsys, sizeof(FILETIME));
	memcpy(&last_user_CPU, &fuser, sizeof(FILETIME));

	PdhOpenQuery(NULL, NULL, &cpu_query);
	PdhAddEnglishCounter(cpu_query, L"\\Processor(_Total)\\% Processor Time", NULL, &cpu_total);
	PdhCollectQueryData(cpu_query);
}

WindowsProfiler::~WindowsProfiler()
{
	PdhRemoveCounter(cpu_total);
	PdhCloseQuery(cpu_query);
}

void WindowsProfiler::QueryRAMCapacity(std::size_t& physicalTotal, std::size_t& virtualTotal)
{
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	physicalTotal = memInfo.ullTotalPhys;
	virtualTotal = memInfo.ullTotalPageFile - memInfo.ullTotalPhys;
}

void WindowsProfiler::QueryCPUStats(std::size_t& gameConsumption, std::size_t& totalConsumption)
{

}

void WindowsProfiler::QueryRAMStats(std::size_t& physicalUsed, std::size_t& virtualUsed)
{
	//current process memory
	//PROCESS_MEMORY_COUNTERS_EX pmc;
	//GetProcessMemoryInfo(this_process, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	//virtualMemoryUsed = pmc.PrivateUsage;
	//physicalMemoryUsed = pmc.WorkingSetSize;

	////global memory
	//MEMORYSTATUSEX memInfo;
	//memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	//GlobalMemoryStatusEx(&memInfo);
	//ramLoad = (memInfo.ullTotalPhys - memInfo.ullAvailPhys) * 100 / (memInfo.ullTotalPhys);
	//physicalMemoryAvailable = memInfo.ullAvailPhys;
}


#endif // _WINDOWS

