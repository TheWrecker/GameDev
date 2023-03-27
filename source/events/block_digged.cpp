
#include "..\utils\conversion_item_block.h"
#include "..\gameplay\items\item_container.h"
#include "..\entities\player.h"

#include "block_digged.h"

BlockDiggedEvent::BlockDiggedEvent(EventHandler* handler, Player* player, SolidBlockType type, DirectX::XMFLOAT3 position)
    :IEvent(EventType::BLOCK_DIGGED, handler), player(player), type(type), position(position)
{
}

void BlockDiggedEvent::Fire()
{
	auto _type = BlockTypeToItemType(type);
	player->GetInventory()->AddItem(_type, 1);

	//TODO: emit item add event?
}
