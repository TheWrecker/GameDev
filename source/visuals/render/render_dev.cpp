
#include "../elements/shader_vertex.h"
#include "../elements/shader_pixel.h"
#include "../elements/input_layout.h"
#include "../elements/render_target.h"
#include "../elements/depth_map.h"
#include "../scene/assets/manager_texture.h"
#include "../scene/assets/master_buffer.h"
#include "../entities/entity_transformable.h"
#include "../scene/elements/renderable_frustrum.h"
#include "../scene/camera/camera_basic.h"
#include "../scene/scene.h"
#include "solid_blocks.h"
#include "aggregator.h"
#include "../presenter.h"

#include "render_dev.h"

static const ::DirectX::XMVECTORF32 RENDER_TARGET_DEFAULT_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };

DevRender::DevRender(Presenter* parent)
	:RenderBase(parent), use_render_target_as_depth_map(false)
{
	object = std::make_unique<TransformableEntity>();
	object->SetPosition(20.0f, 27.0f, 14.0f);
	object->SetScale(5.0f, 5.0f, 5.0f);

	plane = std::make_unique<TransformableEntity>();
	plane->SetPosition(-10.0f, 7.0f, 0.0f);
	plane->SetScale(60.0f, 45.0f, 1.0f);

	object_buffer = std::make_unique<ConstantBuffer<DefaultConstantStruct>>(device, context);
	dev_buffer = std::make_unique<ConstantBuffer<DevData>>(device, context);

	vertex_shader = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/developement.hlsl", "vs_main");
	vertex_shader2 = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/developement.hlsl", "depth_pass_main");
	pixel_shader2 = std::make_unique<PixelShader>(presenter, L"source/visuals/shaders/developement.hlsl", "depth_pass_draw");
	pixel_shader = std::make_unique<PixelShader>(presenter, L"source/visuals/shaders/developement.hlsl", "ps_main");

	texture_overlay_vertex_shader = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/texture_overlay.hlsl", "vs_main");
	texture_overlay_pixel_shader = std::make_unique<PixelShader>(presenter, L"source/visuals/shaders/texture_overlay.hlsl", "ps_main");

	unsigned int _slot1 = static_cast<unsigned int>(DefaultObjects::SPHERE_NORMAL);
	unsigned int _slot2 = static_cast<unsigned int>(DefaultObjects::QUAD_NORMAL);
	unsigned int _slot3 = static_cast<unsigned int>(DefaultObjects::QUAD);

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

	texture_overlay_input_layout = std::make_unique<InputLayout>(presenter, texture_overlay_vertex_shader.get());
	texture_overlay_input_layout->AddElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, _slot3)
		.AddElement("TEXCOORDS", DXGI_FORMAT_R32G32_FLOAT, _slot3)
		.Build();

	dev_data.ambient_color = { 0.2f, 0.2f, 0.2f, 1.0f };
	dev_data.light_color = { 0.8f, 0.8f, 0.8f, 1.0f };
	dev_data.specular_color = { 1.0f, 1.0f, 1.0f, 0.5f };
	dev_data.light_radius = 80.0f;
	dev_data.specular_power = 1.0f;
	dev_data.camera_position = { 20.0f, 20.0f, 20.0f };
	dev_data.light_position = { 30.0f, 40.0f, 30.0f };
	dev_data.projector_matrix = {};

	texture_overlay_buffer = std::make_unique<ConstantBuffer<DefaultConstantStruct>>(device, context);
	auto _scale_matrix = DirectX::XMMatrixScaling(0.5f, 0.5f, 1.0f);
	DirectX::XMFLOAT2 _texture_position = { -1.0f, -1.0f };
	auto _translation_matrix = DirectX::XMMatrixTranslation(_texture_position.x, _texture_position.y, 0.0f);
	auto _final_matrix = DirectX::XMMatrixMultiply(_scale_matrix, _translation_matrix);
	DefaultConstantStruct _cb = { DirectX::XMMatrixTranspose(_final_matrix) };
	texture_overlay_buffer->Update(_cb);
	texture_overlay_buffer->Bind(BindStage::VERTEX, 1);

	depth_map = std::make_unique<DepthMap>(parent, parent->GetScreenWidth(), parent->GetScreenHeight());

	render_target = std::make_unique<RenderTarget>(parent);
	render_target->CreateInterfaces();
}

DevRender::~DevRender()
{
}

