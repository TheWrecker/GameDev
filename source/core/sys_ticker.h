
#ifndef SYS_TICKER_H
	#define SYS_TICKER_H

	#include <cstddef>
	#include <chrono>

	#include "interface_service.h"
	#include "defs_platform.h"

	class SystemTicker : public IService
	{
	public:
		SystemTicker();
		~SystemTicker();

		void Update() override;

		float GetLastTickDuration();
		float GetAverageTickDuration();
		std::size_t GetTickPerSecond();

	private:
		/*SysTickValue
			start_tick,
			previous_tick,
			current_tick,
			frequency_i;*/

		float
			/*frequency_d,*/
			last_tick_duration,
			current_tick_sum,
			average_tick_duration;

		std::chrono::high_resolution_clock::time_point
			current_tp,
			prev_tp;

		std::size_t
			tick_per_second,
			current_tick_count;
	};

#endif // !SYS_TICKER_H
