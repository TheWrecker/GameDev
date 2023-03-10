
#pragma once

#include "interface_event.h"

class Block;
class Player;
class World;

class BeginDigEvent : public IEvent
{
public:
	BeginDigEvent(World* world, Player* player, Block* block);

	void Fire() override;

private:
	Player* player;
	Block* block;
};