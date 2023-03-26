
#ifndef PROCESSOR_SOLID_BLOCK_H
	#define PROCESSOR_SOLID_BLOCK_H

	#include <vector>

	#include "defs_pipeline.h"
	#include "defs_blocks.h"

	typedef const std::vector<SolidBlockVertex> Face;

	class SolidBlock;
	class TextureAtlas;
	class Segment;
	class World;

	class SolidBlockProcessor
	{
	public:
		static bool Setup();
		static void Rebuild(Segment* target);

	private:
		static bool CheckNextSegmentBlock(SolidBlock* block, FaceName face);
		static bool CheckBlockFace(Segment* segment, SolidBlock* block, FaceName face);
		static void AddFaceVertices(Segment* target, Face& face);

		static TextureAtlas* texture_atlas;
		static World* world;

		static unsigned int
			index_x,
			index_y,
			index_z,
			solids,
			current_index;
	};

#endif // !PROCESSOR_SOLID_BLOCK_H
