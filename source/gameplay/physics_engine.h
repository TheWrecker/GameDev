
#ifndef PHYSICS_ENGINE
	#define PHYSICS_ENGINE

	#include <chrono>
	#include <list>

	#include "interface_service.h"
	#include "../core/executor.h"

	class Player;
	class Scene;
	class World;
	class MovePhysics;
	class CollisionPhysics;
	class SystemTicker;
	class Supervisor;

	class PhysicsEngine : public IService
	{
	public:
		PhysicsEngine();
		~PhysicsEngine();

		bool Initialize() override;
		void Update() override;
		void Start();

		void RegisterMovementComponent(MovePhysics* target);
		void RemoveMovementComponent(MovePhysics* target);
		void RegisterCollisionComponent(CollisionPhysics* target);
		void RemoveCollisionComponent(CollisionPhysics* target);

	private:
		friend class Overlay;

		void UpdateAllSystems();
		void ProcessPlayerNoPhysics();

		SystemTicker* ticker;
		Executor* executor;
		World* world;
		Scene* scene;
		Player* player;

		bool enabled;
		PeriodicTask update_task;

		std::list<MovePhysics*> movements;
		std::list<CollisionPhysics*> collisions;
	};


#endif // !PHYSICS_ENGINE
