
#ifndef SEGMENT_H
	#define SEGMENT_H

	#include <atomic>
	#include <mutex>
	#include <memory>
	#include <DirectXMath.h>

	#include "defs_pipeline.h"
	#include "defs_world.h"
	#include "defs_blocks.h"
	#include "../visuals/elements/buffer_vertex.h"

	class IndexBuffer;
	class Sector;
	class Scene;

	class Segment
	{
	public:
		Segment(Scene* scene, BlockType type = BlockType::TEST, bool fill = false, int x = 0, int y = 0, int z = 0);
		~Segment();

		void SetBlock(unsigned int x, unsigned int y, unsigned int z, BlockType type = BlockType::TEST);
		void RemoveBlock(unsigned int x, unsigned int y, unsigned int z);

		void SetType(BlockType type);
		void Fill(BlockType type);
		void Move(int x, int y, int z);
		void RebuildBuffers();

		const DirectX::XMMATRIX World_Matrix();
		const DirectX::XMINT3& Position();
		VertexBuffer<SolidBlockVertex>* GetVertexBuffer();
		IndexBuffer* GetIndexBuffer();
		bool IsEmpty();

		BlockType blocks[SEGMENT_DIMENSION_SIZE][SEGMENT_DIMENSION_SIZE][SEGMENT_DIMENSION_SIZE];

	private:
		friend class SolidBlockProcessor;
		friend class WorldEngine;
		friend class SolidBlockRender;
		friend class GarbageCollector;

		Scene* scene;

		BlockType default_type;
		DirectX::XMINT3 position;

		std::mutex draw_mutex;
		std::atomic<bool> mesh_rebuilt;
		std::atomic<bool> biome_processed;
		std::atomic<unsigned int> block_count;

		std::atomic<VertexBuffer<SolidBlockVertex>*> vertex_buffer;
		std::atomic<IndexBuffer*> index_buffer;
	};

#endif // !SEGMENT_H
