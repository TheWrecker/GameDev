
#ifndef SEGMENT_H
	#define SEGMENT_H

	#include <memory>
	#include <DirectXMath.h>

	#include "defs_pipeline.h"
	#include "defs_world.h"
	#include "defs_blocks.h"
	#include "../visuals/elements/buffer_vertex.h"

	class IndexBuffer;
	class Scene;

	class Segment
	{
	public:
		Segment(Scene* scene, BlockType type = BlockType::TEST, bool fill = false, float x = 0.0f, float y = 0.0f, float z = 0.0f);
		~Segment();

		void SetBlock(unsigned int x, unsigned int y, unsigned int z, BlockType type = BlockType::TEST);
		void RemoveBlock(unsigned int x, unsigned int y, unsigned int z);

		void SetType(BlockType type);
		void Fill(BlockType type);
		void Move(float x, float y, float z);
		void RebuildBuffers();

		const DirectX::XMMATRIX World_Matrix();
		const DirectX::XMFLOAT3& Position();
		VertexBuffer<SolidBlockVertex>* GetVertexBuffer();
		IndexBuffer* GetIndexBuffer();
		bool IsEmpty();

		BlockType blocks[SEGMENT_DIMENSION_SIZE][SEGMENT_DIMENSION_SIZE][SEGMENT_DIMENSION_SIZE];
		bool biome_processed;

	private:
		friend class SolidBlockProcessor;
		Scene* scene;

		BlockType default_type;
		unsigned int block_count;
		const DirectX::XMFLOAT3 position;

		std::unique_ptr<VertexBuffer<SolidBlockVertex>> vertex_buffer;
		std::unique_ptr<IndexBuffer> index_buffer;
	};

#endif // !SEGMENT_H
