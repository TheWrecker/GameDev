
#ifndef PROCESSOR_SOLID_BLOCK_H
	#define PROCESSOR_SOLID_BLOCK_H

	#include <vector>

	#include "defs_pipeline.h"
	#include "../entities/block_solid.h"

	typedef const std::vector<SolidBlockVertex> Face;

	class TextureAtlas;
	class Segment;
	class World;

	class SolidBlockProcessor
	{
	public:
		static void Setup(World* world, TextureAtlas* atlas);
		static void Rebuild(Segment* target);

	private:
		static bool CheckNextSegmentBlock(SolidBlock* block, FaceName face);
		static bool CheckBlockFace(Segment* segment, SolidBlock* block, FaceName face);
		static void AddFaceVertices(Segment* target, Face& face);
		static unsigned int
			index_x,
			index_y,
			index_z,
			solids,
			current_index;
		static TextureAtlas* texture_atlas;
		static World* world;
	};

#endif // !PROCESSOR_SOLID_BLOCK_H
