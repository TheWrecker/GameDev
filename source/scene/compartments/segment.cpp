
#include "../visuals/elements/buffer_index.h"
#include "../processors/processor_solid_block.h"
#include "../scene/scene.h"
#include "../visuals/presenter.h"
#include "../core/supervisor.h"

#include "segment.h"

Segment::Segment(Scene* scene, BlockType type, bool fill, float x, float y, float z)
    :default_type(type), blocks(), scene(scene), block_count(0), biome_processed(false)
{
    position = { x, y, z };
    vertex_buffer = std::make_unique<VertexBuffer<SolidBlockVertex>>(scene->GetDevice(), scene->GetContext());
    index_buffer = std::make_unique<IndexBuffer>(scene->GetDevice(), scene->GetContext());

    for (unsigned int i = 0; i < SEGMENT_DIMENSION_SIZE; i++)
    {
        for (unsigned int j = 0; j < SEGMENT_DIMENSION_SIZE; j++)
        {
            for (unsigned int k = 0; k < SEGMENT_DIMENSION_SIZE; k++)
            {
                blocks[i][j][k] = BlockType::EMPTY;
            }
        }
    }

    if (fill)
    {
        Fill(default_type);
        //RebuildBuffers();
    }
}

Segment::~Segment()
{

}

void Segment::SetType(BlockType type)
{
    default_type = type;
}

void Segment::Move(float x, float y, float z)
{
    position = { x, y, z };
    //RebuildBuffers();
}

void Segment::Fill(BlockType type)
{
    SetType(type);
    for (unsigned int i = 0; i < SEGMENT_DIMENSION_SIZE; i++)
    {
        for (unsigned int j = 0; j < SEGMENT_DIMENSION_SIZE; j++)
        {
            for (unsigned int k = 0; k < SEGMENT_DIMENSION_SIZE; k++)
            {
                blocks[i][j][k] = type;
            }
        }
    }
    block_count = SEGMENT_DIMENSION_SIZE * SEGMENT_DIMENSION_SIZE * SEGMENT_DIMENSION_SIZE;
}

void Segment::SetBlock(unsigned int x, unsigned int y, unsigned int z, BlockType type)
{
    if (blocks[x][y][z] == BlockType::EMPTY)
        block_count++;

    blocks[x][y][z] = type;
}

void Segment::RemoveBlock(unsigned int x, unsigned int y, unsigned int z)
{
    if (blocks[x][y][z] == BlockType::EMPTY)
        return;

    blocks[x][y][z] = BlockType::EMPTY;
    block_count--;
}

void Segment::RebuildBuffers(Sector* parent, SegmentIndex index)
{
    SolidBlockProcessor::Rebuild(parent, this, index);
}

const DirectX::XMMATRIX Segment::World_Matrix()
{
    return DirectX::XMMatrixTranslation(position.x, position.y, position.z);
}

const DirectX::XMFLOAT3& Segment::Position()
{
    return position;
}

VertexBuffer<SolidBlockVertex>* Segment::GetVertexBuffer()
{
    return vertex_buffer.get();
}

IndexBuffer* Segment::GetIndexBuffer()
{
    return index_buffer.get();
}

bool Segment::IsEmpty()
{
    return block_count == 0;
}
