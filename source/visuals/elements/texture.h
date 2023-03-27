
#ifndef TEXTURE_H
	#define TEXTURE_H

	#include <string>
	#include <d3d11.h>

	class Presenter;

	class Texture
	{
	public:
		Texture(ID3D11Device* device, const std::wstring& file);
		~Texture();

		const ID3D11Texture2D* GetTexture() const;
		ID3D11ShaderResourceView* GetShaderView();
		const D3D11_TEXTURE2D_DESC& GetDesc() const;

	private:
		ID3D11Device* device;
		ID3D11Texture2D* texture;
		D3D11_TEXTURE2D_DESC desc;
		ID3D11ShaderResourceView* shader_view;
	};

#endif // !TEXTURE_H
