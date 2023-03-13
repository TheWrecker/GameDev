
#include "../processors/processor_solid_block.h"
#include "../visuals/scene.h"
#include "../visuals/presenter.h"

#include "segment.h"

Segment::Segment(Scene* scene, SolidBlockType type, bool fill, float x, float y, float z)
    :BasicEntity(x, y, z), default_type(type), blocks(), scene(scene)
{
    vertex_buffer = std::make_unique<VertexBuffer<SolidBlockVertex>>(scene->GetDevice(), scene->GetContext());
    index_buffer = std::make_unique<IndexBuffer>(scene->GetDevice(), scene->GetContext());

    for (unsigned int i = 0; i < SEGMENT_DIMENSION_SIZE; i++)
    {
        for (unsigned int j = 0; j < SEGMENT_DIMENSION_SIZE; j++)
        {
            for (unsigned int k = 0; k < SEGMENT_DIMENSION_SIZE; k++)
            {
                blocks[i][j][k] = nullptr;
            }
        }
    }

    if (fill)
    {
        Fill(default_type);
        RebuildBuffers();
    }
}

Segment::~Segment()
{
    for (unsigned int i = 0; i < SEGMENT_DIMENSION_SIZE; i++)
    {
        for (unsigned int j = 0; j < SEGMENT_DIMENSION_SIZE; j++)
        {
            for (unsigned int k = 0; k < SEGMENT_DIMENSION_SIZE; k++)
            {
                if (blocks[i][j][k])
                    delete blocks[i][j][k];
            }
        }
    }
}

void Segment::SetType(SolidBlockType type)
{
    default_type = type;
}

void Segment::Move(float x, float y, float z)
{
    SetPosition(x, y, z);
    //RebuildBuffers();
    for (unsigned int x = 0; x < SEGMENT_DIMENSION_SIZE; x++)
        for (unsigned int y = 0; y < SEGMENT_DIMENSION_SIZE; y++)
            for (unsigned int z = 0; z < SEGMENT_DIMENSION_SIZE; z++)
            {
                if (blocks[x][y][z])
                    blocks[x][y][z]->SetPosition(position.x + (x * SOLID_BLOCK_SIZE), position.y + (y * SOLID_BLOCK_SIZE), position.z + (z * SOLID_BLOCK_SIZE));
            }
}

void Segment::AddBlock(SolidBlock* target, unsigned int x, unsigned int y, unsigned int z)
{
    assert(target);
    if (blocks[x][y][z])
        delete blocks[x][y][z];
    blocks[x][y][z] = target;
    target->SetPosition(position.x + (x * SOLID_BLOCK_SIZE), position.y + (y * SOLID_BLOCK_SIZE), position.z + (z * SOLID_BLOCK_SIZE));
    //TODO: should rebuild buffers?
    //RebuildBuffers();
}

void Segment::AddBlock(SolidBlock* target)
{
    AddBlock(target, 0, 0, 0);
}

void Segment::AddBlock(SolidBlockType type, unsigned int x, unsigned int y, unsigned int z)
{
    SolidBlock* _block = new SolidBlock(type);
    AddBlock(_block, x, y, z);
}

void Segment::AddBlock(unsigned int x, unsigned int y, unsigned int z)
{
    SolidBlock* _block = new SolidBlock(default_type);
    AddBlock(_block, x, y, z);
}

void Segment::Fill(SolidBlockType type)
{
    SetType(type);
    for (unsigned int i = 0; i < SEGMENT_DIMENSION_SIZE; i++)
    {
        for (unsigned int j = 0; j < SEGMENT_DIMENSION_SIZE; j++)
        {
            for (unsigned int k = 0; k < SEGMENT_DIMENSION_SIZE; k++)
            {
                if (blocks[i][j][k])
                    delete blocks[i][j][k];
                AddBlock(i, j, k);
            }
        }
    }
}

void Segment::RemoveBlock(unsigned int x, unsigned int y, unsigned int z)
{
    if (blocks[x][y][z])
    {
        delete blocks[x][y][z];
        blocks[x][y][z] = nullptr;
    }
}

void Segment::RemoveBlock(unsigned int index)
{
    auto _indices = GetArrayIndices(index);
    RemoveBlock(_indices.x, _indices.y, _indices.z);
}

void Segment::RebuildBuffers()
{
    SolidBlockProcessor::Rebuild(this);
}

SegmentIndices Segment::GetArrayIndices(unsigned int value)
{
    SegmentIndices _indices = {};
    _indices.x = value / (SEGMENT_DIMENSION_SIZE * SEGMENT_DIMENSION_SIZE);
    unsigned int _temp = value - (_indices.x * SEGMENT_DIMENSION_SIZE * SEGMENT_DIMENSION_SIZE);
    _indices.y = _temp / SEGMENT_DIMENSION_SIZE;
    _indices.z = _temp - (_indices.y * SEGMENT_DIMENSION_SIZE);
    return _indices;
}

VertexBuffer<SolidBlockVertex>* Segment::GetVertexBuffer()
{
    return vertex_buffer.get();
}

IndexBuffer* Segment::GetIndexBuffer()
{
    return index_buffer.get();
}
