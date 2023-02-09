
#pragma once

#include <d3d11.h>

#include "interface_shader.h"

class PixelShader : public IShader
{
public:
	PixelShader(ID3D11Device* device, ID3D11DeviceContext* context);
	~PixelShader();

	bool LoadShader(ShaderType type,const std::wstring& file) override;
	void Apply() override;

private:
	ID3D11PixelShader* shader;
};
