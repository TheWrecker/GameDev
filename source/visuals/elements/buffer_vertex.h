
#ifndef BUFFER_VERTEX_H
	#define BUFFER_VERTEX_H

	#include <d3d11.h>
	#include <vector>

	#include "util_funcs.h"

	template <typename type>
	class VertexBuffer
	{
	public:
		VertexBuffer(ID3D11Device* device = nullptr, ID3D11DeviceContext* context = nullptr, std::size_t reserve = 3000);
		~VertexBuffer();
		void Initialize(ID3D11Device* device, ID3D11DeviceContext* context);

		void AddVertex(type vertex);
		void Clear();
		void Build();
		void Bind(unsigned int slot);

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
			vertex_offset;

		bool is_built;
	};

	template<typename type>
	inline VertexBuffer<type>::VertexBuffer(ID3D11Device* device, ID3D11DeviceContext* context, std::size_t reserve)
		:device(device), context(context), buffer(), desc(), subresource(), is_built(false)
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
		if (is_built)
			DXRelease(buffer);
	}

	template<typename type>
	inline void VertexBuffer<type>::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
	{
		if (device && context)
		{
			this->device = device;
			this->context = context;
		}
	}

	template<typename type>
	inline void VertexBuffer<type>::AddVertex(type vertex)
	{
		vertices.push_back(vertex);
	}

	template<typename type>
	inline void VertexBuffer<type>::Clear()
	{
		if (is_built)
		{
			DXRelease(buffer);
			is_built = false;
		}

		vertices.clear();
	}

	template<typename type>
	inline void VertexBuffer<type>::Build()
	{
		if (is_built)
			DXRelease(buffer);

		assert(device && context);

		if (vertices.size() > 0)
		{
			subresource.pSysMem = &vertices.at(0);
			desc.ByteWidth = sizeof(type) * static_cast<unsigned int>(vertices.size());
			DXAssert(device->CreateBuffer(&desc, &subresource, &buffer));
			is_built = true;
		}
		else
		{
			is_built = false;
		}
	}

	template<typename type>
	inline void VertexBuffer<type>::Bind(unsigned int slot)
	{
		context->IASetVertexBuffers(slot, 1, &buffer, &vertex_stride, &vertex_offset);
	}

	template<typename type>
	inline unsigned int VertexBuffer<type>::GetVertexCount()
	{
		return vertices.size();
	}

#endif // !BUFFER_VERTEX_H
