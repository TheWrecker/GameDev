
#include "ray.h"

Ray::Ray(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3& dir)
    :position(pos), direction(dir), end(pos), length(0.0f)
{
    position_vector = DirectX::XMLoadFloat3(&pos);
    direction_vector = DirectX::XMLoadFloat3(&dir);
    end_vector = position_vector;
}

Ray::~Ray()
{
}

void Ray::Advance(float scale)
{
    end.x += direction.x * scale;
    end.y += direction.y * scale;
    end.z += direction.z * scale;

    end_vector = DirectX::XMLoadFloat3(&end);

    auto _sub = DirectX::XMVectorSubtract(end_vector, position_vector);
    auto _length = DirectX::XMVector3Length(_sub);
    DirectX::XMStoreFloat(&length, _length);
}

bool Ray::DoesIntersectBox(DirectX::BoundingBox& target, float distance)
{
    return target.Intersects(position_vector, direction_vector, distance);
}

float Ray::GetLength()
{
    return length;
}
