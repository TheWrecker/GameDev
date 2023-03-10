
#pragma once

#include <DirectXMath.h>

#include "interface_event.h"
#include "../entities/block_solid.h"

class Block;
class Player;

class BlockDiggedEvent : public IEvent
{
public:
	BlockDiggedEvent(Player* player, SolidBlockType type, DirectX::XMFLOAT3 position);

	void Fire() override;

private:
	Player* player;
	SolidBlockType type;
};