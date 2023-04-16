
#include "../elements/shader_vertex.h"
#include "../elements/shader_pixel.h"
#include "../elements/input_layout.h"
//#include "../elements/depth_map.h"
#include "depth_pass.h"
#include "../scene/assets/master_buffer.h"
#include "../entities/entity_transformable.h"
#include "../scene/elements/renderable_frustrum.h"
#include "../scene/camera/camera_basic.h"
#include "../scene/scene.h"
#include "../presenter.h"

#include "render_dev.h"

DevRender::DevRender(Presenter* parent)
	:RenderBase(parent)
{
	object = std::make_unique<TransformableEntity>();
	object->SetPosition(20.0f, 27.0f, 20.0f);
	object->SetScale(5.0f, 5.0f, 5.0f);

	plane = std::make_unique<TransformableEntity>();
	plane->SetPosition(20.0f, 27.0f, 10.0f);
	plane->SetScale(20.0f, 20.0f, 20.0f);

	object_buffer = std::make_unique<ConstantBuffer<DefaultConstantStruct>>(device, context);
	dev_buffer = std::make_unique<ConstantBuffer<DevData>>(device, context);

	vertex_shader = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/developement.hlsl", "vs_main");
	vertex_shader2 = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/developement.hlsl", "depth_pass_main");
	pixel_shader = std::make_unique<PixelShader>(presenter, L"source/visuals/shaders/developement.hlsl", "ps_main");

	unsigned int _slot1 = static_cast<unsigned int>(DefaultObjects::SPHERE_NORMAL);
	unsigned int _slot2 = static_cast<unsigned int>(DefaultObjects::QUAD_NORMAL);

	input_layout = std::make_unique<InputLayout>(presenter, vertex_shader.get());
	input_layout->AddElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, _slot1)
		.AddElement("TEXCOORDS", DXGI_FORMAT_R32G32_FLOAT, _slot1)
		.AddElement("NORMALS", DXGI_FORMAT_R32G32B32_FLOAT, _slot1)
		.Build();

	input_layout_plane = std::make_unique<InputLayout>(presenter, vertex_shader.get());
	input_layout_plane->AddElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, _slot2)
		.AddElement("TEXCOORDS", DXGI_FORMAT_R32G32_FLOAT, _slot2)
		.AddElement("NORMALS", DXGI_FORMAT_R32G32B32_FLOAT, _slot2)
		.Build();

	input_layout_depth = std::make_unique<InputLayout>(presenter, vertex_shader2.get());
	input_layout_depth->AddElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, _slot1)
		.AddElement("TEXCOORDS", DXGI_FORMAT_R32G32_FLOAT, _slot1)
		.Build();

	depth_pass = std::make_unique<DepthPass>(parent, 500, 500);

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

bool DevRender::Initialize()
{
	bool _result = true;

	_result &= RenderBase::Initialize();
	_result &= depth_pass->Initialize();

	return _result;
}

void DevRender::Render()
{
	dev_data.camera_position = camera->Position();
	dev_data.projector_matrix = DirectX::XMMatrixTranspose(scene->renderable_frustrum->projector.View_Projection_Matrix());
	dev_buffer->Update(dev_data);
	dev_buffer->Bind(BindStage::VERTEX, 2);
	dev_buffer->Bind(BindStage::PIXEL, 2);

	DefaultConstantStruct _cb1 = { DirectX::XMMatrixTranspose(object->World_Matrix()) };
	object_buffer->Update(_cb1);
	object_buffer->Bind(BindStage::VERTEX, 1);

	input_layout_depth->Bind();
	vertex_shader2->Apply();

	depth_pass->Clear();
	depth_pass->BindAsStencilTarget();

	ID3D11PixelShader* _null_ps = { nullptr };

	context->PSSetShader(_null_ps, nullptr, 0);
	buffer_master->BindDefaultIndexBuffer(DefaultObjects::SPHERE);
	context->DrawIndexed(buffer_master->GetIndexCount(DefaultObjects::SPHERE), 0, 0);

	//TODO: add render target stack class

	presenter->BindDefaultRenderTargetAndStencil();

	dev_data.projector_matrix = DirectX::XMMatrixTranspose(scene->renderable_frustrum->projector.View_Projection_Screen_Matrix());
	dev_buffer->Update(dev_data);

	DefaultConstantStruct _cb2 = { DirectX::XMMatrixTranspose(plane->World_Matrix()) };
	object_buffer->Update(_cb2);
	object_buffer->Bind(BindStage::VERTEX, 1);

	input_layout_plane->Bind();
	vertex_shader->Apply();
	pixel_shader->Apply();
	depth_pass->BindAsShaderResource();

	buffer_master->BindDefaultObject(DefaultObjects::QUAD_NORMAL);
	buffer_master->BindDefaultIndexBuffer(DefaultObjects::QUAD_NORMAL);
	context->DrawIndexed(buffer_master->GetIndexCount(DefaultObjects::QUAD_NORMAL), 0, 0);

	_cb1 = { DirectX::XMMatrixTranspose(object->World_Matrix()) };
	object_buffer->Update(_cb1);
	object_buffer->Bind(BindStage::VERTEX, 1);

	input_layout->Bind();

	buffer_master->BindDefaultIndexBuffer(DefaultObjects::SPHERE_NORMAL);
	context->DrawIndexed(buffer_master->GetIndexCount(DefaultObjects::SPHERE_NORMAL), 0, 0);
}
