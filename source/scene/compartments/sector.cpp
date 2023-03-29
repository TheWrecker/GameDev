
#include "segment.h"

#include "sector.h"

Sector::Sector(Scene* scene, float x, float z)
    :scene(scene), x(x), z(z), biomes_processed(false)
{
    for (unsigned int _x = 0; _x < SECTOR_HORIZONTAL_SIZE; _x++)
        for (unsigned int _y = 0; _y < SECTOR_VERTICAL_SIZE; _y++)
            for (unsigned int _z = 0; _z < SECTOR_HORIZONTAL_SIZE; _z++)
            {
                segments[_x][_y][_z] = nullptr;
            }
}

Sector::~Sector()
{
    for (unsigned int _x = 0; _x < SECTOR_HORIZONTAL_SIZE; _x++)
        for (unsigned int _y = 0; _y < SECTOR_VERTICAL_SIZE; _y++)
            for (unsigned int _z = 0; _z < SECTOR_HORIZONTAL_SIZE; _z++)
            {
                if (segments[_x][_y][_z])
                    delete segments[_x][_y][_z];
            }
}

Segment* Sector::CreateSegment(const SegmentIndex& index)
{
    auto _result = segments[index.x][index.y][index.z];
    if (!_result)
    {
        auto _segment = new Segment(scene, BlockType::TEST, false,
            x + index.x * SEGMENT_LENGTH, index.y * SEGMENT_LENGTH, z + index.z * SEGMENT_LENGTH);
        segments[index.x][index.y][index.z] = _segment;
        return _segment;
    }
    else
    {
        return _result;
    }
}

Segment* Sector::GetSegment(const SegmentIndex& index, bool force)
{
    auto _result = segments[index.x][index.y][index.z];
    if (_result)
    {
        return _result;
    }
    else if (force)
    {
        auto _segment = new Segment(scene, BlockType::TEST, false,
            x + index.x * SEGMENT_LENGTH, index.y * SEGMENT_LENGTH, z + index.z * SEGMENT_LENGTH);
        segments[index.x][index.y][index.z] = _segment;
        return _segment;
    }

    return nullptr;
}
