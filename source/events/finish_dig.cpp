
#include "defs_world.h"
#include "../entities/block_solid.h"
#include "block_digged.h"
#include "..\entities\player.h"
#include "..\scene\compartments\segment.h"
#include "../scene/compartments/sector.h"
#include "..\scene\world.h"
#include "event_handler.h"
#include "../processors/processor_solid_block.h"

#include "finish_dig.h"

FinishDigEvent::FinishDigEvent(EventHandler* handler, Player* player, DirectX::XMINT3& position, BlockType blockType)
	:IEvent(EventType::FINISH_DIG, handler), player(player), block_type(blockType), position(position)
{
}

void FinishDigEvent::Fire()
{
    auto _segment = player->GetWorld()->GetSegmentByGridPos(position.x, position.y, position.z);
    auto _block_index = GetBlockIndex(position.x, position.y, position.z);
    auto _segment_index = GetSegmentIndex(position.x, position.y, position.z);
    auto _sector = player->GetWorld()->GetSectorByGridPos(position.x, position.z);

    _segment->RemoveBlock(_block_index.x, _block_index.y, _block_index.z);

    Sector* _adjacent_sector = nullptr;
    Segment* _adjacent_segment = nullptr;
    SegmentIndex _new_segment_index = {};

    //test
    if ((_segment_index.x == 0) && (_block_index.x == 0))
    {
        _adjacent_sector = player->GetWorld()->GetSectorByGridPos(position.x - SEGMENT_LENGTH, position.z, true);
        _new_segment_index = { SECTOR_HORIZONTAL_SIZE - 1, _segment_index.y, _segment_index.z };
        _adjacent_segment = _adjacent_sector->segments[_new_segment_index.x][_new_segment_index.y][_new_segment_index.z].load();

        if (_adjacent_segment)
            SolidBlockProcessor::RebuildSegmentInSector(_adjacent_sector, _adjacent_segment, _new_segment_index);
    }
    else if ((_segment_index.x == SECTOR_HORIZONTAL_SIZE - 1) && (_block_index.x == SEGMENT_DIMENSION_SIZE - 1))
    {
        _adjacent_sector = player->GetWorld()->GetSectorByGridPos(position.x + SEGMENT_LENGTH, position.z, true);
        _new_segment_index = { 0, _segment_index.y, _segment_index.z };
        _adjacent_segment = _adjacent_sector->segments[_new_segment_index.x][_new_segment_index.y][_new_segment_index.z].load();

        if (_adjacent_segment)
            SolidBlockProcessor::RebuildSegmentInSector(_adjacent_sector, _adjacent_segment, _new_segment_index);
    }
    else if ((_block_index.x == 0) || (_block_index.x == SEGMENT_DIMENSION_SIZE - 1))
    {
        _new_segment_index = { _segment_index.x , _segment_index.y, _segment_index.z };

        if (_block_index.x == 0)
            _new_segment_index.x--;
        else if (_block_index.x == SEGMENT_DIMENSION_SIZE - 1)
            _new_segment_index.x++;

        _adjacent_segment = _sector->segments[_new_segment_index.x][_new_segment_index.y][_new_segment_index.z].load();

        if (_adjacent_segment)
            SolidBlockProcessor::RebuildSegmentInSector(_sector, _adjacent_segment, _new_segment_index);
    }

    if ((_segment_index.y == 0) && (_block_index.y == 0))
    {
        //we shouldnt dig the base layer, add it back
        _segment->SetBlock(_block_index.x, _block_index.y, _block_index.z, block_type);
        SolidBlockProcessor::RebuildSegmentInSector(_sector, _segment, _segment_index);
        //return to prevent emiting further associated events
        return;
    }
    else if ((_segment_index.y == SECTOR_VERTICAL_SIZE - 1) && (_block_index.y == SEGMENT_DIMENSION_SIZE - 1))
    {
            //do nothing
    }
    else if ((_block_index.y == 0) || (_block_index.y == SEGMENT_DIMENSION_SIZE - 1))
    {
        _new_segment_index = { _segment_index.x , _segment_index.y, _segment_index.z };

        if (_block_index.y == 0)
            _new_segment_index.y--;
        else if (_block_index.y == SEGMENT_DIMENSION_SIZE - 1)
            _new_segment_index.y++;

        _adjacent_segment = _sector->segments[_new_segment_index.x][_new_segment_index.y][_new_segment_index.z].load();

        if (_adjacent_segment)
            SolidBlockProcessor::RebuildSegmentInSector(_sector, _adjacent_segment, _new_segment_index);
    }

    if ((_segment_index.z == 0) && (_block_index.z == 0))
    {
        _adjacent_sector = player->GetWorld()->GetSectorByGridPos(position.x, position.z - SEGMENT_LENGTH, true);
        _new_segment_index = { _segment_index.x, _segment_index.y, SECTOR_HORIZONTAL_SIZE - 1 };
        _adjacent_segment = _adjacent_sector->segments[_new_segment_index.x][_new_segment_index.y][_new_segment_index.z].load();

        if (_adjacent_segment)
            SolidBlockProcessor::RebuildSegmentInSector(_adjacent_sector, _adjacent_segment, _new_segment_index);
    }
    else if ((_segment_index.z == SECTOR_HORIZONTAL_SIZE - 1) && (_block_index.z == SEGMENT_DIMENSION_SIZE - 1))
    {
        _adjacent_sector = player->GetWorld()->GetSectorByGridPos(position.x, position.z + SEGMENT_LENGTH, true);
        _new_segment_index = { _segment_index.x, _segment_index.y, 0 };
        _adjacent_segment = _adjacent_sector->segments[_new_segment_index.x][_new_segment_index.y][_new_segment_index.z].load();

        if (_adjacent_segment)
            SolidBlockProcessor::RebuildSegmentInSector(_adjacent_sector, _adjacent_segment, _new_segment_index);
    }
    else if ((_block_index.z == 0) || (_block_index.z == SEGMENT_DIMENSION_SIZE - 1))
    {
        _new_segment_index = { _segment_index.x , _segment_index.y, _segment_index.z };

        if (_block_index.z == 0)
            _new_segment_index.z--;
        else if (_block_index.z == SEGMENT_DIMENSION_SIZE - 1)
            _new_segment_index.z++;

        _adjacent_segment = _sector->segments[_new_segment_index.x][_new_segment_index.y][_new_segment_index.z].load();

        if (_adjacent_segment)
            SolidBlockProcessor::RebuildSegmentInSector(_sector, _adjacent_segment, _new_segment_index);
    }

    SolidBlockProcessor::RebuildSegmentInSector(_sector, _segment, _segment_index);

    auto _event = new BlockDiggedEvent(parent, player, block_type, position);
    parent->FeedEvent(_event);
}
