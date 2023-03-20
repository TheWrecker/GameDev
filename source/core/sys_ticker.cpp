
#include "sys_ticker.h"

SystemTicker::SystemTicker()
	:last_tick_duration(0.0f),current_tick_sum(0.0f), average_tick_duration(0.0f), tick_per_second(0), current_tick_count(0)
{
#ifdef _WINDOWS
	//QueryPerformanceCounter(&start_tick);
	//current_tick = previous_tick = start_tick;
	//QueryPerformanceFrequency(&frequency_i);
	//frequency_d = static_cast<double>(frequency_i.QuadPart);
#endif // _WINDOWS
	current_tp = prev_tp = std::chrono::high_resolution_clock::now();
}

SystemTicker::~SystemTicker()
{
}

void SystemTicker::Update()
{
	current_tick_count++;
	//previous_tick = current_tick;
	//QueryPerformanceCounter(&current_tick);
	//long long _last = (current_tick.QuadPart - previous_tick.QuadPart) / (frequency_i.QuadPart /** 1000*/);
	current_tp = std::chrono::high_resolution_clock::now();
	//last_tick_duration = static_cast<double>((current_tick.QuadPart - previous_tick.QuadPart) / frequency_d );
	last_tick_duration = std::chrono::duration<float>(current_tp - prev_tp).count();
	current_tick_sum += last_tick_duration;
	prev_tp = current_tp;
	if (current_tick_sum > 1.0f)
	{
		tick_per_second = current_tick_count;
		average_tick_duration = current_tick_sum / static_cast<float>(current_tick_count);
		current_tick_sum -= 1.0f;
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
