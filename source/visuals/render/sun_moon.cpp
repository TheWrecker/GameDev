
#include "../entities/camera_basic.h"
#include "../entities/sun.h"
#include "../scene.h"

#include "sun_moon.h"

SunMoon::SunMoon(Scene* scene)
	:RenderBase(scene)
{
	vertex_shader = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/sun_moon_v.hlsl");
	pixel_shader = std::make_unique<PixelShader>(presenter, L"source/visuals/shaders/sun_moon_p.hlsl");

	unsigned int _sphere_slot = static_cast<unsigned int>(DefaultObjects::SPHERE);
	input_layout = std::make_unique<InputLayout>(presenter, vertex_shader.get());
	input_layout->AddElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, _sphere_slot)
		.AddElement("TEXCOORDS", DXGI_FORMAT_R32G32_FLOAT, _sphere_slot)
		.Build();

	per_frame_buffer = std::make_unique<ConstantBuffer<DefaultConstantStruct>>(device, context);
	camera = scene->GetActiveCamera();
	sun = scene->GetSun();
}

SunMoon::~SunMoon()
{
}

void SunMoon::Render()
{
	//auto _pos1 = camera->Position();
	//auto _pos2 = sun->Position();
	//auto _worldMatrix = DirectX::XMMatrixTranslation(_pos1.x + _pos2.x, _pos1.y + _pos2.y, _pos1.z + _pos2.z);
	auto _worldMatrix = sun->World_Matrix();
	DefaultConstantStruct _cb1 = { DirectX::XMMatrixTranspose(_worldMatrix) };
	per_frame_buffer->Update(_cb1);
	per_frame_buffer->Bind(BindStage::VERTEX, 1);
	input_layout->Bind();
	vertex_shader->Apply();
	pixel_shader->Apply();

	context->DrawIndexed(buffer_master->GetIndexCount(DefaultObjects::SPHERE), 0, 0);
}
