
#ifndef PROCESSOR_SOLID_BLOCK_H
	#define PROCESSOR_SOLID_BLOCK_H

	#include <vector>
	#include <DirectXMath.h>

	#include "defs_pipeline.h"
	#include "defs_blocks.h"
	#include "defs_world.h"

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
		static void Rebuild(Sector* sector,Segment* target, SegmentIndex index);

	private:
		static bool CheckNextSegmentBlock(Sector* sector, SegmentIndex& index, FaceName face);
		static bool CheckBlockFace(Sector* sector, Segment* segment, SegmentIndex& index, FaceName face);
		static void AddFaceVertices(Segment* target, Face& face);

		static TextureAtlas* texture_atlas;
		static World* world;

		static thread_local unsigned int
			index_x,
			index_y,
			index_z,
			solids,
			current_index;

		static thread_local DirectX::XMFLOAT3 position;
	};

#endif // !PROCESSOR_SOLID_BLOCK_H
