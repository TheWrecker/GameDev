
#include "render_dev.h"
#include "sky.h"
#include "../scene.h"
#include "../presenter.h"

#include "aggregator.h"

static const ::DirectX::XMVECTORF32 RENDER_TARGET_DEFAULT_COLOR = { 0.0f, 0.0f, 0.0f, 1.0f };

Aggregator::Aggregator(Scene* scene)
	:scene(scene)
{
	presenter = scene->GetPresenter();
	device = presenter->device;
	context = presenter->context;

	//Sky
	RenderBase* _sky = new SkyRender(scene);
	renderers.push_back(_sky);
	//Dev Render
	RenderBase* _dev = new DevRender(scene);
	renderers.push_back(_dev);
}

Aggregator::~Aggregator()
{
}

void Aggregator::AggregateAllRenders()
{
	context->ClearRenderTargetView(presenter->render_target_view, reinterpret_cast<const float*>(&RENDER_TARGET_DEFAULT_COLOR));
	context->ClearDepthStencilView(presenter->depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	for (auto _renderer : renderers)
		_renderer->Render();
}
