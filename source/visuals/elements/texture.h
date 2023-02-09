
#pragma once

#include <string>
#include <d3d11.h>

class Presenter;

class Texture
{
public:
	Texture(Presenter* presenter, const std::wstring& file, const std::string& name = "");
	~Texture();

	const ID3D11Texture2D* GetTexture() const;
	const ID3D11ShaderResourceView* GetShaderView() const;
	const D3D11_TEXTURE2D_DESC& GetDesc() const;
	const std::string& GetName() const;

private:
	ID3D11Texture2D* texture;
	D3D11_TEXTURE2D_DESC desc;
	ID3D11ShaderResourceView* shader_view;
	std::string name;
};
