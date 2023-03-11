
#include "event_handler.h"

EventHandler::EventHandler(Supervisor* supervisor)
{
}

EventHandler::~EventHandler()
{
}

void EventHandler::Update()
{
	ProcessEvents();
}

void EventHandler::FeedEvent(IEvent* event)
{
	auto _event = std::unique_ptr<IEvent>(event);
	queue.push(std::move(_event));
}

void EventHandler::RegisterCallback(EventType type, CallbackFunction func)
{
	auto _type_container = callbacks.find(type);
	if (_type_container != callbacks.end())
		_type_container->second.push_back(func);
}

void EventHandler::ProcessEvents(unsigned int count)
{
	if (queue.empty())
		return;

	unsigned int _i = 0;
	while (_i < count)
	{
		auto _event = queue.front().get();
		_event->Fire();

		if (_event->HasCallback())
			_event->Callback();

		auto _type_container = callbacks.find(_event->GetType());
		if (_type_container != callbacks.end())
		{
			for (auto& func : _type_container->second)
				func();
		}

		queue.pop();
	}
}

void EventHandler::PopCurrentEvent()
{
	if (!queue.empty())
		queue.pop();
}

void EventHandler::ClearEvents()
{
	while (!queue.empty())
		queue.pop();

}
