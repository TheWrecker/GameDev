
#pragma once

#include <d3d11.h>

#include "interface_shader.h"

class Presenter;

class PixelShader : public IShader
{
public:
	PixelShader(Presenter* presenter, const std::wstring& file);
	~PixelShader();

	void Apply() override;

private:
	ID3D11PixelShader* shader;
};
