
#include "../gameplay/item_container.h"
#include "..\entities\player.h"
#include "..\entities\segment.h"
#include "..\entities\world.h"

#include "block_digged.h"

BlockDiggedEvent::BlockDiggedEvent(EventHandler* handler, Player* player, SolidBlockType type, DirectX::XMFLOAT3 position)
    :IEvent(EventType::BLOCK_DIGGED, handler), player(player), type(type), position(position)
{
}

void BlockDiggedEvent::Fire()
{
    ItemType _type = ItemType::UNDEFINED;
	switch (type)
	{
		case SolidBlockType::TEST:
		{
			_type = ItemType::TEST_BLOCK;
			break;
		}
		case SolidBlockType::DIRT:
		{
			_type = ItemType::DIRT_BLOCK;
			break;
		}
		case SolidBlockType::GRASS:
		{
			_type = ItemType::GRASS_BLOCK;
			break;
		}
		default:
			break;
	}
	if (_type == ItemType::UNDEFINED)
		return;

	player->GetInventory()->AddItem(_type, 1);

	//TODO: emit item add event?
}
