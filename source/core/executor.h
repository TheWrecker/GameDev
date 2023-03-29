
#ifndef EXECUTOR_H
	#define EXECUTOR_H

	#include <chrono>
	#include <functional>
	#include <list>
	#include <thread>

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

	class ExecutionThread
	{
	public:
		ExecutionThread(Callback func)
			:paused(false), shutdown(false), exec_thread(), func(func)
		{}

		void PauseExecution();
		void ResumeExecution();

	private:
		friend class Executor;

		void Shutdown();

		bool
			paused,
			shutdown;

		Callback func;
		std::thread exec_thread;
	};

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

		ExecutionThread* StartExecution(Callback func);
		void StopExecution(ExecutionThread* target);

		float GetGameSpeed();
		float GetGameTime();
		float GetRealTime();

	private:
		Supervisor* supervisor;
		SystemTicker* ticker;

		float
			diff,
			speed,
			real_time,
			game_time;

		std::list<PeriodicTaskInfo>
			realtime_container,
			gametime_container;

		std::list<ExecutionThread*> execution_threads;

		unsigned int thread_count;
		bool paused;
	};

#endif // !EXECUTOR_H
