
#include "projector.h"

Projector::Projector()
    :BasicCamera()
{
    screen_matrix = {
        DirectX::XMVECTOR{ +0.5f, +0.0f, +0.0f, +0.0f },
        DirectX::XMVECTOR{ +0.0f, -0.5f, +0.0f, +0.0f },
        DirectX::XMVECTOR{ +0.5f, +0.0f, +1.0f, +0.0f },
        DirectX::XMVECTOR{ +0.5f, +0.5f, +0.0f, +1.0f }
    };
}

Projector::~Projector()
{
}

const DirectX::XMMATRIX Projector::View_Projection_Screen_Matrix() const
{
    return DirectX::XMMatrixMultiply(View_Projection_Matrix(), screen_matrix);
}
