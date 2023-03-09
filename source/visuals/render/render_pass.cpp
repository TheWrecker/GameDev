
#include "defs_pipeline.h"
#include "../elements/render_target.h"
#include "../scene.h"

#include "render_pass.h"

RenderPass::RenderPass(Scene* scene, const std::wstring& pixelShader)
	:RenderBase(scene)
{
	render_target = std::make_unique<RenderTarget>(scene->GetPresenter());
	vertex_shader = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/pass_v.hlsl");
	pixel_shader = std::make_unique<PixelShader>(presenter, pixelShader);

	unsigned int _slot = static_cast<unsigned int>(DefaultObjects::QUAD_FULLSCREEN);
	input_layout = std::make_unique<InputLayout>(presenter, vertex_shader.get());
	input_layout->AddElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, _slot)
		.AddElement("TEXCOORDS", DXGI_FORMAT_R32G32_FLOAT, _slot)
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
	context->PSSetShaderResources(static_cast<unsigned int>(DefaultTextures::FIRST_PASS), 1, render_target->GetPointerToShaderView());
	context->DrawIndexed(buffer_master->GetIndexCount(DefaultObjects::QUAD_FULLSCREEN), 0, 0);
}

RenderTarget* RenderPass::GetRenderTarget()
{
	return render_target.get();
}
