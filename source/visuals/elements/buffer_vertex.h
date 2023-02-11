
#pragma once

#include <d3d11.h>
#include <vector>

#include "util_funcs.h"

template <typename type>
class VertexBuffer
{
public:
	VertexBuffer(ID3D11Device* device, ID3D11DeviceContext* context, std::size_t reserve = 0);
	~VertexBuffer();

	void AddVertex(type vertex);
	void Clear();
	void Build();
	void Bind();
	void Unbind();

private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	std::vector<type> vertices;
	ID3D11Buffer* buffer;
	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA subresource;
	unsigned int
		vertex_stride,
		vertex_offset;
};

template<typename type>
inline VertexBuffer<type>::VertexBuffer(ID3D11Device* device, ID3D11DeviceContext* context, std::size_t reserve)
	:device(device), context(context), buffer(), desc(), subresource()
{
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ZeroMemory(&subresource, sizeof(subresource));
	vertex_stride = sizeof(type);
	vertex_offset = 0;
	if (reserve > 0)
		vertices.reserve(reserve);
}

template<typename type>
inline VertexBuffer<type>::~VertexBuffer()
{
	DXRelease(buffer);
}

template<typename type>
inline void VertexBuffer<type>::AddVertex(type vertex)
{
	vertices.push_back(vertex);
}

template<typename type>
inline void VertexBuffer<type>::Clear()
{
	vertices.clear();
}

template<typename type>
inline void VertexBuffer<type>::Build()
{
	if (vertices.size() > 0)
	{
		subresource.pSysMem = &vertices.at(0);
		desc.ByteWidth = sizeof(type) * vertices.size();
		DXAssert(device->CreateBuffer(&desc, &subresource, &buffer));
	}
}

template<typename type>
inline void VertexBuffer<type>::Bind()
{
	context->IASetVertexBuffers(0, 1, &buffer, &vertex_stride, &vertex_offset);
}

template<typename type>
inline void VertexBuffer<type>::Unbind()
{
	ID3D11Buffer* nullBuffer = { nullptr };
	context->IASetVertexBuffers(0, 1, &nullBuffer, &vertex_stride, &vertex_offset);
}
