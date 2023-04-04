
#ifndef PROCESSOR_SOLID_BLOCK_H
	#define PROCESSOR_SOLID_BLOCK_H

	#include <vector>
	#include <DirectXMath.h>

	#include "defs_pipeline.h"
	#include "defs_blocks.h"
	#include "defs_world.h"
	#include "../visuals/elements/buffer_vertex.h"

	typedef const std::vector<SolidBlockVertex> Face;

	class SolidBlock;
	class TextureAtlas;
	class Sector;
	class Segment;
	class World;

	class SolidBlockProcessor
	{
	public:
		static bool Setup();

		static void RebuildSegmentSingle(Segment* target);
		static void RebuildSegmentInSector(Sector* sector,Segment* target, SegmentIndex index,
			VertexBuffer<SolidBlockVertex>* vbuffer, IndexBuffer* ibuffer);

	private:
		static bool CheckNextSegmentBlock(Sector* sector, SegmentIndex& index, FaceName face);
		static bool CheckBlockFaceInSector(Sector* sector, Segment* segment, SegmentIndex& index, FaceName face);
		static bool CheckBlockFaceSingle(Segment* segment, FaceName face);
		static void AddFaceVertices(Segment* target, Face& face);
		static void AddFaceVerticesCustomBuffers(Segment* target, Face& face, VertexBuffer<SolidBlockVertex>* vbuffer, IndexBuffer* ibuffer);

		static TextureAtlas* texture_atlas;
		static World* world;

		static thread_local unsigned int
			index_x,
			index_y,
			index_z,
			current_index;
	};

#endif // !PROCESSOR_SOLID_BLOCK_H
