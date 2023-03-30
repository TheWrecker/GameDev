
#include <DirectXMath.h>

#include "../visuals/elements/buffer_index.h"
#include "../visuals/elements/buffer_vertex.h"
#include "../scene/assets/texture_atlas.h"
#include "../entities/block_solid.h"
#include "../scene/compartments/segment.h"
#include "../scene/compartments/sector.h"
#include "../scene/world.h"
#include "../scene/scene.h"
#include "../core/supervisor.h"

#include "processor_solid_block.h"

constexpr auto ONE_THIRD = 1.0f / 3.0f;
constexpr auto TWO_THIRDS = 2.0f / 3.0f;
constexpr auto HALF_BLOCK_SIZE = SOLID_BLOCK_SIZE / 2.0f;
constexpr auto THREEHALVES_BLOCK_SIZE = SOLID_BLOCK_SIZE * 1.5f;

Face RightFace = {
	{
	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
	DirectX::XMFLOAT2(0.0f, TWO_THIRDS),
	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f),
	DirectX::XMFLOAT2(0.0f, ONE_THIRD),
	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT2(1.0f, TWO_THIRDS),
	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT2(1.0f, ONE_THIRD),
	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
	0.0f
	}
};

Face LeftFace = {
	{
	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT2(0.0f, TWO_THIRDS),
	DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT2(0.0f, ONE_THIRD),
	DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
	DirectX::XMFLOAT2(1.0f, TWO_THIRDS),
	DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
	DirectX::XMFLOAT2(1.0f, ONE_THIRD),
	DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),
	0.0f
	}
};

Face TopFace = {
	{
	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
	DirectX::XMFLOAT2(0.0f, ONE_THIRD),
	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT2(0.0f, 0.0f),
	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f),
	DirectX::XMFLOAT2(1.0f, ONE_THIRD),
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
	DirectX::XMFLOAT2(0.0f, TWO_THIRDS),
	DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT2(1.0f, 1.0f),
	DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
	DirectX::XMFLOAT2(1.0f, TWO_THIRDS),
	DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f),
	0.0f
	}
};

Face FrontFace = {
	{
	DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f),
	DirectX::XMFLOAT2(1.0f, ONE_THIRD),
	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
	DirectX::XMFLOAT2(1.0f, TWO_THIRDS),
	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
	0.0f
	}, {

	DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f),
	DirectX::XMFLOAT2(0.0f, ONE_THIRD),
	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
	DirectX::XMFLOAT2(0.0f, TWO_THIRDS),
	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
	0.0f
	}
};

Face BackFace = {
	{
	DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT2(0.0f, ONE_THIRD),
	DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT2(0.0f, TWO_THIRDS),
	DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),
	DirectX::XMFLOAT2(1.0f, ONE_THIRD),
	DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),
	0.0f
	}, {
	DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f),
	DirectX::XMFLOAT2(1.0f, TWO_THIRDS),
	DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f),
	0.0f
	}
};

const std::vector<unsigned int> Indices = {
	0, 3, 1,
	0, 2, 3
};

thread_local unsigned int SolidBlockProcessor::index_x = 0;
thread_local unsigned int SolidBlockProcessor::index_y = 0;
thread_local unsigned int SolidBlockProcessor::index_z = 0;
thread_local unsigned int SolidBlockProcessor::solids = 0;
thread_local unsigned int SolidBlockProcessor::current_index = 0;
thread_local DirectX::XMFLOAT3 SolidBlockProcessor::position = {};

TextureAtlas* SolidBlockProcessor::texture_atlas = nullptr;
World* SolidBlockProcessor::world = nullptr;

bool SolidBlockProcessor::CheckNextSegmentBlock(Sector* sector, SegmentIndex& index, FaceName face)
{
	//TODO: check even the adjacent sectors

	BlockType _block = BlockType::EMPTY;
	switch (face)
	{
		case FaceName::LEFT:
		{
			if (index.x == 0)
				return true;

			auto _segment = sector->segments[index.x - 1][index.y][index.z].load();
			if (!_segment)
				return true;

			_block = _segment->blocks[SEGMENT_DIMENSION_SIZE - 1][index_y][index_z];
			if (_block == BlockType::EMPTY)
				return true;

			break; 
		}
		case FaceName::RIGHT:
		{
			if (index.x == SECTOR_HORIZONTAL_SIZE - 1)
				return true;

			auto _segment = sector->segments[index.x + 1][index.y][index.z].load();
			if (!_segment)
				return true;

			_block = _segment->blocks[0][index_y][index_z];
			if (_block == BlockType::EMPTY)
				return true;

			break;
		}
		case FaceName::TOP:
		{
			if (index.y == SECTOR_VERTICAL_SIZE - 1)
				return true;

			auto _segment = sector->segments[index.x][index.y + 1][index.z].load();
			if (!_segment)
				return true;

			_block = _segment->blocks[index_x][0][index_z];
			if (_block == BlockType::EMPTY)
				return true;

			break;
		}
		case FaceName::BOTTOM:
		{
			if (index.y == 0)
				return true;

			auto _segment = sector->segments[index.x][index.y - 1][index.z].load();
			if (!_segment)
				return true;

			_block = _segment->blocks[index_x][SEGMENT_DIMENSION_SIZE - 1][index_z];
			if (_block == BlockType::EMPTY)
				return true;

			break;
		}
		case FaceName::FRONT:
		{
			if (index.z == 0)
				return true;

			auto _segment = sector->segments[index.x][index.y][index.z - 1].load();
			if (!_segment)
				return true;

			_block = _segment->blocks[index_x][index_y][SEGMENT_DIMENSION_SIZE - 1];
			if (_block == BlockType::EMPTY)
				return true;

			break;
		}
		case FaceName::BACK:
		{
			if (index.z == SECTOR_HORIZONTAL_SIZE - 1)
				return true;

			auto _segment = sector->segments[index.x][index.y][index.z + 1].load();
			if (!_segment)
				return true;

			_block = _segment->blocks[index_x][index_y][0];
			if (_block == BlockType::EMPTY)
				return true;

			break;
		}
		default:
			break;
	}
	return false;
}

