
#include "util_funcs.h"
#include "../presenter.h"

#include "depth_map.h"

DepthMap::DepthMap(Presenter* presenter, unsigned int width, unsigned int height)
    :presenter(presenter)
{
    device = presenter->GetDevice();
    context = presenter->GetContext();

    ZeroMemory(&texture_desc, sizeof(texture_desc));
    texture_desc.Width = width;
    texture_desc.Height = height;
    texture_desc.MipLevels = 1;
    texture_desc.ArraySize = 1;
    //texture_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    texture_desc.Format = DXGI_FORMAT_R32_TYPELESS;
    texture_desc.SampleDesc.Count = 1;
    texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    DXAssert(device->CreateTexture2D(&texture_desc, 0, &texture));

    ZeroMemory(&view_desc, sizeof(view_desc));
    //view_desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    view_desc.Format = DXGI_FORMAT_R32_FLOAT;
    view_desc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
    view_desc.Texture2D.MipLevels = 1;
    DXAssert(device->CreateShaderResourceView(texture, &view_desc, &shader_view));

    ZeroMemory(&stencil_desc, sizeof(stencil_desc));
    //stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    stencil_desc.Format = DXGI_FORMAT_D32_FLOAT;
    stencil_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    stencil_desc.Texture2D.MipSlice = 0;
    DXAssert(device->CreateDepthStencilView(texture, &stencil_desc, &stencil_view));

    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
}

DepthMap::~DepthMap()
{
    DXRelease(texture);
    DXRelease(shader_view);
    DXRelease(stencil_view);
}

void DepthMap::Bind()
{
    ID3D11RenderTargetView* _null_render_target = nullptr;
    context->OMSetRenderTargets(1, &_null_render_target, stencil_view);
    context->RSSetViewports(1, &viewport);
}

unsigned int DepthMap::GetWidth() const
{
    return (unsigned int)viewport.Width;
}

unsigned int DepthMap::GetHeight() const
{
    return (unsigned int)viewport.Height;
}

const D3D11_VIEWPORT& DepthMap::Viewport() const
{
    return viewport;
}

ID3D11ShaderResourceView* DepthMap::GetShaderView() const
{
    return shader_view;
}

ID3D11ShaderResourceView** DepthMap::GetPointerToShaderView()
{
    return &shader_view;
}

ID3D11DepthStencilView* DepthMap::GetDepthStencilView() const
{
    return stencil_view;
}
