
#include "sun.h"

Sun::Sun()
    :light_info()
{
    SetPosition(0.0f,200.0f,0.0f);
    SetScale(25.0f);
    light_info.direction = { -0.3f, 1.0f, -0.6f, 1.0f };
    light_info.color = { 1.0f, 1.0f, 0.9f, 0.8f };
    light_info.ambient = { 1.0f, 1.0f, 1.0f, 0.5f };
}

Sun::~Sun()
{
}

void Sun::SetPosition(float x, float y, float z)
{
    position.x = x;
    position.y = y;
    position.z = z;
    translation_matrix = DirectX::XMMatrixTranslation(x, y, z);
}

void Sun::SetScale(float value)
{
    scale = value;
    scale_matrix = DirectX::XMMatrixScaling(scale, scale, scale);
}

DirectX::XMMATRIX Sun::GetLightInfo()
{
    DirectX::XMMATRIX _temp = {};
    _temp.r[0] = DirectX::XMLoadFloat4(&light_info.direction);
    _temp.r[1] = DirectX::XMLoadFloat4(&light_info.color);
    _temp.r[2] = DirectX::XMLoadFloat4(&light_info.ambient);
    _temp.r[3] = DirectX::XMLoadFloat4(&light_info.UNUSED);
    return _temp;
}

DirectX::XMMATRIX Sun::World_Matrix()
{
    return DirectX::XMMatrixMultiply(scale_matrix, translation_matrix);
}