bool DevRender::Initialize()
{
	bool _result = true;

	_result &= RenderBase::Initialize();
	//_result &= depth_pass->Initialize();

	return _result;
}

void DevRender::Render()
{
	ID3D11ShaderResourceView* _null_view = { nullptr };
	ID3D11PixelShader* _null_ps = { nullptr };

	dev_data.camera_position = camera->Position();
	dev_data.light_position = scene->renderable_frustrum->GetPosition();
	dev_data.projector_matrix = DirectX::XMMatrixTranspose(scene->renderable_frustrum->projector.View_Projection_Matrix());
	//dev_data.projector_matrix = DirectX::XMMatrixTranspose(camera->View_Projection_Matrix());
	dev_buffer->Update(dev_data);
	dev_buffer->Bind(BindStage::VERTEX, 2);
	dev_buffer->Bind(BindStage::PIXEL, 2);

	DefaultConstantStruct _cb1 = { DirectX::XMMatrixTranspose(object->World_Matrix()) };
	object_buffer->Update(_cb1);
	object_buffer->Bind(BindStage::VERTEX, 1);

	input_layout_depth->Bind();
	vertex_shader2->Apply();

	/*if (use_render_target_as_depth_map)
		pixel_shader2->Apply();*/

	if (use_render_target_as_depth_map)
	{
		context->ClearRenderTargetView(render_target->GetTargetView(), reinterpret_cast<const float*>(&RENDER_TARGET_DEFAULT_COLOR));
		context->ClearDepthStencilView(render_target->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		render_target->Bind();
		pixel_shader2->Apply();
	}
	else
	{
		depth_map->Clear();
		depth_map->BindAsStencilTarget();
		context->PSSetShader(_null_ps, nullptr, 0);

		//presenter->SetRasterizerState(RasterizerMode::DEPTH_MAP);
	}

	aggregator->render_solid_blocks->test_bool = false;

	aggregator->render_solid_blocks->Update();
	aggregator->render_solid_blocks->Render();

	aggregator->render_solid_blocks->test_bool = true;

	buffer_master->BindDefaultIndexBuffer(DefaultObjects::SPHERE);
	context->DrawIndexed(buffer_master->GetIndexCount(DefaultObjects::SPHERE), 0, 0);

	if (!use_render_target_as_depth_map)
	{
		presenter->SetRasterizerState(RasterizerMode::CULL_BACK_SOLID_CW);
	}

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

	if (use_render_target_as_depth_map)
	{
		context->PSSetShaderResources(2, 1, render_target->GetPointerToShaderView());
		context->PSSetShaderResources(3, 1, render_target->GetPointerToShaderView());
	}
	else
	{
		depth_map->BindAsShaderResource(3);
		depth_map->BindAsShaderResource(2);
	}

	buffer_master->BindDefaultObject(DefaultObjects::QUAD_NORMAL);
	buffer_master->BindDefaultIndexBuffer(DefaultObjects::QUAD_NORMAL);
	context->DrawIndexed(buffer_master->GetIndexCount(DefaultObjects::QUAD_NORMAL), 0, 0);

	_cb1 = { DirectX::XMMatrixTranspose(object->World_Matrix()) };
	object_buffer->Update(_cb1);
	object_buffer->Bind(BindStage::VERTEX, 1);

	input_layout->Bind();

	dev_data.projector_matrix = DirectX::XMMatrixTranspose(scene->renderable_frustrum->projector.View_Projection_Screen_Matrix());
	dev_buffer->Update(dev_data);

	buffer_master->BindDefaultIndexBuffer(DefaultObjects::SPHERE_NORMAL);
	context->DrawIndexed(buffer_master->GetIndexCount(DefaultObjects::SPHERE_NORMAL), 0, 0);

	texture_overlay_buffer->Bind(BindStage::VERTEX, 1);
	texture_overlay_input_layout->Bind();
	texture_overlay_vertex_shader->Apply();
	texture_overlay_pixel_shader->Apply();
	buffer_master->BindDefaultObject(DefaultObjects::QUAD);
	buffer_master->BindDefaultIndexBuffer(DefaultObjects::QUAD);
	context->DrawIndexed(buffer_master->GetIndexCount(DefaultObjects::QUAD), 0, 0);

	//context->PSSetShaderResources(2, 1, &_null_view);
	//context->PSSetShaderResources(3, 1, &_null_view);
}
