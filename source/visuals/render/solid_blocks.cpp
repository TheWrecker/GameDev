
#include "../elements/shader_vertex.h"
#include "../elements/shader_pixel.h"
#include "../elements/input_layout.h"
#include "../elements/render_target.h"
#include "../elements/depth_map.h"
#include "../elements/buffer_vertex.h"
#include "../scene/assets/master_buffer.h"
#include "../scene/elements/renderable_frustrum.h"
#include "../scene/camera/projector.h"
#include "../scene/compartments/segment.h"
#include "../scene/world.h"
#include "../scene/elements/sun.h"
#include "../scene/scene.h"
#include "vision_perimeter.h"
#include "../presenter.h"

#include "solid_blocks.h"

static const ::DirectX::XMVECTORF32 RENDER_TARGET_DEFAULT_COLOR = { 1.0f, 1.0f, 1.0f, 1.0f };

SolidBlockRender::SolidBlockRender(Presenter* parent)
	:RenderBase(parent), render_shadows(true), show_depth_map(true)
{
	vertex_shader_depth = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/solid_blocks.hlsl", "vs_first_pass");
	pixel_shader_depth = std::make_unique<PixelShader>(presenter, L"source/visuals/shaders/solid_blocks.hlsl", "ps_first_pass");
	vertex_shader = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/solid_blocks.hlsl", "vs_second_pass");
	pixel_shader = std::make_unique<PixelShader>(presenter, L"source/visuals/shaders/solid_blocks.hlsl", "ps_second_pass");

	texture_overlay_vertex_shader = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/texture_overlay.hlsl", "vs_main");
	texture_overlay_pixel_shader = std::make_unique<PixelShader>(presenter, L"source/visuals/shaders/texture_overlay.hlsl", "ps_main");

	unsigned int _slot1 = 1;
	unsigned int _slot2 = static_cast<unsigned int>(DefaultObjects::QUAD);

	input_layout = std::make_unique<InputLayout>(presenter, vertex_shader.get());
	input_layout->AddElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, _slot1)
		.AddElement("TEXCOORDS", DXGI_FORMAT_R32G32_FLOAT, _slot1)
		.AddElement("NORMALS", DXGI_FORMAT_R32G32B32_FLOAT, _slot1)
		.AddElement("ATLAS_SLICE", DXGI_FORMAT_R32_FLOAT, _slot1)
		.Build();

	texture_overlay_input_layout = std::make_unique<InputLayout>(presenter, texture_overlay_vertex_shader.get());
	texture_overlay_input_layout->AddElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, _slot2)
		.AddElement("TEXCOORDS", DXGI_FORMAT_R32G32_FLOAT, _slot2)
		.Build();

	per_object_buffer = std::make_unique<ConstantBuffer<DefaultConstantStruct>>(device, context);
	sun_direction_buffer = std::make_unique<ConstantBuffer<DefaultConstantStruct>>(device, context);

	texture_overlay_buffer = std::make_unique<ConstantBuffer<DefaultConstantStruct>>(device, context);
	auto _scale_matrix = DirectX::XMMatrixScaling(0.5f, 0.5f, 1.0f);
	DirectX::XMFLOAT2 _texture_position = { -1.0f, -1.0f };
	auto _translation_matrix = DirectX::XMMatrixTranslation(_texture_position.x, _texture_position.y, 0.0f);
	auto _final_matrix = DirectX::XMMatrixMultiply(_scale_matrix, _translation_matrix);
	DefaultConstantStruct _cb = { DirectX::XMMatrixTranspose(_final_matrix) };
	texture_overlay_buffer->Update(_cb);
	texture_overlay_buffer->Bind(BindStage::VERTEX, 1);

	depth_map = std::make_unique<DepthMap>(parent, 4096, 3072);

	render_target = std::make_unique<RenderTarget>(parent, 4096, 3072);
	render_target->CreateInterfaces();

	bias_buffer = std::make_unique<ConstantBuffer<DefaultConstantStruct>>(device, context);

	DirectX::XMMATRIX _bias_mat = {
		DirectX::XMVECTOR{ +0.5f, +0.0f, +0.0f, +0.0f },
		DirectX::XMVECTOR{ +0.0f, +0.5f, +0.0f, +0.0f },
		DirectX::XMVECTOR{ +0.5f, +0.0f, +0.5f, +0.0f },
		DirectX::XMVECTOR{ +0.5f, +0.5f, +0.0f, +1.0f }
	};

	_cb = { DirectX::XMMatrixTranspose(_bias_mat) };
	bias_buffer->Update(_cb);
	bias_buffer->Bind(BindStage::VERTEX, 3);

	light_buffer = std::make_unique<ConstantBuffer<DefaultConstantStruct>>(device, context);

	_cb.matrix.r[0] = { -.5f, -0.5f, -0.5f, 1.0f };
	_cb.matrix.r[1] = { 1.0f, 1.0f, 1.0f, 1.0f }; //light
	_cb.matrix.r[2] = { 0.5f, 0.5f, 0.5f, 1.0f }; //ambient
	_cb.matrix.r[3] = { 0.0f, 0.0f, 0.0f, 0.0f };

	light_buffer->Update(_cb);
}

SolidBlockRender::~SolidBlockRender()
{
}

