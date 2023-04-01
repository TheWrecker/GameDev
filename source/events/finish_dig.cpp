
#include "defs_world.h"
#include "../entities/block_solid.h"
#include "block_digged.h"
#include "..\entities\player.h"
#include "..\scene\compartments\segment.h"
#include "..\scene\world.h"
#include "event_handler.h"

#include "finish_dig.h"

FinishDigEvent::FinishDigEvent(EventHandler* handler, Player* player, DirectX::XMFLOAT3& position, BlockType blockType)
	:IEvent(EventType::FINISH_DIG, handler), player(player), block_type(blockType), position(position)
{
}

void FinishDigEvent::Fire()
{
    auto _segment = player->GetWorld()->GetSegment(position.x, position.y, position.z);
    auto _block_index = GetBlockIndex(position.x, position.y, position.z);
    auto _segment_index = GetSegmentIndex(position.x, position.y, position.z);
    auto _sector = player->GetWorld()->GetSector(position.x, position.z);
    _segment->RemoveBlock(_block_index.x, _block_index.y, _block_index.z);
    _segment->RebuildBuffers();

    auto _event = new BlockDiggedEvent(parent, player, block_type, position);
    parent->FeedEvent(_event);
}
