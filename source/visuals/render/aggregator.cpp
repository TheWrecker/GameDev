
#include "../elements/render_target.h"
#include "render_dev.h"
#include "sky.h"
#include "render_pass.h"
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
	state_master = scene->GetStateMaster();
	model_manager = scene->GetModelManager();
	texture_manager = scene->GetTextureManager();
	buffer_master = scene->GetBufferMaster();

	//TODO: move to scene change

	//Sky = 0
	//TODO:separate culling
	RenderBase* _sky = new SkyRender(scene);
	renderers.push_back(std::unique_ptr<RenderBase>(_sky));

	//Dev Render = 1
	RenderBase* _dev = new DevRender(scene);
	renderers.push_back(std::unique_ptr<RenderBase>(_dev));

	//render pass = 2
	RenderBase* _pass = new RenderPass(scene, L"source/visuals/shaders/pass_p.hlsl");
	renderers.push_back(std::unique_ptr<RenderBase>(_pass));
}

Aggregator::~Aggregator()
{
}

void Aggregator::AggregateAllRenders()
{
	//hotpath
	// 
	//TODO: change based on scene mode
	//TODO: replace hardcoded order with name lookup/std::set/std::deque?

	auto _sky = renderers[0].get();
	auto _dev = renderers[1].get();
	auto _pass = reinterpret_cast<RenderPass*>(renderers[2].get());

	//clear rendertargets
	context->ClearRenderTargetView(presenter->render_target_view, reinterpret_cast<const float*>(&RENDER_TARGET_DEFAULT_COLOR));
	context->ClearDepthStencilView(presenter->depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->ClearRenderTargetView(_pass->GetRenderTarget()->GetTargetView(), reinterpret_cast<const float*>(&RENDER_TARGET_DEFAULT_COLOR));
	context->ClearDepthStencilView(_pass->GetRenderTarget()->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//update constants
	//view projection
	buffer_master->UpdateDefaultConstant(DefaultConstants::VIEW_PROJECTION_MATRIX);
	buffer_master->BindDefaultConstant(DefaultConstants::VIEW_PROJECTION_MATRIX);

	//setup states
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	state_master->BindDefaultTextureSampler(DefaultSampler::BILINEAR, 0);

	//bind render pass
	_pass->BindAsRenderTarget();

	//render sky
	auto _shaderView = texture_manager->GetShaderView("sky");
	context->PSSetShaderResources(0, 1, &_shaderView);
	buffer_master->BindDefaultObject(DefaultObjects::SPHERE);
	_sky->Render();

	//render dev
	_shaderView = texture_manager->GetShaderView("earth");
	context->PSSetShaderResources(0, 1, &_shaderView);
	buffer_master->BindDefaultObject(DefaultObjects::BLOCK);
	_dev->Render();

	//render the pass
	buffer_master->BindDefaultObject(DefaultObjects::QUAD);
	context->OMSetRenderTargets(1, &presenter->render_target_view, presenter->depth_stencil_view);
	_pass->Render();

	//restore defaults
}
