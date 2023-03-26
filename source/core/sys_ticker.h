
#ifndef SYS_TICKER_H
	#define SYS_TICKER_H

	#include <cstdint>
	#include <chrono>

	#include "interface_service.h"

	class SystemTicker : public IService
	{
	public:
		SystemTicker();
		~SystemTicker();

		void Update() override;

		float GetLastTickDuration();
		float GetAverageTickDuration();
		std::uint_fast16_t GetTickPerSecond();

	private:

		float
			last_tick_duration,
			current_tick_sum,
			average_tick_duration;

		std::chrono::high_resolution_clock::time_point
			current_tp,
			prev_tp;

		std::uint_fast16_t
			tick_per_second,
			current_tick_count;
	};

#endif // !SYS_TICKER_H
