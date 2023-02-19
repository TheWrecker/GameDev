
#include "util_funcs.h"
#include "../presenter.h"

#include "render_target.h"

RenderTarget::RenderTarget(Presenter* presenter)
	:presenter(presenter)
{
	device = presenter->GetDevice();
	context = presenter->GetContext();
	//TODO: dynamic initialization
	ZeroMemory(&back_buffer_desc, sizeof(back_buffer_desc));
	back_buffer_desc.Width = presenter->GetScreenWidth();
	back_buffer_desc.Height = presenter->GetScreenHeight();
	back_buffer_desc.MipLevels = 1;
	back_buffer_desc.ArraySize = 1;
	back_buffer_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	back_buffer_desc.SampleDesc.Count = 1;
	back_buffer_desc.SampleDesc.Quality = 0;
	back_buffer_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	back_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	DXAssert(device->CreateTexture2D(&back_buffer_desc, 0, &back_buffer));
	DXAssert(device->CreateShaderResourceView(back_buffer, 0, &back_buffer_view));
	DXAssert(device->CreateRenderTargetView(back_buffer, 0, &back_buffer_target_view));

	ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));
	depth_stencil_desc.Width = back_buffer_desc.Width;
	depth_stencil_desc.Height = back_buffer_desc.Height;
	depth_stencil_desc.MipLevels = 1;
	depth_stencil_desc.ArraySize = 1;
	depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_stencil_desc.SampleDesc.Count = 1;
	depth_stencil_desc.SampleDesc.Quality = 0;
	depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
	DXAssert(device->CreateTexture2D(&depth_stencil_desc, 0, &depth_stencil));
	DXAssert(device->CreateDepthStencilView(depth_stencil, 0, &depth_stencil_view));
}

RenderTarget::~RenderTarget()
{
	DXRelease(back_buffer);
	DXRelease(back_buffer_view);
	DXRelease(back_buffer_target_view);
	DXRelease(depth_stencil);
	DXRelease(depth_stencil_view);
}

void RenderTarget::Bind()
{
	context->OMSetRenderTargets(1, &back_buffer_target_view, depth_stencil_view);
}

ID3D11RenderTargetView* RenderTarget::GetTargetView()
{
	return back_buffer_target_view;
}

ID3D11DepthStencilView* RenderTarget::GetDepthStencilView()
{
	return depth_stencil_view;
}

ID3D11ShaderResourceView* RenderTarget::GetShaderView()
{
	return back_buffer_view;
}

ID3D11ShaderResourceView** RenderTarget::GetPointerToShaderView()
{
	return &back_buffer_view;
}
