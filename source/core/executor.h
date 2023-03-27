
#ifndef EXECUTOR_H
	#define EXECUTOR_H

	#include <chrono>
	#include <functional>
	#include <list>

	#include "interface_service.h"

	class SystemTicker;

	typedef std::function<void()> Callback;

	struct PeriodicTaskInfo
	{
		PeriodicTaskInfo(Callback callback, float interval)
			:func(callback), call_limit(interval), current(0)
		{
		}

		Callback func;
		float
			call_limit,
			current;
	};

	typedef std::list<PeriodicTaskInfo>::const_iterator PeriodicTask;

	class Supervisor;

	class Executor : public IService
	{
	public:
		Executor(Supervisor* parent);
		~Executor();

		bool Initialize() override;
		void Update() override;
		void SetGameSpeed(float value);
		void Pause();
		void Resume();

		PeriodicTask RegisterPeriodicTask(Callback func, float interval, bool realTime = false);
		void RemovePeriodicTask(PeriodicTask target, bool realTime = false);

		float GetGameSpeed();
		float GetGameTime();
		float GetRealTime();

	private:
		Supervisor* supervisor;

		std::list<PeriodicTaskInfo>
			realtime_container,
			gametime_container;

		SystemTicker* ticker;
		float
			diff,
			speed,
			real_time,
			game_time;
		bool paused;
	};

#endif // !EXECUTOR_H
