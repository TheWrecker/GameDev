


#include "frustrum.h"

Frustrum::Frustrum()
{
}

Frustrum::~Frustrum()
{
}

void Frustrum::CalculateFrustrum(DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projMatrix)
{
    DirectX::BoundingFrustum::CreateFromMatrix(frustrum, projMatrix);
    frustrum.Transform(frustrum, viewMatrix);
}

void Frustrum::CalculateFrustrum(DirectX::XMMATRIX viewProjMatrix)
{
    DirectX::BoundingFrustum::CreateFromMatrix(frustrum, viewProjMatrix);
}

bool Frustrum::IntersectsCube(DirectX::XMVECTOR minPoint, DirectX::XMVECTOR maxPoint)
{
    DirectX::BoundingBox _box = {};
    DirectX::BoundingBox::CreateFromPoints(_box, minPoint, maxPoint);
    return frustrum.Intersects(_box);
}
