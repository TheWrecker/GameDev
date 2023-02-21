
#include "sun.h"

Sun::Sun()
    :light_info()
{
    SetPosition(0.0f, 200.0f, 0.0f);
    light_info.direction = { 0.0f, 1.0f, 0.0f, 1.0f };
    light_info.color = { 1.0f, 1.0f, 0.9f, 0.8f };
    light_info.ambient = { 0.5f, 0.5f, 0.5f, 0.3f };
}

Sun::~Sun()
{
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
