
#include "util_funcs.h"

#include "buffer_index.h"

IndexBuffer::IndexBuffer(ID3D11Device* device, ID3D11DeviceContext* context, std::size_t reserve)
	:device(device), context(context), buffer(), desc(), subresource()
{
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ZeroMemory(&subresource, sizeof(subresource));
	if (reserve > 0)
		indices.reserve(reserve);
}

IndexBuffer::~IndexBuffer()
{
	DXRelease(buffer);
}

void IndexBuffer::AddIndex(unsigned int index)
{
	indices.push_back(index);
}

void IndexBuffer::Clear()
{
	indices.clear();
}

void IndexBuffer::Build()
{
	if (indices.size() > 0)
	{
		subresource.pSysMem = &indices.at(0);
		desc.ByteWidth = sizeof(unsigned int) * indices.size();
		DXAssert(device->CreateBuffer(&desc, &subresource, &buffer));
	}
}

void IndexBuffer::Bind()
{
	context->IASetIndexBuffer(buffer, DXGI_FORMAT_R32_UINT, 0);
}

void IndexBuffer::Unbind()
{
	ID3D11Buffer* nullBuffer = { nullptr };
	context->IASetIndexBuffer(nullBuffer, DXGI_FORMAT_R32_UINT, 0);
}

std::size_t IndexBuffer::GetIndexCount()
{
	return indices.size();
}
