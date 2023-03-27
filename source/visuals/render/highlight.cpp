
#include "../elements/shader_vertex.h"
#include "../elements/shader_pixel.h"
#include "../elements/input_layout.h"
#include "../scene/assets/master_buffer.h"
#include "../entities/block_solid.h"
#include "../entities/player.h"
#include "../scene/scene.h"
#include "../presenter.h"

#include "highlight.h"

HighlightRender::HighlightRender(Presenter* parent)
	:RenderBase(parent)
{
	object_buffer = std::make_unique<ConstantBuffer<DefaultConstantStruct>>(device, context);

	vertex_shader = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/highlight_v.hlsl");
	pixel_shader = std::make_unique<PixelShader>(presenter, L"source/visuals/shaders/highlight_p.hlsl");

	unsigned int _slot = static_cast<unsigned int>(DefaultObjects::BLOCK);
	input_layout = std::make_unique<InputLayout>(presenter, vertex_shader.get());
	input_layout->AddElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, _slot)
		.AddElement("TEXCOORDS", DXGI_FORMAT_R32G32_FLOAT, _slot)
		.Build();

	transformation_matrix = DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(1.04f, 1.04f, 1.04f),
		DirectX::XMMatrixTranslation(-0.02f, -0.02f, -0.02f));
}

HighlightRender::~HighlightRender()
{
}

bool HighlightRender::Initialize()
{
	auto _result = RenderBase::Initialize();

	player = scene->GetPlayer();

	if (!player)
		return false;

	return _result;
}

void HighlightRender::Render()
{
	DefaultConstantStruct _cb = { };
	switch (player->GetInteractionMode())
	{
		case InteractionMode::DEFAULT: return;
			break;
		case InteractionMode::DIG_MODE:
		{
			auto _block = player->GetInteractionBlock();
			if (!_block)
				return;
			_cb = { DirectX::XMMatrixMultiplyTranspose(transformation_matrix, _block->World_Matrix()) };
			break;
		}
		case InteractionMode::PLACEMENT_MODE:
		{
			DirectX::XMFLOAT3 _temp_pos = {};
			if (!player->GetPlacementBlockPos(_temp_pos))
				return;

			_cb = { DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(_temp_pos.x , _temp_pos.y, _temp_pos.z)) };
			break;
		}
		default:
			break;
	}

	object_buffer->Update(_cb);
	object_buffer->Bind(BindStage::VERTEX, 1);

	input_layout->Bind();
	vertex_shader->Apply();
	pixel_shader->Apply();

	context->DrawIndexed(buffer_master->GetIndexCount(DefaultObjects::BLOCK), 0, 0);
}
