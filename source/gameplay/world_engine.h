
#ifndef WORLD_ENGINE_H
	#define WORLD_ENGINE_H

	#include <memory>
	#include <vector>

	#include <interface_service.h>
	#include "../core/executor.h"

	class Player;
	class Sector;
	class World;
	class Scene;
	class FastNoiseSIMD;

	class WorldEngine : public IService
	{
	public:
		WorldEngine(int seed = 0);
		~WorldEngine();

		void Update() override;
		bool Initialize() override;
		void SetupStartingWorld();
		void StartWorldGeneration();
		void StopWorldGeneration();

		void SetVicinityRange(unsigned int value);

		std::vector<Sector*> near_sectors;

	private:
		friend Scene;

		void LoadWorld(float x, float z);
		void WorldLoadTick();

		void RebuildSector(Sector* target);

		Executor* executor;
		Scene* scene;
		World* world;
		Player* player;

		bool enabled;
		PeriodicTask update_task;
		ExecutionThread* exec_task;
		Sector* last_sector;
		int seed;
		int range;

		std::unique_ptr<FastNoiseSIMD> noise_generator;
		float* heightmap;
	};

#endif // !WORLD_ENGINE_H
