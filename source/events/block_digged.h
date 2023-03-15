
#ifndef BLOCK_DIGGED_H
	#define BLOCK_DIGGED_H

	#include <DirectXMath.h>

	#include "interface_event.h"
	#include "defs_blocks.h"

	class Player;

	class BlockDiggedEvent : public IEvent
	{
	public:
		BlockDiggedEvent(EventHandler* handler, Player* player, SolidBlockType type, DirectX::XMFLOAT3 position);

		void Fire() override;

	private:
		Player* player;
		SolidBlockType type;
		DirectX::XMFLOAT3 position;
	};

#endif // !BLOCK_DIGGED_H
