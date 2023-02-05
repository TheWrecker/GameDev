
#pragma once

#include "interface_service.h"
#include "platfrom_defs.h"

class SystemTicker : public IService
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
