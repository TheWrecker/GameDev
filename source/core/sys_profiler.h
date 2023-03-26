
#ifndef SYS_PROFILER_H
	#define SYS_PROFILER_H

	#include <cstdint>
	#include "defs_platform.h"
	#include <Pdh.h>

	#include "interface_service.h"

	class SystemProfiler : public IService
	{
	public:
		SystemProfiler();
		~SystemProfiler();

		bool Initialize() override;
		void Update() override;

		std::uint64_t GetCPUConsumption();
		std::uint64_t GetCPULoad();
		std::uint64_t GetPhysMemUsed();
		std::uint64_t GetPhysMemAvailable();
		std::uint64_t GetPhysMemTotal();

	private:

		void QueryRAMCapacity(uint64_t& physicalTotal, uint64_t& virtualTotal);
		void QueryCPUStats(uint64_t& gameConsumption, uint64_t& totalConsumption);
		void QueryRAMStats(uint64_t& physicalUsed, uint64_t& virtualUsed);

		std::uint8_t
			cpu_consumption,
			cpu_load,
			ram_load;

		std::uint64_t
			physical_memory_used,
			physical_memory_available,
			physical_memory_total,
			virtual_memory_used,
			virtual_memory_available,
			virtual_memory_total;

	#ifdef _WINDOWS

		unsigned long long
			last_CPU,
			last_sys_CPU,
			last_user_CPU,
			current_CPU,
			current_sys_CPU,
			current_user_CPU;

		HANDLE this_process;

		FILETIME
			ftime,
			fsys,
			fuser;

		PDH_HQUERY cpu_query;
		PDH_HCOUNTER cpu_total;

	#endif // _WINDOWS

	};

#endif // !SYS_PROFILER_H