bool SolidBlockProcessor::CheckBlockFace(Sector* sector, Segment* segment, SegmentIndex& index, FaceName face)
{
	switch (face)
	{
		case FaceName::LEFT:
		{
			if (index_x == 0)
				return CheckNextSegmentBlock(sector, index, face);

			if (segment->blocks[index_x - 1][index_y][index_z] == BlockType::EMPTY)
				return true;

			break;
		}
		case FaceName::RIGHT:
		{
			if (index_x == SEGMENT_DIMENSION_SIZE - 1)
				return CheckNextSegmentBlock(sector, index, face);

			if (segment->blocks[index_x + 1][index_y][index_z] == BlockType::EMPTY)
				return true;

			break;
		}
		case FaceName::TOP:
		{
			if (index_y == SEGMENT_DIMENSION_SIZE - 1)
				return CheckNextSegmentBlock(sector, index, face);

			if (segment->blocks[index_x][index_y + 1][index_z] == BlockType::EMPTY)
				return true;
			break;
		}
		case FaceName::BOTTOM:
		{
			if (index_y == 0)
				return CheckNextSegmentBlock(sector, index, face);

			if (segment->blocks[index_x][index_y - 1][index_z] == BlockType::EMPTY)
				return true;
			break;
		}
		case FaceName::FRONT:
		{
			if (index_z == 0)
				return CheckNextSegmentBlock(sector, index, face);

			if (segment->blocks[index_x][index_y][index_z - 1] == BlockType::EMPTY)
				return true;

			break;
		}
		case FaceName::BACK:
		{
			if (index_z == SEGMENT_DIMENSION_SIZE - 1)
				return CheckNextSegmentBlock(sector, index, face);

			if (segment->blocks[index_x][index_y][index_z + 1] == BlockType::EMPTY)
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
	auto _array_range = texture_atlas->GetArraySliceCount();
	for (auto& _face_vertex : face)
	{
		_vertex = _face_vertex;
		//auto _pos = target->blocks[index_x][index_y][index_z]->Position();
		_vertex.position.x = _face_vertex.position.x + (index_x * SOLID_BLOCK_SIZE); //_pos.x;
		_vertex.position.y = _face_vertex.position.y + (index_y * SOLID_BLOCK_SIZE); //_pos.y;
		_vertex.position.z = _face_vertex.position.z + (index_z * SOLID_BLOCK_SIZE); //_pos.z;
		_vertex.atlas_slice = (static_cast<float>(static_cast<unsigned int>(target->blocks[index_x][index_y][index_z]) - 1.0f)) /* / _array_range*/;
		target->vertex_buffer->AddVertex(_vertex);
	}
	for (auto& _index : Indices)
	{
		target->index_buffer->AddIndex(_index + current_index);
	}
	current_index += 4;
}

bool SolidBlockProcessor::Setup()
{
	texture_atlas = Supervisor::QueryService<Scene*>("scene")->GetTextureAtlas();
	world = Supervisor::QueryService<Scene*>("scene")->GetWorld();

	return texture_atlas && world;
}

void SolidBlockProcessor::Rebuild(Sector* sector, Segment* target, SegmentIndex index)
{
	if (!texture_atlas)
		return;

	target->vertex_buffer->Clear();
	target->index_buffer->Clear();
	index_x = index_y = index_z = solids = current_index = 0;
	BlockType _block = BlockType::EMPTY;

	SegmentIndex _index = index;

	//TODO: better approach/algorithm
	for (index_x = 0; index_x < SEGMENT_DIMENSION_SIZE; index_x++)
		for (index_y = 0; index_y < SEGMENT_DIMENSION_SIZE; index_y++)
			for (index_z = 0; index_z < SEGMENT_DIMENSION_SIZE; index_z++)
			{
				_block = target->blocks[index_x][index_y][index_z];
				if (_block == BlockType::EMPTY)
					continue;

				position.x = target->position.x + (index_x * SOLID_BLOCK_SIZE);
				position.y = target->position.y + (index_y * SOLID_BLOCK_SIZE);
				position.z = target->position.z + (index_z * SOLID_BLOCK_SIZE);

				if (CheckBlockFace(sector, target, _index, FaceName::LEFT))
					AddFaceVertices(target, LeftFace);

				if (CheckBlockFace(sector, target, _index, FaceName::RIGHT))
					AddFaceVertices(target, RightFace);

				if (CheckBlockFace(sector, target, _index, FaceName::TOP))
					AddFaceVertices(target, TopFace);

				if (CheckBlockFace(sector, target, _index, FaceName::BOTTOM))
					AddFaceVertices(target, BottomFace);

				if (CheckBlockFace(sector, target, _index, FaceName::FRONT))
					AddFaceVertices(target, FrontFace);

				if (CheckBlockFace(sector, target, _index, FaceName::BACK))
					AddFaceVertices(target, BackFace);
			}
	
	target->vertex_buffer->Build();
	target->index_buffer->Build();
}
