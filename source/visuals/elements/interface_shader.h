
#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>

enum class ShaderType
{
	UNDEFINED,
	VERTEX,
	PIXEL
};

class IShader
{
public:
	IShader(ID3D11Device* device, ID3D11DeviceContext* context);
	virtual ~IShader();

	virtual bool LoadShader(ShaderType type, const std::wstring& file);
	virtual void Apply() = 0;

protected:
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	ID3DBlob* blob;
	void* byte_code;
	std::size_t byte_code_size;
	ShaderType type;
};
