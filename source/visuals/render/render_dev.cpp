
#include "../elements/shader_vertex.h"
#include "../elements/shader_pixel.h"
#include "../elements/input_layout.h"
#include "../scene/assets/master_buffer.h"
#include "../entities/entity_transformable.h"

#include "render_dev.h"

DevRender::DevRender(Presenter* parent)
	:RenderBase(parent)
{
	object = std::make_unique<TransformableEntity>();
	object->SetPosition(0.0f, 0.0f, 0.0f);
	object->SetScale(5.0f, 5.0f, 5.0f);

	object_buffer = std::make_unique<ConstantBuffer<DefaultConstantStruct>>(device, context);

	vertex_shader = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/dev_v.hlsl");
	pixel_shader = std::make_unique<PixelShader>(presenter, L"source/visuals/shaders/dev_p.hlsl");

	unsigned int _slot = static_cast<unsigned int>(DefaultObjects::BLOCK_NORMAL);
	input_layout = std::make_unique<InputLayout>(presenter, vertex_shader.get());
	input_layout->AddElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, _slot)
		.AddElement("TEXCOORDS", DXGI_FORMAT_R32G32_FLOAT, _slot)
		.Build();
}

DevRender::~DevRender()
{
}

void DevRender::Render()
{
	DefaultConstantStruct _cb2 = { DirectX::XMMatrixTranspose(object->World_Matrix()) };
	object_buffer->Update(_cb2);
	object_buffer->Bind(BindStage::VERTEX, 1);

	input_layout->Bind();
	vertex_shader->Apply();
	pixel_shader->Apply();

	context->DrawIndexed(buffer_master->GetIndexCount(DefaultObjects::BLOCK_NORMAL), 0, 0);
}
