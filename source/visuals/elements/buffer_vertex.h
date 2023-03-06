
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
	void Bind(unsigned int slot);
	void Unbind();

	ID3D11Buffer* GetBuffer();
	D3D11_BUFFER_DESC GetDesc();
	unsigned int GetVertexCount();

private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	std::vector<type> vertices;
	ID3D11Buffer* buffer;
	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA subresource;
	unsigned int
		vertex_stride,
		vertex_offset,
		current_slot;

	bool is_bound;
};

template<typename type>
inline VertexBuffer<type>::VertexBuffer(ID3D11Device* device, ID3D11DeviceContext* context, std::size_t reserve)
	:device(device), context(context), buffer(), desc(), subresource(), is_bound(false)
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
	Unbind();
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
		desc.ByteWidth = sizeof(type) * static_cast<unsigned int>(vertices.size());
		DXAssert(device->CreateBuffer(&desc, &subresource, &buffer));
	}
}

template<typename type>
inline void VertexBuffer<type>::Bind(unsigned int slot)
{
	context->IASetVertexBuffers(slot, 1, &buffer, &vertex_stride, &vertex_offset);
	current_slot = slot;
	is_bound = true;
}

template<typename type>
inline void VertexBuffer<type>::Unbind()
{
	if (!is_bound)
		return;

	ID3D11Buffer* _buffer = { nullptr };
	unsigned int _stride, _offset;
	context->IAGetVertexBuffers(current_slot, 1, &_buffer, &_stride, &_offset);
	if ((_buffer == buffer) && (_stride == vertex_stride) && (_offset == vertex_offset))
	{
		ID3D11Buffer* _null_buffer = { nullptr };
		_stride = _offset = 0;
		context->IASetVertexBuffers(current_slot, 1, &_null_buffer, &_stride, &_offset);
		is_bound = false;
	}
	DXRelease(_buffer);
}

template<typename type>
inline ID3D11Buffer* VertexBuffer<type>::GetBuffer()
{
	return buffer;
}

template<typename type>
inline D3D11_BUFFER_DESC VertexBuffer<type>::GetDesc()
{
	return desc;
}

template<typename type>
inline unsigned int VertexBuffer<type>::GetVertexCount()
{
	return vertices.size();
}
