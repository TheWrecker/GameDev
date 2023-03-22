
#ifndef PHYSICS_ENGINE
	#define PHYSICS_ENGINE

	#include <chrono>
	#include <list>

	#include "interface_service.h"
	#include "game_time.h"

	class Player;
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
		friend class Overlay;

		void UpdateAllSystems();
		void ProcessPlayerNoPhysics();

		Supervisor* supervisor;
		SystemTicker* ticker;
		GameTime* game_time;
		PeriodicTask update_task;
		World* world;
		Scene* scene;
		Player* player;

		bool enabled;

		std::list<MovePhysics*> movements;
		std::list<CollisionPhysics*> collisions;
	};


#endif // !PHYSICS_ENGINE
