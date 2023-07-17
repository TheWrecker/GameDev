
#ifndef RENDER_TARGET_H
	#define RENDER_TARGET_H

	#include <d3d11.h>

	class Presenter;

	class RenderTarget
	{
	public:
		RenderTarget(Presenter* presenter);
		~RenderTarget();

		bool CreateInterfaces();
		void Bind();

		ID3D11RenderTargetView* GetTargetView();
		ID3D11DepthStencilView* GetDepthStencilView();
		ID3D11ShaderResourceView* GetShaderView();
		ID3D11ShaderResourceView** GetPointerToShaderView();

	protected:
		ID3D11Device* device;
		ID3D11DeviceContext* context;
		Presenter* presenter;

		ID3D11Texture2D* back_buffer;
		D3D11_TEXTURE2D_DESC back_buffer_desc;
		ID3D11ShaderResourceView* back_buffer_view;
		ID3D11RenderTargetView* back_buffer_target_view;

		ID3D11Texture2D* depth_stencil;
		D3D11_TEXTURE2D_DESC depth_stencil_desc;
		ID3D11DepthStencilView* depth_stencil_view;

		D3D11_VIEWPORT viewport;
	};

#endif // !RENDER_TARGET_H
