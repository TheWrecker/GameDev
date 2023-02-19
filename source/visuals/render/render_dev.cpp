
#include "render_dev.h"

DevRender::DevRender(Scene* scene)
	:RenderBase(scene)
{
	test_object = std::make_unique<BasicEntity>();
	vertex_shader = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/dev_v.hlsl");
	pixel_shader = std::make_unique<PixelShader>(presenter, L"source/visuals/shaders/dev_p.hlsl");

	unsigned int _sphere_slot = static_cast<unsigned int>(DefaultObjects::BLOCK);
	input_layout = std::make_unique<InputLayout>(presenter, vertex_shader.get());
	input_layout->AddElement("POSITION", DXGI_FORMAT_R32G32B32A32_FLOAT, _sphere_slot)
		.AddElement("TEXCOORDS", DXGI_FORMAT_R32G32_FLOAT, _sphere_slot)
		.Build();

	per_object_buffer = std::make_unique<ConstantBuffer<PerObjectBuffer>>(device, context);
}

DevRender::~DevRender()
{
}

void DevRender::Render()
{
	PerObjectBuffer _cb1 = { DirectX::XMMatrixTranspose(test_object->World_Matrix()) };
	per_object_buffer->Update(_cb1);
	per_object_buffer->Bind(BindStage::VERTEX, 1);
	input_layout->Bind();
	vertex_shader->Apply();
	pixel_shader->Apply();

	context->DrawIndexed(buffer_master->GetIndexCount(DefaultObjects::BLOCK), 0, 0);
}
