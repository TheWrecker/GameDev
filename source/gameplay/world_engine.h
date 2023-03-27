
#ifndef WORLD_ENGINE_H
	#define WORLD_ENGINE_H

	#include <memory>
	#include <vector>

	#include <interface_service.h>

	class Player;
	class Pillar;
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

	private:
		void LoadWorld(float x, float z);

		Scene* scene;
		World* world;
		Player* player;

		Pillar* last_pillar;
		int seed;

		std::unique_ptr<FastNoiseSIMD> noise_generator;
		float* heightmap;
	};

#endif // !WORLD_ENGINE_H
