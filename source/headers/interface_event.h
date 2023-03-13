
#ifndef IEVENT_H
	#define IEVENT_H

	#include <functional>

	enum class EventType
	{
		UNDEFINED,
		BEGIN_DIG,
		FINISH_DIG,
		BLOCK_DIGGED
	};

	typedef std::function<void()> CallbackFunction;

	class EventHandler;

	class IEvent
	{
	public:
		IEvent(EventType event, EventHandler* handler)
			:type(event), parent(handler)
		{}
		virtual ~IEvent() {}

		virtual void Fire() = 0;

		void Callback()
		{
			callback();
		}

		void AddCallback(CallbackFunction func)
		{
			callback = func;
		}

		bool HasCallback()
		{
			return static_cast<bool>(callback);
		}

		EventType GetType()
		{
			return type;
		}

	protected:
		EventType type;
		CallbackFunction callback;
		EventHandler* parent;
	};

#endif // !IEVENT_H
