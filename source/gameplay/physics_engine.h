
#ifndef PHYSICS_ENGINE
	#define PHYSICS_ENGINE

	#include <list>

	#include "interface_service.h"
	#include "game_time.h"

	class MovePhysics;
	class SystemTicker;
	class Supervisor;

	class PhysicsEngine : public IService
	{
	public:
		PhysicsEngine(Supervisor* supervisor);
		~PhysicsEngine();

		void Update() override;

		void RegisterMovementComponent(MovePhysics* target);
		void RemoveMovementComponent(MovePhysics* target);

	private:
		void UpdateAllSystems();

		Supervisor* supervisor;
		SystemTicker* ticker;
		PeriodicTask update_task;

		std::list<MovePhysics*> movements;
	};


#endif // !PHYSICS_ENGINE
