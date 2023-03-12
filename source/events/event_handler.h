
#pragma once

#include <functional>
#include <list>
#include <memory>
#include <queue>
#include <unordered_map>

#include "interface_service.h"
#include "interface_event.h"

constexpr unsigned int EVENT_PROCESS_COUNT_DEFAULT = 10000;

class Supervisor;

class EventHandler : public IService
{
public:
	EventHandler(Supervisor* supervisor);
	~EventHandler();
	void Update() override;

	void FeedEvent(IEvent* event);
	void RegisterCallback(EventType type, CallbackFunction func);
	void RemoveCallback(EventType type, CallbackFunction func);

	void ProcessEvents(unsigned int count = EVENT_PROCESS_COUNT_DEFAULT);
	void PopCurrentEvent();
	void ClearEvents();

private:
	std::queue<std::unique_ptr<IEvent>> queue;
	std::unordered_map < EventType, std::list<CallbackFunction*>> callbacks;
};
