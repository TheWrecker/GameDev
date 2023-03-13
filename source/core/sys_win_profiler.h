
#ifndef SYS_WIN_PROFILER_H
	#define SYS_WIN_PROFILER_H

	#ifdef _WINDOWS

		#include <pdh.h>
		#include <Psapi.h>
		#include <TCHAR.h>

		#include "defs_platform.h"
		#include "interface_sys_profiler.h"

		class WindowsProfiler : public ISystemProfiler
		{
		public:
			WindowsProfiler();
			~WindowsProfiler();

			void QueryRAMCapacity(std::size_t& physicalTotal, std::size_t& virtualTotal) override;
			void QueryCPUStats(std::size_t& gameConsumption, std::size_t& totalConsumption) override;
			void QueryRAMStats(std::size_t& physicalUsed, std::size_t& virtualUsed) override;

		private:
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
		};

	#endif // _WINDOWS

#endif // !SYS_WIN_PROFILER_H
