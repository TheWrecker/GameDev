
#pragma once

#include <string>
#include <d3d11.h>

class IShader
{
public:
	IShader();
	virtual ~IShader();

	virtual void Apply() = 0;

	virtual std::size_t GetSize();
	virtual void* GetByteCode();

protected:
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	ID3DBlob* blob;
	void* byte_code;
	std::size_t byte_code_size;
};
