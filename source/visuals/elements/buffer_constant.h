
#pragma once

#include <d3d11.h>
#include <vector>

#include "defs_pipeline.h"
#include "util_funcs.h"

template <typename type>
class ConstantBuffer
{
public:
	ConstantBuffer(ID3D11Device* device, ID3D11DeviceContext* context);
	~ConstantBuffer();

	void Update(type& info);
	void Bind(BindStage stage, unsigned int slot);
	void Unbind();

private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	type data;
	ID3D11Buffer* buffer;
	D3D11_BUFFER_DESC desc;
	D3D11_MAPPED_SUBRESOURCE subresource;
	unsigned int current_slot;
	BindStage current_stage;
	bool is_bound;
};

template<typename type>
inline ConstantBuffer<type>::ConstantBuffer(ID3D11Device* device, ID3D11DeviceContext* context)
	:device(device), context(context), buffer(), desc(), subresource(), data(), is_bound(false)
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
	Unbind();
	DXRelease(buffer);
}

template<typename type>
inline void ConstantBuffer<type>::Update(type& info)
{
	bool _was_bound = is_bound;
	if (is_bound)
		Unbind();

	data = info;
	ZeroMemory(&subresource, sizeof(subresource));
	context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
	memcpy(subresource.pData, &data, sizeof(data));
	context->Unmap(buffer, 0);

	if (_was_bound)
		Bind(current_stage, current_slot);
}

template<typename type>
inline void ConstantBuffer<type>::Bind(BindStage stage, unsigned int slot)
{
	if (is_bound)
		return;

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
	is_bound = true;
}

template<typename type>
inline void ConstantBuffer<type>::Unbind()
{
	if (!is_bound)
		return;

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
	is_bound = false;
}
