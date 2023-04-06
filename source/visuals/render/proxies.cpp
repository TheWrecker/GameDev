
#include "../elements/shader_vertex.h"
#include "../elements/shader_pixel.h"
#include "../elements/input_layout.h"
#include "../scene/assets/master_buffer.h"
#include "../scene/elements/proxy.h"

#include "proxies.h"

ProxyRender::ProxyRender(Presenter* parent)
    :RenderBase(parent), enabled(false)
{
	vertex_shader = std::make_unique<VertexShader>(presenter, L"source/visuals/shaders/proxies.hlsl", "vs_main");
	pixel_shader = std::make_unique<PixelShader>(presenter, L"source/visuals/shaders/proxies.hlsl", "ps_main");

	unsigned int _slot = 1;
	input_layout = std::make_unique<InputLayout>(presenter, vertex_shader.get());
	input_layout->AddElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT, _slot)
		.AddElement("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT, _slot)
		.Build();

	per_object_buffer = std::make_unique<ConstantBuffer<DefaultConstantStruct>>(device, context);
}

ProxyRender::~ProxyRender()
{
}

void ProxyRender::Render()
{
	if (!enabled)
		return;

	if (proxies.empty())
		return;

	vertex_shader->Apply();
	pixel_shader->Apply();
	input_layout->Bind();

	for (auto _proxy : proxies)
	{
		if (!_proxy)
			continue;

		DefaultConstantStruct _cb = { DirectX::XMMatrixTranspose(_proxy->World_Matrix()) };
		per_object_buffer->Update(_cb);
		per_object_buffer->Bind(BindStage::VERTEX, 1);
		_proxy->GetVertexBuffer()->Bind(1);
		_proxy->GetIndexBuffer()->Bind();
		context->DrawIndexed(_proxy->GetIndexBuffer()->GetIndexCount(), 0, 0);
	}
}

void ProxyRender::ShowProxies()
{
	enabled = true;
}

void ProxyRender::HideProxies()
{
	enabled = false;
}

void ProxyRender::AddProxy(Proxy* target)
{
	assert(target);
	proxies.push_back(target);
}

void ProxyRender::RemoveProxy(Proxy* target)
{
	assert(target);
	proxies.remove(target);
}
