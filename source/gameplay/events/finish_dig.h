#pragma once

#include "interface_event.h"

class Block;
class Player;

class FinishDigEvent : public IEvent
{
public:
	FinishDigEvent(Player* player, Block* block);

	void Fire() override;

private:
	Player* player;
	Block* block;
};