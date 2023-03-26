
#include "../elements/shader_vertex.h"
#include "../elements/shader_pixel.h"
#include "../elements/input_layout.h"
#include "../scene/assets/master_buffer.h"
#include "../scene/compartments/segment.h"
#include "../scene/compartments/pillar.h"
#include "../scene/world.h"
#include "../scene/scene.h"
#include "vision_perimeter.h"

#include "solid_blocks.h"

SolidBlockRender::SolidBlockRender(Presenter* parent)
	:RenderBase(parent)
{
	vertex_shader = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/solid_blocks_v.hlsl");
	pixel_shader = std::make_unique<PixelShader>(presenter, L"source/visuals/shaders/solid_blocks_p.hlsl");

	unsigned int _slot = 1;
	input_layout = std::make_unique<InputLayout>(presenter, vertex_shader.get());
	input_layout->AddElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, _slot)
		.AddElement("TEXCOORDS", DXGI_FORMAT_R32G32_FLOAT, _slot)
		.AddElement("NORMALS", DXGI_FORMAT_R32G32B32_FLOAT, _slot)
		.AddElement("ATLAS_SLICE", DXGI_FORMAT_R32_FLOAT, _slot)
		.Build();

	per_object_buffer = std::make_unique<ConstantBuffer<DefaultConstantStruct>>(device, context);
}

SolidBlockRender::~SolidBlockRender()
{
}

bool SolidBlockRender::Initialize()
{
	bool _result = RenderBase::Initialize();;

	perimeter = std::make_unique<VisionPerimeter>(scene);
	if (!perimeter.get())
		return false;

	render_segments.reserve(500);

	return _result;
}

void SolidBlockRender::Update()
{
	render_segments.clear();
	perimeter->CollectVisionPerimeter(render_segments);
}

void SolidBlockRender::Render()
{
	vertex_shader->Apply();
	pixel_shader->Apply();
	input_layout->Bind();

	for (auto _segment : render_segments)
	{
		if (_segment->IsEmpty())
			continue;

		DefaultConstantStruct _cb = { DirectX::XMMatrixTranspose(_segment->World_Matrix()) };
		per_object_buffer->Update(_cb);
		per_object_buffer->Bind(BindStage::VERTEX, 1);
		_segment->GetVertexBuffer()->Bind(1);
		_segment->GetIndexBuffer()->Bind();
		context->DrawIndexed(_segment->GetIndexBuffer()->GetIndexCount(), 0, 0);
	}
}
