
#include "../elements/shader_vertex.h"
#include "../elements/shader_pixel.h"
#include "../elements/input_layout.h"
#include "../scene/assets/master_buffer.h"
#include "../scene/elements/sun.h"
#include "../scene/scene.h"

#include "sun_moon.h"

SunMoon::SunMoon(Presenter* parent)
	:RenderBase(parent)
{
	vertex_shader = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/sun_moon_v.hlsl");
	pixel_shader = std::make_unique<PixelShader>(presenter, L"source/visuals/shaders/sun_moon_p.hlsl");

	unsigned int _sphere_slot = static_cast<unsigned int>(DefaultObjects::SPHERE);
	input_layout = std::make_unique<InputLayout>(presenter, vertex_shader.get());
	input_layout->AddElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, _sphere_slot)
		.AddElement("TEXCOORDS", DXGI_FORMAT_R32G32_FLOAT, _sphere_slot)
		.Build();

	per_frame_buffer = std::make_unique<ConstantBuffer<DefaultConstantStruct>>(device, context);
}

SunMoon::~SunMoon()
{
}

bool SunMoon::Initialize()
{
	bool _result = RenderBase::Initialize();

	sun = scene->GetSun();

	if (!sun)
		return false;

	return _result;
}

void SunMoon::Render()
{
	DefaultConstantStruct _cb1 = { DirectX::XMMatrixTranspose(sun->World_Matrix()) };
	per_frame_buffer->Update(_cb1);
	per_frame_buffer->Bind(BindStage::VERTEX, 1);
	input_layout->Bind();
	vertex_shader->Apply();
	pixel_shader->Apply();

	context->DrawIndexed(buffer_master->GetIndexCount(DefaultObjects::SPHERE), 0, 0);
}
