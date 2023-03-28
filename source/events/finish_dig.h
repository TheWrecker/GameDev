
#ifndef FINISH_DIG_H
	#define FINISH_DIG_H

	#include "interface_event.h"
	#include "defs_blocks.h"

	class Player;

	class FinishDigEvent : public IEvent
	{
	public:
		FinishDigEvent(EventHandler* handler ,Player* player, DirectX::XMFLOAT3& position, BlockType blockType);

		void Fire() override;

	private:
		Player* player;
		DirectX::XMFLOAT3 position;
		BlockType block_type;
	};

#endif // !FINISH_DIG_H
