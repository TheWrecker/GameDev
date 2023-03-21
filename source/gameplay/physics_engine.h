
#ifndef PHYSICS_ENGINE
	#define PHYSICS_ENGINE

	#include <chrono>
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

		float GetLastPhysicsTickDuration();

		std::chrono::high_resolution_clock::time_point _tp1, _tp2;
		float last_tick;

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
