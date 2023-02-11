
#pragma once

#include <d3d11.h>

#include "interface_shader.h"

class Presenter;

class VertexShader : public IShader
{
public:
	VertexShader(Presenter* presenter, const std::wstring& file);
	~VertexShader();

	void Apply() override;

private:
	ID3D11VertexShader* shader;
};
