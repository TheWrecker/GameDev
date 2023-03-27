
#include "directxtk/DDSTextureLoader.h"
#include "directxtk/WICTextureLoader.h"

#include "util_funcs.h"
#include "../presenter.h"

#include "texture.h"

Texture::Texture(ID3D11Device* device, const std::wstring& file)
	:texture(), shader_view(), desc(), device(device)
{
	//TODO: postpone loading? / separate creation from loading?
	auto extension = file.substr(file.size() - 3, 3);
	if (extension == L"dds")
	{
		DXAssert(DirectX::CreateDDSTextureFromFile(device, file.c_str(), (ID3D11Resource**)&texture, &shader_view));
	}
	else
	{
		DXAssert(DirectX::CreateWICTextureFromFile(device, file.c_str(), (ID3D11Resource**)&texture, &shader_view));
	}
	texture->GetDesc(&desc);
}

Texture::~Texture()
{
	DXRelease(texture);
	DXRelease(shader_view);
}

const ID3D11Texture2D* Texture::GetTexture() const
{
	return texture;
}

ID3D11ShaderResourceView* Texture::GetShaderView()
{
	return shader_view;
}

const D3D11_TEXTURE2D_DESC& Texture::GetDesc() const
{
	return desc;
}
