
#ifndef FINISH_DIG_H
	#define FINISH_DIG_H

	#include "interface_event.h"

	class SolidBlock;
	class Player;

	class FinishDigEvent : public IEvent
	{
	public:
		FinishDigEvent(EventHandler* handler ,Player* player, SolidBlock* block);

		void Fire() override;

	private:
		Player* player;
		SolidBlock* block;
	};

#endif // !FINISH_DIG_H
