
#pragma once

#include <d3d11.h>
#include <string>
#include <vector>

class VertexShader;
class Presenter;

class InputLayout
{
public:
	InputLayout(Presenter* presenter, VertexShader* shader);
	~InputLayout();

	InputLayout& AddElement(const std::string& semantic, DXGI_FORMAT format, unsigned int slot = 0);
	InputLayout& AddElement(D3D11_INPUT_ELEMENT_DESC& target);
	bool Build();
	void Bind();
	void Unbind();
	void Clear();

private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	VertexShader* shader;
	std::vector<D3D11_INPUT_ELEMENT_DESC> container;
	ID3D11InputLayout* input_layout;
};

