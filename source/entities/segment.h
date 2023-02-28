
#pragma once

#include <array>
#include <d3d11.h>
#include <memory>
#include <DirectXMath.h>

#include "../visuals/elements/buffer_vertex.h"
#include "block_solid.h"
#include "entity_basic.h"

constexpr unsigned int SEGMENT_DIMENSION_SIZE = 5;
constexpr unsigned int SEGMENT_ARRAY_SIZE = SEGMENT_DIMENSION_SIZE * SEGMENT_DIMENSION_SIZE * SEGMENT_DIMENSION_SIZE;
constexpr float SOLID_BLOCK_SIZE = 1.0f;

class IndexBuffer;

struct SegmentIndices
{
	unsigned int x, y, z;
};

class Segment : public BasicEntity
{
public:
	Segment(SolidBlockType type = SolidBlockType::TEST, bool fill = false);
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
	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();

private:
	friend class SolidBlockProcessor;
	
	struct NormalVertexStruct
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 uv;
		DirectX::XMFLOAT3 normal;
	};

	SolidBlockType default_type;
	SolidBlock* blocks[SEGMENT_DIMENSION_SIZE][SEGMENT_DIMENSION_SIZE][SEGMENT_DIMENSION_SIZE];
	std::unique_ptr<VertexBuffer<NormalVertexStruct>> vertex_buffer;
	std::unique_ptr<IndexBuffer> index_buffer;
};
