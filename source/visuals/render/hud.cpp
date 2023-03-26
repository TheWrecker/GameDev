
#include "../elements/shader_vertex.h"
#include "../elements/shader_pixel.h"
#include "../elements/input_layout.h"
#include "../scene/assets/master_buffer.h"
#include "../entities/player.h"
#include "../scene/scene.h"
#include "../presenter.h"

#include "hud.h"

HUDRender::HUDRender(Presenter* parent)
	:RenderBase(parent)
{
	vertex_shader = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/hud_v.hlsl");
	pixel_shader = std::make_unique<PixelShader>(presenter, L"source/visuals/shaders/hud_p.hlsl");

	unsigned int _slot = static_cast<unsigned int>(DefaultObjects::QUAD);
	input_layout = std::make_unique<InputLayout>(presenter, vertex_shader.get());
	input_layout->AddElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, _slot)
		.AddElement("TEXCOORDS", DXGI_FORMAT_R32G32_FLOAT, _slot)
		.Build();
}

HUDRender::~HUDRender()
{
}

void HUDRender::Render()
{
	input_layout->Bind();
	vertex_shader->Apply();
	pixel_shader->Apply();

	context->DrawIndexed(buffer_master->GetIndexCount(DefaultObjects::QUAD), 0, 0);
}
