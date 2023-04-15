
#ifndef DEPTH_MAP_H
	#define DEPTH_MAP_H

	#include <d3d11.h>

	class Presenter;

	class DepthMap
	{
	public:
		DepthMap(Presenter* presenter, unsigned int width, unsigned int height);
		~DepthMap();

		void Bind();

		unsigned int GetWidth() const;
		unsigned int GetHeight() const;
		const D3D11_VIEWPORT& Viewport() const;
		ID3D11ShaderResourceView* GetShaderView() const;
		ID3D11ShaderResourceView** GetPointerToShaderView();
		ID3D11DepthStencilView* GetDepthStencilView() const;

	private:
		friend class DepthPass;

		ID3D11Device* device;
		ID3D11DeviceContext* context;
		Presenter* presenter;

		ID3D11Texture2D* texture;
		D3D11_TEXTURE2D_DESC texture_desc;
		ID3D11ShaderResourceView* shader_view;

		D3D11_DEPTH_STENCIL_VIEW_DESC stencil_desc;
		ID3D11DepthStencilView* stencil_view;

		D3D11_VIEWPORT viewport;
		D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;

	};

#endif // !DEPTH_MAP_H
