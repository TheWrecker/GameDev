
#ifndef PHYSICS_ENGINE
	#define PHYSICS_ENGINE

	#include <list>

	#include "interface_service.h"
	#include "game_time.h"

	class Scene;
	class World;
	class MovePhysics;
	class CollisionPhysics;
	class GameTime;
	class SystemTicker;
	class Supervisor;

	class PhysicsEngine : public IService
	{
	public:
		PhysicsEngine(Supervisor* supervisor);
		~PhysicsEngine();

		void Start();
		void Update() override;

		void RegisterMovementComponent(MovePhysics* target);
		void RemoveMovementComponent(MovePhysics* target);
		void RegisterCollisionComponent(CollisionPhysics* target);
		void RemoveCollisionComponent(CollisionPhysics* target);

	private:
		void UpdateAllSystems();

		Supervisor* supervisor;
		SystemTicker* ticker;
		GameTime* game_time;
		PeriodicTask update_task;
		World* world;
		Scene* scene;

		std::list<MovePhysics*> movements;
		std::list<CollisionPhysics*> collisions;
	};


#endif // !PHYSICS_ENGINE
