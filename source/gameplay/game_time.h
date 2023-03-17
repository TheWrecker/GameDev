
#ifndef GAME_TIME_H
	#define GAME_TIME_H

	#include <functional>
	#include <list>

	#include "interface_service.h"

	class SystemTicker;

	typedef std::function<void()> Callback;

	struct PeriodicTaskInfo
	{
		PeriodicTaskInfo(Callback callback, unsigned int interval)
			:func(callback), call_limit(interval), current(0)
		{
		}

		Callback func;
		unsigned int
			call_limit,
			current;
	};

	typedef std::list<PeriodicTaskInfo>::const_iterator PeriodicTask;

	class GameTime : public IService
	{
	public:

		GameTime(SystemTicker* ticker);
		~GameTime();

		void Update() override;
		void SetGameSpeed(float value);
		void Pause();
		void Resume();

		PeriodicTask RegisterFunction(Callback func, unsigned int interval, bool realTime = false);
		void RemoveFunction(PeriodicTask target, bool realTime = false);

		float GetGameSpeed();
		float GetGameTime();
		float GetRealTime();

	private:

		std::list<PeriodicTaskInfo>
			realtime_container,
			gametime_container;

		SystemTicker* ticker;
		float
			speed,
			real_time,
			game_time;
		bool paused;
	};

#endif // !GAME_TIME_H


