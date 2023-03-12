
#include "../entities/player.h"
#include "../scene.h"

#include "highlight.h"

HighlightRender::HighlightRender(Scene* scene)
	:RenderBase(scene)
{
	object_buffer = std::make_unique<ConstantBuffer<DefaultConstantStruct>>(device, context);

	vertex_shader = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/highlight_v.hlsl");
	pixel_shader = std::make_unique<PixelShader>(presenter, L"source/visuals/shaders/highlight_p.hlsl");

	unsigned int _slot = static_cast<unsigned int>(DefaultObjects::BLOCK);
	input_layout = std::make_unique<InputLayout>(presenter, vertex_shader.get());
	input_layout->AddElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, _slot)
		.AddElement("TEXCOORDS", DXGI_FORMAT_R32G32_FLOAT, _slot)
		.Build();

	transformation_matrix = DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(1.04f, 1.04f, 1.04f),
		DirectX::XMMatrixTranslation(-0.02f, -0.02f, -0.02f));
}

HighlightRender::~HighlightRender()
{
}

void HighlightRender::Render()
{
	auto _block = scene->GetPlayer()->GetInteractionBlock();
	if (!_block)
		return;
	DefaultConstantStruct _cb = { DirectX::XMMatrixMultiplyTranspose(transformation_matrix, _block->World_Matrix()) };
	object_buffer->Update(_cb);
	object_buffer->Bind(BindStage::VERTEX, 1);

	input_layout->Bind();
	vertex_shader->Apply();
	pixel_shader->Apply();

	context->DrawIndexed(buffer_master->GetIndexCount(DefaultObjects::BLOCK), 0, 0);
}
