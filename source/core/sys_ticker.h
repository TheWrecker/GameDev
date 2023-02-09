
#pragma once

#include "interface_service.h"
#include "platfrom_defs.h"

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
	SysTickValue
		start_tick,
		previous_tick,
		current_tick;

	double frequency;

	float 
		last_tick_duration,
		current_tick_sum,
		average_tick_duration;

	std::size_t
		tick_per_second,
		current_tick_count;
};
