
#ifndef WORLD_ENGINE_H
	#define WORLD_ENGINE_H

	#include <memory>
	#include <vector>

	#include <interface_service.h>

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

		bool Initialize() override;
		void SetupStartingWorld();
		void BeginWorldGeneration();
		void WorldLoadTick();

		void SetVicinityRange(unsigned int value);

		std::vector<Sector*> near_sectors;

	private:
		void LoadWorld(float x, float z);

		Scene* scene;
		World* world;
		Player* player;

		Sector* last_sector;
		int seed;
		int range;

		std::unique_ptr<FastNoiseSIMD> noise_generator;
		float* heightmap;
	};

#endif // !WORLD_ENGINE_H
