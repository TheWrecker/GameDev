
#pragma once

#include <d3d11.h>

#include "shader_interface.h"

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
