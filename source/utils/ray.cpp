
#include "ray.h"

Ray::Ray(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir)
    :position(pos), direction(dir), end(pos), length(0.0f)
{
    position_vector = DirectX::XMLoadFloat3(&pos);
    direction_vector = DirectX::XMLoadFloat3(&dir);
    end_vector = position_vector;
}

Ray::Ray(DirectX::XMVECTOR& pos, DirectX::XMVECTOR& dir)
    :position_vector(pos), direction_vector(dir), length(0.0f)
{
    DirectX::XMStoreFloat3(&position, position_vector);
    DirectX::XMStoreFloat3(&direction, direction_vector);
    end = position;
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

extern inline Ray ComputeIntersectionLine(DirectX::FXMVECTOR p1, DirectX::FXMVECTOR p2)
{
    using namespace DirectX;
    XMVECTOR direction = XMVector3Cross(p1, p2);
    XMVECTOR lengthSquared = XMVector3LengthSq(direction);
    XMVECTOR position = XMVector3Cross((-XMVectorGetW(p1) * p2) + (XMVectorGetW(p2) * p1), direction) / lengthSquared;

    return Ray(position, direction);
}

extern inline DirectX::XMVECTOR ComputeIntersectionPoint(DirectX::FXMVECTOR& plane, Ray& ray)
{
    using namespace DirectX;

    float value = (-XMVectorGetW(plane) - XMVectorGetX(XMVector3Dot(plane, ray.position_vector))) / XMVectorGetX(XMVector3Dot(plane, ray.direction_vector));

    //advance ray by value and return end?

    return (ray.position_vector + (ray.direction_vector * value));
}
