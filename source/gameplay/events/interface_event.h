
#pragma once

#include <functional>

enum class EventType
{
	UNDEFINED,
	BEGIN_DIG,
	BLOCK_DIGGED
};

typedef std::function<void()> CallbackFunction;

class IEvent
{
public:
	IEvent(EventType event)
		:type(event)
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

private:
	EventType type;
	CallbackFunction callback;
};
