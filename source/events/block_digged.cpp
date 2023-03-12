
#include "block_digged.h"
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
    //TODO: add functionality
    //do nothing for now, maybe add effect? or handle inventory changes?
}
