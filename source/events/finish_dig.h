#pragma once

#include "interface_event.h"

class SolidBlock;
class Player;

class FinishDigEvent : public IEvent
{
public:
	FinishDigEvent(EventHandler* handler ,Player* player, SolidBlock* block);

	void Fire() override;

private:
	Player* player;
	SolidBlock* block;
};