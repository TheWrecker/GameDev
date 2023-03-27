
#include "event_handler.h"

EventHandler::EventHandler()
	:paused(true)
{
}

EventHandler::~EventHandler()
{
}

void EventHandler::Update()
{
	//TODO: maybe move to main loop?
	if (!paused)
		ProcessEvents();
}

void EventHandler::Resume()
{
	paused = false;
}

void EventHandler::Pause()
{
	paused = true;
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
		_type_container->second.push_back(&func);
}

void EventHandler::RemoveCallback(EventType type, CallbackFunction func)
{
	auto _type_container = callbacks.find(type);
	if (_type_container != callbacks.end())
		_type_container->second.remove(&func);
}

void EventHandler::ProcessEvents(unsigned int count)
{

	unsigned int _i = 0;
	while ((!queue.empty()) && (_i < count))
	{
		auto _event = queue.front().get();
		_event->Fire();

		if (_event->HasCallback())
			_event->Callback();

		auto _type_container = callbacks.find(_event->GetType());
		if (_type_container != callbacks.end())
		{
			for (auto& func : _type_container->second)
				(*func)();
		}

		queue.pop();
		_i++;
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
