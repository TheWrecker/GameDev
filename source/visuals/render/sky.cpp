
#include "../elements/shader_vertex.h"
#include "../elements/shader_pixel.h"
#include "../elements/input_layout.h"
#include "../scene/assets/master_buffer.h"
#include "../scene/camera/camera_basic.h"
#include "../scene/scene.h"

#include "sky.h"

SkyRender::SkyRender(Presenter* parent)
	:RenderBase(parent)
{
	vertex_shader = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/sky_v.hlsl");
	pixel_shader = std::make_unique<PixelShader>(presenter, L"source/visuals/shaders/sky_p.hlsl");

	unsigned int _sphere_slot = static_cast<unsigned int>(DefaultObjects::SPHERE);
	input_layout = std::make_unique<InputLayout>(presenter, vertex_shader.get());
	input_layout->AddElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, _sphere_slot)
		.AddElement("TEXCOORDS", DXGI_FORMAT_R32G32_FLOAT, _sphere_slot)
		.Build();

	per_frame_buffer = std::make_unique<ConstantBuffer<DefaultConstantStruct>>(device, context);
	scale_matrix = DirectX::XMMatrixScaling(190.0f, 190.0f, 190.0f);
}

SkyRender::~SkyRender()
{
}

void SkyRender::Render()
{
	DirectX::XMFLOAT3 _currentPosition = camera->Position();
	DirectX::XMMATRIX _worldMatrix = scale_matrix * DirectX::XMMatrixTranslation(_currentPosition.x, _currentPosition.y, _currentPosition.z);
	DefaultConstantStruct _cb1 = { DirectX::XMMatrixTranspose(_worldMatrix)};
	per_frame_buffer->Update(_cb1);
	per_frame_buffer->Bind(BindStage::VERTEX, 1);
	input_layout->Bind();
	vertex_shader->Apply();
	pixel_shader->Apply();

	context->DrawIndexed(buffer_master->GetIndexCount(DefaultObjects::SPHERE), 0, 0);
}
