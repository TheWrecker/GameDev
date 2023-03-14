
#include "../entities/segment.h"
#include "../entities/world.h"
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
	vertex_shader->Apply();
	pixel_shader->Apply();
	input_layout->Bind();
	for (auto& _segment : scene->GetWorld()->segments)
	{
		if (_segment.second->IsEmpty())
			continue;

		DefaultConstantStruct _cb = { DirectX::XMMatrixTranspose(_segment.second->World_Matrix()) };
		per_object_buffer->Update(_cb);
		per_object_buffer->Bind(BindStage::VERTEX, 1);
		_segment.second->GetVertexBuffer()->Bind(1);
		_segment.second->GetIndexBuffer()->Bind();
		context->DrawIndexed(_segment.second->GetIndexBuffer()->GetIndexCount(), 0, 0);
	}
}
