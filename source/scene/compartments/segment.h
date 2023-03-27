
#ifndef SEGMENT_H
	#define SEGMENT_H

	#include <memory>
	#include <DirectXMath.h>

	#include "defs_pipeline.h"
	#include "defs_world.h"
	#include "../visuals/elements/buffer_vertex.h"
	#include "../entities/block_solid.h"

	class IndexBuffer;
	class Scene;

	struct SegmentIndices
	{
		unsigned int x, y, z;
	};

	class Segment
	{
	public:
		Segment(Scene* scene, SolidBlockType type = SolidBlockType::TEST, bool fill = false, float x = 0.0f, float y = 0.0f, float z = 0.0f);
		~Segment();

		void SetType(SolidBlockType type);
		void Move(float x, float y, float z);
		void AddBlock(SolidBlock* target, unsigned int x, unsigned int y, unsigned int z);
		void AddBlock(SolidBlock* target);
		void AddBlock(SolidBlockType type, unsigned int x, unsigned int y, unsigned int z);
		void AddBlock(unsigned int x, unsigned int y, unsigned int z);
		void Fill(SolidBlockType type);
		void RemoveBlock(unsigned int x, unsigned int y, unsigned int z);
		void RebuildBuffers();

		DirectX::XMMATRIX World_Matrix();
		DirectX::XMFLOAT3 Position();
		SegmentIndices GetArrayIndices(unsigned int value);
		VertexBuffer<SolidBlockVertex>* GetVertexBuffer();
		IndexBuffer* GetIndexBuffer();
		bool IsEmpty();

		SolidBlock* blocks[SEGMENT_DIMENSION_SIZE][SEGMENT_DIMENSION_SIZE][SEGMENT_DIMENSION_SIZE];

	private:
		friend class SolidBlockProcessor;
		Scene* scene;

		SolidBlockType default_type;
		unsigned int block_count;
		DirectX::XMFLOAT3 position;

		std::unique_ptr<VertexBuffer<SolidBlockVertex>> vertex_buffer;
		std::unique_ptr<IndexBuffer> index_buffer;
	};

#endif // !SEGMENT_H
