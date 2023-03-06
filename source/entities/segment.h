
#pragma once

#include <array>
#include <memory>

#include "defs_pipeline.h"
#include "../visuals/elements/buffer_vertex.h"
#include "../visuals/elements/buffer_index.h"
#include "block_solid.h"
#include "entity_basic.h"

constexpr unsigned int SEGMENT_DIMENSION_SIZE = 5;
constexpr unsigned int SEGMENT_ARRAY_SIZE = SEGMENT_DIMENSION_SIZE * SEGMENT_DIMENSION_SIZE * SEGMENT_DIMENSION_SIZE;
constexpr float SOLID_BLOCK_SIZE = 1.0f;

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

	void Move(float x, float y, float z);
	void AddBlock(SolidBlock* target, unsigned int x, unsigned int y, unsigned int z);
	void AddBlock(SolidBlock* target, unsigned int index = 0);
	void AddBlock(SolidBlock* target);
	void AddBlock(SolidBlockType type, unsigned int x, unsigned int y, unsigned int z);
	void AddBlock(SolidBlockType type = SolidBlockType::TEST, unsigned int index = 0);
	void Fill(SolidBlockType type);
	void RemoveBlock(unsigned int x, unsigned int y, unsigned int z);
	void RemoveBlock(unsigned int index = 0);
	void RebuildBuffers();

	SegmentIndices GetArrayIndices(unsigned int value);
	VertexBuffer<SolidBlockVertex>* GetVertexBuffer();
	IndexBuffer* GetIndexBuffer();

private:
	friend class SolidBlockProcessor;

	Scene* scene;
	SolidBlockType default_type;
	SolidBlock* blocks[SEGMENT_DIMENSION_SIZE][SEGMENT_DIMENSION_SIZE][SEGMENT_DIMENSION_SIZE];
	std::unique_ptr<VertexBuffer<SolidBlockVertex>> vertex_buffer;
	std::unique_ptr<IndexBuffer> index_buffer;
};
