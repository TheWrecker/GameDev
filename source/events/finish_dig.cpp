
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
    auto _segment = player->GetWorld()->GetSegment(_pos.x, _pos.z);
    if (!_segment)
        assert(false);
    unsigned int _x = static_cast<unsigned int>(fmod(_pos.x, SEGMENT_LENGTH));
    unsigned int _y = static_cast<unsigned int>(fmod(_pos.y, SEGMENT_LENGTH));
    unsigned int _z = static_cast<unsigned int>(fmod(_pos.z, SEGMENT_LENGTH));
    _segment->RemoveBlock(_x, _y, _z);
    _segment->RebuildBuffers();

    auto _event = new BlockDiggedEvent(parent, player, _type, _pos);
    parent->FeedEvent(_event);
}