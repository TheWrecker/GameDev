
#ifndef PHYSICS_ENGINE
	#define PHYSICS_ENGINE

	#include <list>

	#include "interface_service.h"

	class Supervisor;

	class PhysicsEngine : public IService
	{
	public:
		PhysicsEngine(Supervisor* supervisor);
		~PhysicsEngine();

		void Update() override;

	private:
		Supervisor* supervisor;
	};


#endif // !PHYSICS_ENGINE
