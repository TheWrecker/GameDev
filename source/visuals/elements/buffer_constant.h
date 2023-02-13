
#pragma once

#include <d3d11.h>
#include <vector>

#include "util_funcs.h"

enum class BindStage
{
	VERTEX,
	PIXEL
};

template <typename type>
class ConstantBuffer
{
public:
	ConstantBuffer(ID3D11Device* device, ID3D11DeviceContext* context);
	~ConstantBuffer();

	void Update(type info);
	void Bind(BindStage stage, std::size_t slot);
	void Unbind();

private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	type data;
	ID3D11Buffer* buffer;
	D3D11_BUFFER_DESC desc;
	D3D11_MAPPED_SUBRESOURCE subresource;
	std::size_t current_slot;
	BindStage current_stage;
};

template<typename type>
inline ConstantBuffer<type>::ConstantBuffer(ID3D11Device* device, ID3D11DeviceContext* context)
	:device(device), context(context), buffer(), desc(), subresource(), data()
{
	ZeroMemory(&desc, sizeof(desc));
	desc.ByteWidth = sizeof(type);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	DXAssert(device->CreateBuffer(&desc, 0, &buffer));
	ZeroMemory(&subresource, sizeof(subresource));
}

template<typename type>
inline ConstantBuffer<type>::~ConstantBuffer()
{
	DXRelease(buffer);
}

template<typename type>
inline void ConstantBuffer<type>::Update(type info)
{
	data = info;
	ZeroMemory(&subresource, sizeof(subresource));
	context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
	memcpy(subresource.pData, &data, sizeof(data));
	context->Unmap(buffer, 0);
}

template<typename type>
inline void ConstantBuffer<type>::Bind(BindStage stage, std::size_t slot)
{
	switch (stage)
	{
		case BindStage::VERTEX:
		{
			context->VSSetConstantBuffers(slot, 1, &buffer);
			break;
		}
		case BindStage::PIXEL:
		{
			context->PSSetConstantBuffers(slot, 1, &buffer);
			break;
		}
		default:
		{
			assert(false);
			return;
		}
	}
	current_stage = stage;
	current_slot = slot;
}

template<typename type>
inline void ConstantBuffer<type>::Unbind()
{
	ID3D11Buffer* nullBuffer = { nullptr };
	switch (current_stage)
	{
		case BindStage::VERTEX:
		{
			context->VSSetConstantBuffers(current_slot, 1, &nullBuffer);
			break;
		}
		case BindStage::PIXEL:
		{
			context->PSSetConstantBuffers(current_slot, 1, &nullBuffer);
			break;
		}
		default:
		{
			assert(false);
			break;
		}
	}
}
