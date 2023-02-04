
#pragma once

#include "platfrom_defs.h"

class SystemTicker
{
public:
	SystemTicker();
	~SystemTicker();

	void Update();
	float GetLastTickDuration();

private:

	SysTickValue
		start_tick,
		previous_tick,
		current_tick;

	double frequency;

	float last_tick_duration;

};
