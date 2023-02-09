
#pragma once

#include <d3d11.h>

#include "interface_shader.h"

class VertexShader : public IShader
{
public:
	VertexShader(ID3D11Device* device, ID3D11DeviceContext* context);
	~VertexShader();

	bool LoadShader(ShaderType type,const std::wstring& file) override;
	void Apply() override;

private:
	ID3D11VertexShader* shader;
};
