
#pragma once

#include "interface_event.h"

class Block;
class Player;
class World;

class CancelDigEvent : public IEvent
{
public:
	CancelDigEvent(World* world, Player* player, Block* block);

	void Fire() override;

private:
	Player* player;
	Block* block;
};