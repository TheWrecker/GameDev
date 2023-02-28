#pragma once

#include <d3d11.h>
#include <vector>

class IndexBuffer
{
public:
	IndexBuffer(ID3D11Device* device, ID3D11DeviceContext* context, std::size_t reserve = 0);
	~IndexBuffer();

	void AddIndex(unsigned int index);
	void Clear();
	void Build();
	void Bind();
	void Unbind();

	ID3D11Buffer* GetBuffer();
	D3D11_BUFFER_DESC GetDesc();
	unsigned int GetIndexCount();

private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	std::vector<unsigned int> indices;
	ID3D11Buffer* buffer;
	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA subresource;
};