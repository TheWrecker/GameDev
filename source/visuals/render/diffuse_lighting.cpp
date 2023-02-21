#include "diffuse_lighting.h"

DiffuseLighting::DiffuseLighting(Scene* scene)
	:RenderBase(scene)
{
	object = std::make_unique<BasicEntity>();
	object->SetPosition(4.0f, 0.0f, 0.0f);

	object_buffer = std::make_unique<ConstantBuffer<PerObjectBuffer>>(device, context);

	vertex_shader = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/diffuse_lighting_v.hlsl");
	pixel_shader = std::make_unique<PixelShader>(presenter, L"source/visuals/shaders/diffuse_lighting_p.hlsl");

	unsigned int _slot = static_cast<unsigned int>(DefaultObjects::SPHERE_NORMAL);
	input_layout = std::make_unique<InputLayout>(presenter, vertex_shader.get());
	input_layout->AddElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, _slot)
		.AddElement("TEXCOORDS", DXGI_FORMAT_R32G32_FLOAT, _slot)
		.AddElement("NORMALS", DXGI_FORMAT_R32G32B32_FLOAT, _slot)
		.Build();

}

DiffuseLighting::~DiffuseLighting()
{
}

void DiffuseLighting::Render()
{
	PerObjectBuffer _cb2 = { DirectX::XMMatrixTranspose(object->World_Matrix()) };
	object_buffer->Update(_cb2);
	object_buffer->Bind(BindStage::VERTEX, 1);

	input_layout->Bind();
	vertex_shader->Apply();
	pixel_shader->Apply();

	context->DrawIndexed(buffer_master->GetIndexCount(DefaultObjects::SPHERE_NORMAL), 0, 0);
}
