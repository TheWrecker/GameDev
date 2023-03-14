
#ifndef PROCESSOR_SOLID_BLOCK_H
	#define PROCESSOR_SOLID_BLOCK_H

	#include <vector>

	#include "defs_pipeline.h"
	#include "../entities/block_solid.h"

	typedef const std::vector<SolidBlockVertex> Face;

	class TextureAtlas;
	class Segment;

	class SolidBlockProcessor
	{
	public:
		static void Setup(TextureAtlas* atlas);
		static void Rebuild(Segment* target);

	private:
		static bool CheckBlockFace(Segment* target, FaceName face);
		static void AddFaceVertices(Segment* target, Face& face);
		static unsigned int
			index_x,
			index_y,
			index_z,
			solids,
			current_index;
		static TextureAtlas* texture_atlas;
	};

#endif // !PROCESSOR_SOLID_BLOCK_H
