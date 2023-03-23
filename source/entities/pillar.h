
#ifndef PILLAR_H
	#define PILLAR_H

	#include <unordered_map>

	class Segment;
	class Scene;

	class Pillar
	{
	public:
		Pillar(Scene* scene,float x, float z);
		~Pillar();

		void AddSegment(Segment* target, int index);
		Segment* CreateSegment(int index);
		Segment* GetSegment(int index, bool force = false);

		float x, z;

	private:
		friend class World;
		friend class WorldEngine;
		friend class SolidBlockRender;

		Scene* scene;

		std::unordered_map<int, Segment*> segments;
	};

#endif // !PILLAR_H
