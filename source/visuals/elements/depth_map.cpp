
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

    ZeroMemory(&shader_view_desc, sizeof(shader_view_desc));
    //shader_view_desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    shader_view_desc.Format = DXGI_FORMAT_R32_FLOAT;
    shader_view_desc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
    shader_view_desc.Texture2D.MipLevels = 1;
    DXAssert(device->CreateShaderResourceView(texture, &shader_view_desc, &shader_view));

    ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));
    //depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depth_stencil_desc.Format = DXGI_FORMAT_D32_FLOAT;
    depth_stencil_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depth_stencil_desc.Texture2D.MipSlice = 0;
    DXAssert(device->CreateDepthStencilView(texture, &depth_stencil_desc, &depth_stencil_view));

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
    DXRelease(depth_stencil_view);
}

void DepthMap::Clear()
{
    context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DepthMap::BindAsStencilTarget()
{
    ID3D11RenderTargetView* _null_render_target = nullptr;
    context->OMSetRenderTargets(1, &_null_render_target, depth_stencil_view);
    context->RSSetViewports(1, &viewport);
}

void DepthMap::BindAsShaderResource(unsigned int slot)
{
    context->PSSetShaderResources(slot, 1, GetPointerToShaderView());
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
    return depth_stencil_view;
}
