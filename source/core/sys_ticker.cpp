
#include "sys_ticker.h"

SystemTicker::SystemTicker()
	:last_tick_duration(0.0f)
{
#ifdef _WINDOWS
	QueryPerformanceCounter(&start_tick);
	current_tick = previous_tick = start_tick;
	SysTickValue _freq;
	QueryPerformanceFrequency(&_freq);
	frequency = static_cast<double>(_freq.QuadPart);
#endif // _WINDOWS
}

SystemTicker::~SystemTicker()
{
}

void SystemTicker::Update()
{
	previous_tick = current_tick;
	QueryPerformanceCounter(&current_tick);
	last_tick_duration = static_cast<float>((current_tick.QuadPart - previous_tick.QuadPart) / frequency);
}

float SystemTicker::GetLastTickDuration()
{
	return last_tick_duration;
}
