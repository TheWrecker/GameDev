
#include "sys_ticker.h"

SystemTicker::SystemTicker()
	:last_tick_duration(0.0f),current_tick_sum(0.0f), average_tick_duration(0.0f), tick_per_second(0), current_tick_count(0)
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
	current_tick_count++;
	previous_tick = current_tick;
	QueryPerformanceCounter(&current_tick);
	last_tick_duration = static_cast<float>((current_tick.QuadPart - previous_tick.QuadPart) / frequency);
	current_tick_sum += last_tick_duration;
	if (current_tick_sum > 1.0f)
	{
		current_tick_sum -= 1.0f;
		tick_per_second = current_tick_count;
		current_tick_count = 0;
	}
}

float SystemTicker::GetLastTickDuration()
{
	return last_tick_duration;
}

float SystemTicker::GetAverageTickDuration()
{
	return average_tick_duration;
}

std::size_t SystemTicker::GetTickPerSecond()
{
	return tick_per_second;
}
