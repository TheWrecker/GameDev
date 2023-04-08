
#include "../elements/shader_vertex.h"
#include "../elements/shader_pixel.h"
#include "../elements/input_layout.h"
#include "../scene/assets/master_buffer.h"
#include "../entities/entity_transformable.h"
#include "../scene/elements/renderable_frustrum.h"
#include "../scene/camera/camera_basic.h"
#include "../scene/scene.h"

#include "render_dev.h"

DevRender::DevRender(Presenter* parent)
	:RenderBase(parent)
{
	object = std::make_unique<TransformableEntity>();
	object->SetPosition(20.0f, 27.0f, 20.0f);
	object->SetScale(5.0f, 5.0f, 5.0f);

	object_buffer = std::make_unique<ConstantBuffer<DefaultConstantStruct>>(device, context);
	dev_buffer = std::make_unique<ConstantBuffer<DevData>>(device, context);

	vertex_shader = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/developement.hlsl", "vs_main");
	pixel_shader = std::make_unique<PixelShader>(presenter, L"source/visuals/shaders/developement.hlsl", "ps_main");

	unsigned int _slot = static_cast<unsigned int>(DefaultObjects::SPHERE_NORMAL);
	input_layout = std::make_unique<InputLayout>(presenter, vertex_shader.get());
	input_layout->AddElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, _slot)
		.AddElement("TEXCOORDS", DXGI_FORMAT_R32G32_FLOAT, _slot)
		.AddElement("NORMALS", DXGI_FORMAT_R32G32B32_FLOAT, _slot)
		.Build();

	dev_data.ambient_color = { 0.2f, 0.2f, 0.0f, 0.5f };
	dev_data.light_color = { 0.8f, 0.0f, 0.0f, 0.8f };
	dev_data.specular_color = { 1.0f, 1.0f, 1.0f, 0.5f };
	dev_data.light_radius = 40.0f;
	dev_data.specular_power = 2.0f;
	dev_data.camera_position = { 20.0f, 20.0f, 20.0f };
	dev_data.light_position = { 30.0f, 40.0f, 30.0f };
	dev_data.projector_matrix = {};
}

DevRender::~DevRender()
{
}

void DevRender::Render()
{
	DefaultConstantStruct _cb2 = { DirectX::XMMatrixTranspose(object->World_Matrix()) };
	object_buffer->Update(_cb2);
	object_buffer->Bind(BindStage::VERTEX, 1);

	auto _i = sizeof(DevData);

	dev_data.camera_position = camera->Position();
	dev_data.projector_matrix = DirectX::XMMatrixTranspose(scene->renderable_frustrum->projector.View_Projection_Screen_Matrix());
	dev_buffer->Update(dev_data);
	dev_buffer->Bind(BindStage::VERTEX, 2);
	dev_buffer->Bind(BindStage::PIXEL, 2);

	input_layout->Bind();
	vertex_shader->Apply();
	pixel_shader->Apply();

	context->DrawIndexed(buffer_master->GetIndexCount(DefaultObjects::SPHERE_NORMAL), 0, 0);
}
