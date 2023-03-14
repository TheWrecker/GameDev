
#include "block_digged.h"
#include "..\entities\player.h"
#include "..\entities\segment.h"
#include "..\entities\world.h"
#include "event_handler.h"

#include "finish_dig.h"

FinishDigEvent::FinishDigEvent(EventHandler* handler, Player* player, SolidBlock* block)
	:IEvent(EventType::FINISH_DIG, handler), player(player), block(block)
{
}

void FinishDigEvent::Fire()
{
    auto _pos = block->Position();
    auto _type = block->GetBlockType();
    auto _segment = player->GetWorld()->GetSegment(_pos.x, _pos.y, _pos.z);
    auto _block_index = player->GetWorld()->GetBlockIndex(_pos.x, _pos.y, _pos.z);
    _segment->RemoveBlock(_block_index.x, _block_index.y, _block_index.z);
    _segment->RebuildBuffers();

    auto _event = new BlockDiggedEvent(parent, player, _type, _pos);
    parent->FeedEvent(_event);
}
