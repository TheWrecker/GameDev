
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
    float yaw = 0.0f;
    if (direction.y > 0.0f)
    {
        yaw = 1.0f - direction.y;
    }
    else
    {
        yaw = -1.0f - direction.y;
    }
    float pitch = direction.x;

    end.x -= cos(yaw) * scale;
    end.z -= sin(yaw) * scale;
    end.y -= tan(pitch) * scale;

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

const DirectX::XMFLOAT3& Ray::GetEnd() const
{
    return end;
}
