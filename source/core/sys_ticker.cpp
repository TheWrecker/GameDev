
#include "sys_ticker.h"

SystemTicker::SystemTicker()
	:last_tick_duration(0.0f),current_tick_sum(0.0f), average_tick_duration(0.0f), tick_per_second(0), current_tick_count(0)
{
	current_tp = prev_tp = std::chrono::high_resolution_clock::now();
}

SystemTicker::~SystemTicker()
{
}

void SystemTicker::Update()
{
	current_tp = std::chrono::high_resolution_clock::now();
	last_tick_duration = std::chrono::duration<float>(current_tp - prev_tp).count();
	current_tick_sum += last_tick_duration;
	current_tick_count++;
	prev_tp = current_tp;
	if (current_tick_sum > 1.0f)
	{
		tick_per_second = current_tick_count;
		average_tick_duration = current_tick_sum / (float)(current_tick_count);
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

std::uint_fast16_t SystemTicker::GetTickPerSecond()
{
	return tick_per_second;
}
