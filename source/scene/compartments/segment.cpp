
#include "../visuals/elements/buffer_index.h"
#include "../processors/processor_solid_block.h"
#include "../scene/scene.h"
#include "../visuals/presenter.h"
#include "../core/supervisor.h"

#include "segment.h"

Segment::Segment(Scene* scene, BlockType type, bool fill, int x, int y, int z)
    :default_type(type), blocks(), scene(scene), block_count(0), biome_processed(false)
{
    position = { x, y, z };
    vertex_buffer = new VertexBuffer<SolidBlockVertex>(scene->GetDevice(), scene->GetContext());
    index_buffer = new IndexBuffer(scene->GetDevice(), scene->GetContext());

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
    delete vertex_buffer;
    delete index_buffer;
}

void Segment::SetType(BlockType type)
{
    default_type = type;
}

void Segment::Move(int x, int y, int z)
{
    position = { x, y, z };
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
    mesh_rebuilt.store(false);
}

void Segment::SetBlock(unsigned int x, unsigned int y, unsigned int z, BlockType type)
{
    if (blocks[x][y][z] == BlockType::EMPTY)
        block_count++;

    blocks[x][y][z] = type;
    mesh_rebuilt.store(false);
}

void Segment::RemoveBlock(unsigned int x, unsigned int y, unsigned int z)
{
    if (blocks[x][y][z] == BlockType::EMPTY)
        return;

    blocks[x][y][z] = BlockType::EMPTY;
    block_count--;
    mesh_rebuilt.store(false);
}

void Segment::RebuildBuffers()
{
    SolidBlockProcessor::RebuildSegmentSingle(this);
    //we don't set mesh_rebuilt to true here, since we need to process the segment against adjacent segments and sectors
    //instead we create a preliminary mesh to prevent visual artifacts and then later finalize the mesh which
    //we do in the WorldEngine tick
}

const DirectX::XMMATRIX Segment::World_Matrix()
{
    return DirectX::XMMatrixTranslation((float)position.x, (float)position.y, (float)position.z);
}

const DirectX::XMINT3& Segment::Position()
{
    return position;
}

VertexBuffer<SolidBlockVertex>* Segment::GetVertexBuffer()
{
    return vertex_buffer;
}

IndexBuffer* Segment::GetIndexBuffer()
{
    return index_buffer;
}

bool Segment::IsEmpty()
{
    return block_count == 0;
}
