
#include "../utils/conversion_item_block.h"
#include "../gameplay/items/item_container.h"
#include "../entities/player.h"
#include "../scene/world.h"

#include "begin_placement.h"

BeginPlacementEvent::BeginPlacementEvent(EventHandler* handler, Player* player, DirectX::XMFLOAT3 pos)
	:IEvent(EventType::FINISH_DIG, handler), player(player), position(pos)
{
}

void BeginPlacementEvent::Fire()
{
	auto _item = player->GetInventory()->GetItemAtSlot(player->GetActiveInventorySlot());

	if (_item->IsEmpty())
		return;

	auto _type = ItemTypeToBlockType(_item->GetType());

	if (_type == SolidBlockType::UNDEFINED)
		return;

	auto _block = player->GetWorld()->CreateBlock(_type, position.x, position.y, position.z, true);

	_item->RemoveFromStack();
}
