
#ifndef WORLD_ENGINE_H
	#define WORLD_ENGINE_H

	#include <memory>
	#include <vector>

	#include <interface_service.h>
	#include "defs_world.h"

	class Player;
	class Pillar;
	class Segment;
	class World;
	class Scene;
	class FastNoise;

	class WorldEngine : public IService
	{
	public:
		WorldEngine(int seed = 0);
		~WorldEngine();

		void SetupWorld(Scene* scene);
		void BeginWorldLoading();
		void WorldLoadTick();

		std::unique_ptr<FastNoise> noise_generator;

	private:


		void LoadWorld(float x, float z);

		Scene* scene;
		World* world;
		Player* player;
		Pillar* last_pillar;
		int seed;

		std::vector<BiomeInfo> biomes;
	};

#endif // !WORLD_ENGINE_H