bool SolidBlockRender::Initialize()
{
	bool _result = RenderBase::Initialize();

	perimeter = std::make_unique<VisionPerimeter>(presenter);
	if (!perimeter.get())
		return false;

	render_segments.reserve(500);
	_result &= perimeter->Initialize();

	return _result;
}

void SolidBlockRender::Update()
{
	render_segments.clear();
	perimeter->CollectVisionPerimeter(render_segments);
}

void SolidBlockRender::Render()
{

	if (render_segments.empty())
		return;
	if (!render_shadows)
	{
		vertex_shader->Apply();
		pixel_shader->Apply();
		input_layout->Bind();

		RenderSegments();
	}
	else
	{
		ID3D11ShaderResourceView* _null_view = { nullptr };
		ID3D11PixelShader* _null_ps = { nullptr };

		//depth pass
		auto& _pos = scene->renderable_frustrum->projector.Position_Vector();
		auto _dir = scene->renderable_frustrum->projector.Direction_Vector();
		auto _up = scene->renderable_frustrum->projector.Up_Vector();

		//create light orthographic projection
		//auto _matWV = DirectX::XMMatrixMultiply(scene->renderable_frustrum->World_Matrix(),	DirectX::XMMatrixLookToRH(_pos, _dir, _up));
		auto _matWV = DirectX::XMMatrixLookToRH(_pos, _dir, _up);
		auto _matWVP = DirectX::XMMatrixMultiply(_matWV, DirectX::XMMatrixOrthographicRH(160, 120, 50, 200.0f));
		//auto _matWVP = DirectX::XMMatrixMultiply(_matWV, scene->renderable_frustrum->projector.Projection_Matrix());

		DefaultConstantStruct _cb = { DirectX::XMMatrixTranspose(_matWVP) };
		sun_direction_buffer->Update(_cb);
		sun_direction_buffer->Bind(BindStage::VERTEX, 2);
		bias_buffer->Bind(BindStage::VERTEX, 3);

		//TODO: move to sun
		_cb = { scene->GetSun()->GetLightInfo() };
		_cb.matrix.r[0] = DirectX::XMVectorNegate(scene->renderable_frustrum->projector.Direction_Vector());

		light_buffer->Update(_cb);
		light_buffer->Bind(BindStage::PIXEL, 4);

		//apply depth states
		input_layout->Bind();
		vertex_shader_depth->Apply();
		//presenter->SetRasterizerState(RasterizerMode::CULL_FRONT_SOLID_CW);

		//setup render target
		if (use_render_target_as_depth_map)
		{
			context->ClearRenderTargetView(render_target->GetTargetView(), reinterpret_cast<const float*>(&RENDER_TARGET_DEFAULT_COLOR));
			context->ClearDepthStencilView(render_target->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			render_target->Bind();
			pixel_shader_depth->Apply();
		}
		else
		{
			depth_map->Clear();
			depth_map->BindAsStencilTarget();
			context->PSSetShader(_null_ps, nullptr, 0);

			//presenter->SetRasterizerState(RasterizerMode::DEPTH_MAP);
		}

		//render segments from light's orthographic view
		RenderSegments();

		//bind default render targets
		//TODO: add render target stack class
		presenter->BindDefaultRenderTargetAndStencil();

		//bind render targets as resources
		if (use_render_target_as_depth_map)
		{
			context->PSSetShaderResources(2, 1, render_target->GetPointerToShaderView());
			context->PSSetShaderResources(3, 1, render_target->GetPointerToShaderView());
		}
		else
		{
			depth_map->BindAsShaderResource(2);
			depth_map->BindAsShaderResource(3);
		}

		//apply proper render states
		vertex_shader->Apply();
		pixel_shader->Apply();
		presenter->SetRasterizerState(RasterizerMode::CULL_BACK_SOLID_CW);

		//render segments
		RenderSegments();

		//render the depth map
		if (show_depth_map)
		{
			texture_overlay_buffer->Bind(BindStage::VERTEX, 1);
			texture_overlay_input_layout->Bind();
			texture_overlay_vertex_shader->Apply();
			texture_overlay_pixel_shader->Apply();
			buffer_master->BindDefaultObject(DefaultObjects::QUAD);
			buffer_master->BindDefaultIndexBuffer(DefaultObjects::QUAD);
			context->DrawIndexed(buffer_master->GetIndexCount(DefaultObjects::QUAD), 0, 0);
		}

		//TODO: cleanup
		context->PSSetShaderResources(2, 1, &_null_view);
		context->PSSetShaderResources(3, 1, &_null_view);
	}
}

void SolidBlockRender::RenderSegments()
{
	for (auto _segment : render_segments)
	{
		if (!_segment)
			continue;

		if (_segment->vertex_buffer.load()->GetVertexCount() == 0)
			continue;

		_segment->draw_mutex.lock();

		DefaultConstantStruct _cb = { DirectX::XMMatrixTranspose(_segment->World_Matrix()) };
		per_object_buffer->Update(_cb);
		per_object_buffer->Bind(BindStage::VERTEX, 1);
		_segment->GetVertexBuffer()->Bind(1);
		_segment->GetIndexBuffer()->Bind();
		context->DrawIndexed(_segment->GetIndexBuffer()->GetIndexCount(), 0, 0);

		_segment->draw_mutex.unlock();
	}
}
