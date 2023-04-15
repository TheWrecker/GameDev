
#include "defs_pipeline.h"
#include "../elements/depth_map.h"
#include "../elements/shader_vertex.h"
#include "../elements/shader_pixel.h"
#include "../scene/assets/master_buffer.h"
#include "../elements/input_layout.h"
#include "../elements/render_target.h"

#include "depth_pass.h"

DepthPass::DepthPass(Presenter* parent, unsigned int width, unsigned int height)
    :RenderBase(parent)
{
    depth_map = std::make_unique<DepthMap>(parent, width, height);
    vertex_shader = std::make_unique<VertexShader>(parent, L"source/visuals/shaders/depth_map.hlsl", "vs_main");
    pixel_shader = {}; //no pixel shader/no rendertargets

    unsigned int _slot = static_cast<unsigned int>(1);
    input_layout = std::make_unique<InputLayout>(presenter, vertex_shader.get());
    input_layout->AddElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, _slot)
        .Build();
}

DepthPass::~DepthPass()
{
}

void DepthPass::Clear()
{
    context->ClearDepthStencilView(depth_map->stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DepthPass::BindAsStencilTarget()
{
    depth_map->Bind();
}

void DepthPass::BindAsShaderResource()
{
    //TODO: slot as parameter?
    context->PSSetShaderResources(3, 1, depth_map->GetPointerToShaderView());
}

DepthMap* DepthPass::GetDepthMap()
{
    return depth_map.get();
}
