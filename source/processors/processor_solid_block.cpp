
#include <DirectXMath.h>

#include "../visuals/elements/texture_atlas.h"
#include "../entities/segment.h"

#include "processor_solid_block.h"

Face RightFace = {
	{
	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
	DirectX::XMFLOAT2(0.0f, 1.0f),
	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f),
	DirectX::XMFLOAT2(0.0f, 0.0f),
	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT2(1.0f, 1.0f),
	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT2(1.0f, 0.0f),
	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
	0.0f
	}
};

Face LeftFace = {
	{
	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT2(0.0f, 1.0f),
	DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT2(0.0f, 0.0f),
	DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
	DirectX::XMFLOAT2(1.0f, 1.0f),
	DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
	DirectX::XMFLOAT2(1.0f, 0.0f),
	DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),
	0.0f
	}
};

Face TopFace = {
	{
	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
	DirectX::XMFLOAT2(0.0f, 1.0f),
	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT2(0.0f, 0.0f),
	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f),
	DirectX::XMFLOAT2(1.0f, 1.0f),
	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT2(1.0f, 0.0f),
	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
	0.0f
	}
};

Face BottomFace = {
	{
	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT2(0.0f, 1.0f),
	DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
	DirectX::XMFLOAT2(0.0f, 0.0f),
	DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT2(1.0f, 1.0f),
	DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
	DirectX::XMFLOAT2(1.0f, 0.0f),
	DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f),
	0.0f
	}
};

Face FrontFace = {
	{
	DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f),
	DirectX::XMFLOAT2(1.0f, 0.0f),
	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
	DirectX::XMFLOAT2(1.0f, 1.0f),
	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
	0.0f
	}, {

	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
	DirectX::XMFLOAT2(0.0f, 0.0f),
	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
	DirectX::XMFLOAT2(0.0f, 1.0f),
	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
	0.0f
	}
};

Face BackFace = {
	{
	DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT2(0.0f, 0.0f),
	DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT2(0.0f, 1.0f),
	DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT2(1.0f, 0.0f),
	DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT2(1.0f, 1.0f),
	DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),
	0.0f
	}
};

const std::vector<unsigned int> Indices = {
	0, 3, 1,
	0, 2, 3
};

unsigned int SolidBlockProcessor::index_x = 0;
unsigned int SolidBlockProcessor::index_y = 0;
unsigned int SolidBlockProcessor::index_z = 0;
unsigned int SolidBlockProcessor::solids = 0;
unsigned int SolidBlockProcessor::current_index = 0;
TextureAtlas* SolidBlockProcessor::texture_atlas = nullptr;

bool SolidBlockProcessor::CheckBlockFace(Segment* target, FaceName face)
{
	switch (face)
	{
		case FaceName::LEFT:
		{
			if (index_x == 0)
				return true;

			if (!target->blocks[index_x - 1][index_y][index_z])
				return true;

			break;
		}
		case FaceName::RIGHT:
		{
			if (index_x == SEGMENT_DIMENSION_SIZE - 1)
				return true;

			if (!target->blocks[index_x + 1][index_y][index_z])
				return true;

			break;
		}
		case FaceName::TOP:
		{
			if (index_y == SEGMENT_DIMENSION_SIZE - 1)
				return true;

			if (!target->blocks[index_x][index_y + 1][index_z])
				return true;
			break;
		}
		case FaceName::BOTTOM:
		{
			if (index_y == 0)
				return true;

			if (!target->blocks[index_x][index_y - 1][index_z])
				return true;
			break;
		}
		case FaceName::FRONT:
		{
			if (index_z == 0)
				return true;

			if (!target->blocks[index_x][index_y][index_z - 1])
				return true;

			break;
		}
		case FaceName::BACK:
		{
			if (index_z == SEGMENT_DIMENSION_SIZE - 1)
				return true;

			if (!target->blocks[index_x][index_y][index_z + 1])
				return true;
			break;
		}
		default:
		{
			assert(false);
			break;
		}
	}
	return false;
}

void SolidBlockProcessor::AddFaceVertices(Segment* target, Face& face)
{
	SolidBlockVertex _vertex = {};
	auto _array_range = texture_atlas->GetArraySliceCount() - 1.0f;
	for (auto& _face_vertex : face)
	{
		_vertex = _face_vertex;
		//auto _pos = target->blocks[index_x][index_y][index_z]->Position();
		_vertex.position.x = _face_vertex.position.x + (index_x * SOLID_BLOCK_SIZE); //_pos.x;
		_vertex.position.y = _face_vertex.position.y + (index_y * SOLID_BLOCK_SIZE); //_pos.y;
		_vertex.position.z = _face_vertex.position.z + (index_z * SOLID_BLOCK_SIZE); //_pos.z;
		_vertex.atlas_slice = static_cast<float>(target->blocks[index_x][index_y][index_z]->GetAtlasIndex()) / _array_range;
		target->vertex_buffer->AddVertex(_vertex);
	}
	for (auto& _index : Indices)
	{
		target->index_buffer->AddIndex(_index + current_index);
	}
	current_index += 4;
}

void SolidBlockProcessor::Setup(TextureAtlas* atlas)
{
	texture_atlas = atlas;
}

void SolidBlockProcessor::Rebuild(Segment* target)
{
	if (!texture_atlas)
		return;

	target->vertex_buffer->Clear();
	target->index_buffer->Clear();
	index_x = index_y = index_z = solids = current_index = 0;

	//TODO: better approach/algorithm
	for (index_x = 0; index_x < SEGMENT_DIMENSION_SIZE; index_x++)
		for (index_y = 0; index_y < SEGMENT_DIMENSION_SIZE; index_y++)
			for (index_z = 0; index_z < SEGMENT_DIMENSION_SIZE; index_z++)
			{
				if (!target->blocks[index_x][index_y][index_z])
					continue;

				if (CheckBlockFace(target, FaceName::LEFT))
					AddFaceVertices(target, LeftFace);

				if (CheckBlockFace(target, FaceName::RIGHT))
					AddFaceVertices(target, RightFace);

				if (CheckBlockFace(target, FaceName::TOP))
					AddFaceVertices(target, TopFace);

				if (CheckBlockFace(target, FaceName::BOTTOM))
					AddFaceVertices(target, BottomFace);

				if (CheckBlockFace(target, FaceName::FRONT))
					AddFaceVertices(target, FrontFace);

				if (CheckBlockFace(target, FaceName::BACK))
					AddFaceVertices(target, BackFace);
			}
	
	target->vertex_buffer->Build();
	target->index_buffer->Build();
}
