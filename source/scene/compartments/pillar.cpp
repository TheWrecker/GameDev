
#include "segment.h"

#include "pillar.h"

Pillar::Pillar(Scene* scene, float x, float z)
    :scene(scene), x(x), z(z), biome_processed(false)
{
}

Pillar::~Pillar()
{
    for (auto& _element : segments)
    {
        delete _element.second;
    }
}

void Pillar::AddSegment(Segment* target, int index)
{
    auto _result = segments.find(index);
    if (_result == segments.end())
        segments.insert(std::pair(index, target));
}

Segment* Pillar::CreateSegment(int index)
{
    auto _result = segments.find(index);
    if (_result == segments.end())
    {
        auto _segment = new Segment(scene, SolidBlockType::TEST, false, x, index * SEGMENT_LENGTH, z);
        segments.insert(std::pair(index, _segment));
        return _segment;
    }
    else
    {
        return _result->second;
    }
}

Segment* Pillar::GetSegment(int index, bool force)
{
    auto _result = segments.find(index);
    if (_result != segments.end())
    {
        return _result->second;
    }
    else if (force)
    {
        auto _segment = new Segment(scene, SolidBlockType::TEST, false, x, index * SEGMENT_LENGTH, z);
        segments.insert(std::pair(index, _segment));
        return _segment;
    }
    return nullptr;
}
