
#ifndef BEGIN_PLACEMENT_H
	#define BEGIN_PLACEMENT_H

	#include <DirectXMath.h>

	#include "interface_event.h"

	class SolidBlock;
	class Player;

	class BeginPlacementEvent : public IEvent
	{
	public:
		BeginPlacementEvent(EventHandler* handler, Player* player, DirectX::XMINT3 pos);

		void Fire() override;

	private:
		Player* player;
		DirectX::XMINT3 position;
	};

#endif // !BEGIN_PLACEMENT_H
