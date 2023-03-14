
#ifndef SEGMENT_H
	#define SEGMENT_H

	#include <array>
	#include <memory>

	#include "defs_pipeline.h"
	#include "defs_world.h"
	#include "../visuals/elements/buffer_vertex.h"
	#include "../visuals/elements/buffer_index.h"
	#include "block_solid.h"
	#include "entity_basic.h"

	class IndexBuffer;
	class Scene;

	struct SegmentIndices
	{
		unsigned int x, y, z;
	};

	class Segment : public BasicEntity
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

		SegmentIndices GetArrayIndices(unsigned int value);
		VertexBuffer<SolidBlockVertex>* GetVertexBuffer();
		IndexBuffer* GetIndexBuffer();
		bool IsEmpty();

	private:
		friend class SolidBlockProcessor;
		friend class World;

		Scene* scene;
		SolidBlockType default_type;
		SolidBlock* blocks[SEGMENT_DIMENSION_SIZE][SEGMENT_DIMENSION_SIZE][SEGMENT_DIMENSION_SIZE];
		std::unique_ptr<VertexBuffer<SolidBlockVertex>> vertex_buffer;
		std::unique_ptr<IndexBuffer> index_buffer;
		unsigned int block_count;
	};

#endif // !SEGMENT_H
