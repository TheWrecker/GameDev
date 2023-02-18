
#include "../entities/camera_basic.h"
#include "../scene.h"

#include "sky.h"

SkyRender::SkyRender(Scene* scene)
	:RenderBase(scene)
{
	vertex_shader = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/sky_v.hlsl");
	pixel_shader = std::make_unique<PixelShader>(presenter, L"source/visuals/shaders/sky_p.hlsl");

	unsigned int _sphere_slot = static_cast<unsigned int>(DefaultObjects::SPHERE);
	input_layout = std::make_unique<InputLayout>(presenter, vertex_shader.get());
	input_layout->AddElement("POSITION", DXGI_FORMAT_R32G32B32A32_FLOAT, _sphere_slot)
		.AddElement("TEXCOORDS", DXGI_FORMAT_R32G32_FLOAT, _sphere_slot)
		.Build();

	per_frame_buffer = std::make_unique<ConstantBuffer<PerFrameBuffer>>(device, context);
}

SkyRender::~SkyRender()
{
}

void SkyRender::Render()
{
	DirectX::XMFLOAT3 _currentPosition = scene->GetActiveCamera()->Position();
	DirectX::XMMATRIX _worldMatrix = DirectX::XMMatrixScaling(500.0f, 500.0f, 500.0f) * DirectX::XMMatrixTranslation(_currentPosition.x, _currentPosition.y, _currentPosition.z);
	PerFrameBuffer _cb1 = { DirectX::XMMatrixTranspose(_worldMatrix)};
	per_frame_buffer->Update(_cb1);

	auto _shaderView = texture_manager->GetShaderView("sky");
	context->PSSetShaderResources(0, 1, &_shaderView);
	buffer_master->BindDefaultConstant(DefaultConstants::VIEW_PROJECTION_MATRIX);
	buffer_master->BindDefaultObject(DefaultObjects::SPHERE);
	state_master->BindDefaultTextureSampler(DefaultSampler::BILINEAR, 0);
	buffer_master->UpdateDefaultConstant(DefaultConstants::VIEW_PROJECTION_MATRIX);
	per_frame_buffer->Bind(BindStage::VERTEX, 1);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	input_layout->Bind();
	vertex_shader->Apply();
	pixel_shader->Apply();

	context->DrawIndexed(buffer_master->GetIndexCount(DefaultObjects::SPHERE), 0, 0);
}
