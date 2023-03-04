
#include <DirectXMath.h>

#include "defs_pipeline.h"
#include "../entities/segment.h"

#include "processor_solid_block.h"

const std::vector<NormalVertexStruct> FrontFace = {
	{
	DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
	DirectX::XMFLOAT2(0.0f, 1.0f),
	DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)
	}, {
	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
	DirectX::XMFLOAT2(0.0f, 0.0f),
	DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)
	}, {
	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
	DirectX::XMFLOAT2(1.0f, 1.0f),
	DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)
	}, {
	DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f),
	DirectX::XMFLOAT2(1.0f, 0.0f),
	DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)
	}
};

const std::vector<NormalVertexStruct> TopFace = {
	{
	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
	DirectX::XMFLOAT2(0.0f, 1.0f),
	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)
	}, {
	DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT2(0.0f, 0.0f),
	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)
	}, {
	DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f),
	DirectX::XMFLOAT2(1.0f, 1.0f),
	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)
	}, {
	DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT2(1.0f, 0.0f),
	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)
	}
};

const std::vector<NormalVertexStruct> RightFace = {
	{
	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
	DirectX::XMFLOAT2(0.0f, 1.0f),
	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)
	}, {
	DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f),
	DirectX::XMFLOAT2(0.0f, 0.0f),
	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)
	}, {
	DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT2(1.0f, 1.0f),
	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)
	}, {
	DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT2(1.0f, 0.0f),
	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)
	}
};

const std::vector<NormalVertexStruct> LeftFace = {
	{
	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT2(0.0f, 1.0f),
	DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)
	}, {
	DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT2(0.0f, 0.0f),
	DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)
	}, {
	DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
	DirectX::XMFLOAT2(1.0f, 1.0f),
	DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)
	}, {
	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
	DirectX::XMFLOAT2(1.0f, 0.0f),
	DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)
	}
};

const std::vector<NormalVertexStruct> BottomFace = {
	{
	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT2(0.0f, 1.0f),
	DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)
	}, {
	DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
	DirectX::XMFLOAT2(0.0f, 0.0f),
	DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)
	}, {
	DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT2(1.0f, 1.0f),
	DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)
	}, {
	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
	DirectX::XMFLOAT2(1.0f, 0.0f),
	DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)
	}
};

const std::vector<NormalVertexStruct> BackFace = {
	{
	DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT2(1.0f, 0.0f),
	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)
	}, {
	DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT2(1.0f, 1.0f),
	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)
	}, {

	DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT2(0.0f, 0.0f),
	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)
	}, {
	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT2(0.0f, 1.0f),
	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)
	}
};

const std::vector<unsigned int> Indices = {
	0, 1, 3,
	0, 3, 2
};

void SolidBlockProcessor::Rebuild(Segment* target)
{
	target->vertex_buffer->Clear();
	target->index_buffer->Clear();
	NormalVertexStruct _vertex = {};
	//test for now, add all surface layers
	//TODO:: change to actual algorithm
	/*for (unsigned int x = 0; x < SEGMENT_DIMENSION_SIZE; x++)
	{
		for (unsigned int y = 0; y < SEGMENT_DIMENSION_SIZE; y++)
		{
			for (unsigned int z = 0; z < SEGMENT_DIMENSION_SIZE; z++)
			{

			}
		}
	}*/
	
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int z = 0;
	unsigned int current_index = 0;
	for (y = 0; y < SEGMENT_DIMENSION_SIZE; y++)
	{
		for (z = 0; z < SEGMENT_DIMENSION_SIZE; z++)
		{
			for (auto& _face_vertex : LeftFace)
			{
				_vertex = _face_vertex;
				auto _pos = target->blocks[x][y][z]->Position();
				_vertex.position.y = _face_vertex.position.y + _pos.y;
				_vertex.position.z = _face_vertex.position.z + _pos.z;
				target->vertex_buffer->AddVertex(_vertex);
			}
			for (auto& _index : Indices)
			{
				target->index_buffer->AddIndex(_index + current_index);
			}
			current_index += 4;
		}
	}
	y = 0;
	for (x = 0; x < SEGMENT_DIMENSION_SIZE; x++)
	{
		for (z = 0; z < SEGMENT_DIMENSION_SIZE; z++)
		{
			for (auto& _face_vertex : TopFace)
			{
				_vertex = _face_vertex;
				auto _pos = target->blocks[x][y][z]->Position();
				_vertex.position.x = _face_vertex.position.x + _pos.x;
				_vertex.position.z = _face_vertex.position.z + _pos.z;
				target->vertex_buffer->AddVertex(_vertex);
			}
			for (auto& _index : Indices)
			{
				target->index_buffer->AddIndex(_index + current_index);
			}
			current_index += 4;
		}
	}

	z = 0;
	for (x = 0; x < SEGMENT_DIMENSION_SIZE; x++)
	{
		for (y = 0; y < SEGMENT_DIMENSION_SIZE; y++)
		{
			for (auto& _face_vertex : FrontFace)
			{
				_vertex = _face_vertex;
				auto _pos = target->blocks[x][y][z]->Position();
				_vertex.position.x = _face_vertex.position.x + _pos.x;
				_vertex.position.y = _face_vertex.position.y + _pos.y;
				target->vertex_buffer->AddVertex(_vertex);
			}
			for (auto& _index : Indices)
			{
				target->index_buffer->AddIndex(_index + current_index);
			}
			current_index += 4;
		}
	}

	target->vertex_buffer->Build();
	target->index_buffer->Build();

}
