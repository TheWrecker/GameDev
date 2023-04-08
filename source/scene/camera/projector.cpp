
#include "projector.h"

Projector::Projector()
    :BasicCamera()
{
    DirectX::XMFLOAT4X4 _temp_mat = {};

    float _scalingBiasX = 0.5f + (0.5f / 800.0f);
    float _scalingBiasY = 0.5f + (0.5f / 600.0f);

    _temp_mat._11 = 0.5f;
    _temp_mat._22 = -0.5f;
    _temp_mat._33 = 1.0f;
    _temp_mat._41 = _scalingBiasX;
    _temp_mat._42 = _scalingBiasY;
    _temp_mat._44 = 1.0f;

    screen_matrix = DirectX::XMLoadFloat4x4(&_temp_mat);

    /*screen_matrix = {
        DirectX::XMVECTOR{ +0.5f, +0.0f, +0.0f, +0.0f },
        DirectX::XMVECTOR{ +0.0f, -0.5f, +0.0f, +0.0f },
        DirectX::XMVECTOR{ +0.5f, +0.0f, +1.0f, +0.0f },
        DirectX::XMVECTOR{ +0.5f, +0.5f, +0.0f, +1.0f }
    };*/
}

Projector::~Projector()
{
}

const DirectX::XMMATRIX Projector::Screen_Matrix() const
{
    return screen_matrix;
}

const DirectX::XMMATRIX Projector::View_Projection_Screen_Matrix() const
{
    return DirectX::XMMatrixMultiply(View_Projection_Matrix(), screen_matrix);
}
