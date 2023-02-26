
#include "../elements/render_target.h"
#include "diffuse_lighting.h"
#include "render_dev.h"
#include "sun_moon.h"
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

	//Sky 
	render_sky = std::make_unique<SkyRender>(scene);

	//Dev Render
	render_dev = std::make_unique<DevRender>(scene);

	//render pass
	render_pass = std::make_unique<RenderPass>(scene, L"source/visuals/shaders/pass_p.hlsl");

	//diffuse lighting
	render_lighting_diffuse = std::make_unique<DiffuseLighting>(scene);

	//Sun and Moon
	render_sun_moon = std::make_unique<SunMoon>(scene);
}

Aggregator::~Aggregator()
{
}

void Aggregator::AggregateAllRenders()
{
	//hotpath
	
	//TODO: change based on scene mode
	//TODO: replace hardcoded order with name lookup/std::set/std::deque?

	//clear rendertargets
	context->ClearRenderTargetView(presenter->render_target_view, reinterpret_cast<const float*>(&RENDER_TARGET_DEFAULT_COLOR));
	context->ClearDepthStencilView(presenter->depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->ClearRenderTargetView(render_pass->GetRenderTarget()->GetTargetView(), reinterpret_cast<const float*>(&RENDER_TARGET_DEFAULT_COLOR));
	context->ClearDepthStencilView(render_pass->GetRenderTarget()->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//update constants
	//view projection
	buffer_master->UpdateDefaultConstant(DefaultConstants::VIEW_PROJECTION_MATRIX);
	buffer_master->BindDefaultConstant(DefaultConstants::VIEW_PROJECTION_MATRIX);
	buffer_master->UpdateDefaultConstant(DefaultConstants::SUN_LIGHT_DATA);
	buffer_master->BindDefaultConstant(DefaultConstants::SUN_LIGHT_DATA);

	//TODO: bind  default samplers to predewfined slots and change shaders accordingly
	//setup states
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	state_master->BindDefaultTextureSampler(DefaultSampler::BILINEAR);

	//bind render pass
	//render_pass->BindAsRenderTarget();

	//render sky
	auto _shaderView = texture_manager->GetShaderView("sky");
	context->PSSetShaderResources(0, 1, &_shaderView);
	buffer_master->BindDefaultObject(DefaultObjects::SPHERE);
	render_sky->Render();

	//render sun or moon
	//TODO: dynamic time of day
	_shaderView = texture_manager->GetShaderView("sun");
	context->PSSetShaderResources(0, 1, &_shaderView);
	buffer_master->BindDefaultObject(DefaultObjects::SPHERE);
	render_sun_moon->Render();

	//render dev
	_shaderView = texture_manager->GetShaderView("test_checkers");
	context->PSSetShaderResources(0, 1, &_shaderView);
	buffer_master->BindDefaultObject(DefaultObjects::BLOCK);
	render_dev->Render();

	//render diffuse lighting
	_shaderView = texture_manager->GetShaderView("earth");
	context->PSSetShaderResources(0, 1, &_shaderView);
	buffer_master->BindDefaultObject(DefaultObjects::QUAD_NORMAL);
	render_lighting_diffuse->Render();

	//render the pass
	/*buffer_master->BindDefaultObject(DefaultObjects::QUAD);
	context->OMSetRenderTargets(1, &presenter->render_target_view, presenter->depth_stencil_view);
	render_pass->Render();*/

	//restore defaults
}
