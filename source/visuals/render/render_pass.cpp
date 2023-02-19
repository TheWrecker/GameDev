
#include "../elements/render_target.h"
#include "../scene.h"

#include "render_pass.h"

constexpr unsigned int SLOT = D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT - 1;

RenderPass::RenderPass(Scene* scene, const std::wstring& pixelShader)
	:RenderBase(scene)
{
	render_target = std::make_unique<RenderTarget>(scene->GetPresenter());
	vertex_shader = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/pass_v.hlsl");
	pixel_shader = std::make_unique<PixelShader>(presenter, pixelShader);

	unsigned int _quad_slot = static_cast<unsigned int>(DefaultObjects::QUAD);
	input_layout = std::make_unique<InputLayout>(presenter, vertex_shader.get());
	input_layout->AddElement("POSITION", DXGI_FORMAT_R32G32B32A32_FLOAT, _quad_slot)
		.AddElement("TEXCOORDS", DXGI_FORMAT_R32G32_FLOAT, _quad_slot)
		.Build();
}

RenderPass::~RenderPass()
{
}

void RenderPass::BindAsRenderTarget()
{
	render_target->Bind();
}

void RenderPass::Render()
{
	input_layout->Bind();
	vertex_shader->Apply();
	pixel_shader->Apply();
	//TODO: slot system for SRVs? or move to aggregator?
	context->PSSetShaderResources(SLOT, 1, render_target->GetPointerToShaderView());
	context->DrawIndexed(buffer_master->GetIndexCount(DefaultObjects::QUAD), 0, 0);
}

RenderTarget* RenderPass::GetRenderTarget()
{
	return render_target.get();
}
