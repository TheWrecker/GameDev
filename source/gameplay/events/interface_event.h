
#pragma once

enum class Event
{
	UNDEFINED,
	BEGIN_DIG,
	BLOCK_DIGGED
};

class IEvent
{
public:
	IEvent(Event event)
		:type(event)
	{}
	virtual ~IEvent() {}

	virtual void Fire() = 0;

	Event GetType()
	{
		return type;
	}

private:
	Event type;
};
