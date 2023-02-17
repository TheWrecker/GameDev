
#include <d3dcompiler.h>

#include "../elements/shader_pixel.h"
#include "../elements/shader_vertex.h"
#include "../entities/camera_basic.h"
#include "../elements/input_layout.h"
#include "../elements/buffer_index.h"
#include "../elements/master_state.h"
#include "../elements/master_buffer.h"
#include "../scene.h"
#include "../presenter.h"
#include "util_funcs.h"

#include "test.h"

TestRender::TestRender(Presenter* presenter, Scene* scene)
	:scene(scene), presenter(presenter)
{
	device = presenter->GetDevice();
	context = presenter->GetContext();
	state_master = scene->GetStateMaster();
	buffer_master = scene->GetBufferMaster();
	texture_manager = scene->GetTextureManager();


	test_object = std::make_unique<BasicEntity>();
	vertex_shader = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/test_v.hlsl");
	pixel_shader = std::make_unique<PixelShader>(presenter, L"source/visuals/shaders/test_p.hlsl");
	unsigned int _sphere_slot = static_cast<unsigned int>(DefaultObjects::SPHERE);

	input_layout = std::make_unique<InputLayout>(presenter, vertex_shader.get());
	input_layout->AddElement("POSITION", DXGI_FORMAT_R32G32B32A32_FLOAT, _sphere_slot)
		.AddElement("TEXCOORDS", DXGI_FORMAT_R32G32_FLOAT, _sphere_slot)
		.Build();

	auto _shaderView = texture_manager->GetShaderView("earth");
	context->PSSetShaderResources(0, 1, &_shaderView);
	buffer_master->BindDefaultConstant(DefaultConstants::VIEW_PROJECTION_MATRIX);
	buffer_master->BindDefaultObject(DefaultObjects::SPHERE);
	//setup constant buffers
	per_object_buffer = std::make_unique<ConstantBuffer<PerObjectBuffer>>(device, context);
}

TestRender::~TestRender()
{
}

void TestRender::Draw()
{
	PerObjectBuffer _cb1 = { DirectX::XMMatrixTranspose(test_object->World_Matrix()) };

	per_object_buffer->Update(_cb1);

	buffer_master->UpdateDefaultConstant(DefaultConstants::VIEW_PROJECTION_MATRIX);
	per_object_buffer->Bind(BindStage::VERTEX, 1);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	input_layout->Bind();
	vertex_shader->Apply();
	pixel_shader->Apply();
	state_master->BindTextureSampler(DefaultSampler::BILINEAR, 0);

	context->DrawIndexed(buffer_master->GetIndexCount(DefaultObjects::SPHERE), 0, 0);
}
