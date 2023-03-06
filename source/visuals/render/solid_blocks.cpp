
#include "../scene.h"

#include "solid_blocks.h"

SolidBlockRender::SolidBlockRender(Scene* scene)
	:RenderBase(scene)
{
	vertex_shader = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/solid_blocks_v.hlsl");
	pixel_shader = std::make_unique<PixelShader>(presenter, L"source/visuals/shaders/solid_blocks_p.hlsl");

	unsigned int _slot = 1;
	input_layout = std::make_unique<InputLayout>(presenter, vertex_shader.get());
	input_layout->AddElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, _slot)
		.AddElement("TEXCOORDS", DXGI_FORMAT_R32G32_FLOAT, _slot)
		.AddElement("NORMALS", DXGI_FORMAT_R32G32B32_FLOAT, _slot)
		.AddElement("ATLAS_SLICE", DXGI_FORMAT_R32_FLOAT, _slot)
		.Build();

	per_object_buffer = std::make_unique<ConstantBuffer<DefaultConstantStruct>>(device, context);
}

SolidBlockRender::~SolidBlockRender()
{
}

void SolidBlockRender::Render()
{
	DefaultConstantStruct _cb1 = { DirectX::XMMatrixTranspose(scene->segment1->World_Matrix()) };
	per_object_buffer->Update(_cb1);
	per_object_buffer->Bind(BindStage::VERTEX, 1);
	input_layout->Bind();
	vertex_shader->Apply();
	pixel_shader->Apply();
	scene->GetTextureAtlas()->Bind();
	scene->segment1->GetVertexBuffer()->Bind(1);
	scene->segment1->GetIndexBuffer()->Bind();
	context->DrawIndexed(scene->segment1->GetIndexBuffer()->GetIndexCount(), 0, 0);

	_cb1 = { DirectX::XMMatrixTranspose(scene->segment2->World_Matrix()) };
	per_object_buffer->Update(_cb1);
	per_object_buffer->Bind(BindStage::VERTEX, 1);

	scene->segment2->GetVertexBuffer()->Bind(1);
	scene->segment2->GetIndexBuffer()->Bind();
	context->DrawIndexed(scene->segment2->GetIndexBuffer()->GetIndexCount(), 0, 0);
}
