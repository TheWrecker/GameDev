
#include "../visuals/elements/buffer_index.h"

#include "proxy.h"

Proxy::Proxy(float x, float y, float z)
    :position(DirectX::XMFLOAT3(x,y,z))
{
    world_matrix = DirectX::XMMatrixTranslation(x, y, z);
    vertex_buffer = std::make_unique<VertexBuffer<ColorVertexStruct>>();
    index_buffer = std::make_unique<IndexBuffer>();
}

Proxy::~Proxy()
{
}

void Proxy::SetPosition(float x, float y, float z)
{
    position.x = x;
    position.y = y;
    position.z = z;
    world_matrix = DirectX::XMMatrixTranslation(x, y, z);
}

const DirectX::XMFLOAT3& Proxy::GetPosition() const
{
    return position;
}

const DirectX::XMMATRIX& Proxy::World_Matrix() const
{
    return world_matrix;
}

VertexBuffer<ColorVertexStruct>* Proxy::GetVertexBuffer()
{
    return vertex_buffer.get();
}

IndexBuffer* Proxy::GetIndexBuffer()
{
    return index_buffer.get();
}
