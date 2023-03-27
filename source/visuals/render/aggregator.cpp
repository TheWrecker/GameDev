
#include "../scene/assets/master_state.h"
#include "../scene/assets/manager_texture.h"
#include "../scene/assets/texture_atlas.h"
#include "../scene/assets/master_buffer.h"
#include "../elements/render_target.h"
#include "hud.h"
#include "highlight.h"
#include "solid_blocks.h"
#include "render_dev.h"
#include "sun_moon.h"
#include "sky.h"
#include "render_pass.h"
#include "../scene/scene.h"
#include "../presenter.h"

#include "aggregator.h"

static const ::DirectX::XMVECTORF32 RENDER_TARGET_DEFAULT_COLOR = { 0.0f, 0.0f, 0.0f, 1.0f };

Aggregator::Aggregator(Presenter* parent)
	:presenter(parent)
{
	device = presenter->device;
	context = presenter->context;
	//TODO: move to scene change

	//Sky 
	render_sky = std::make_unique<SkyRender>(parent);

	//Dev Render
	render_dev = std::make_unique<DevRender>(parent);

	//render pass
	render_pass = std::make_unique<RenderPass>(parent, L"source/visuals/shaders/pass_p.hlsl");

	//Sun and Moon
	render_sun_moon = std::make_unique<SunMoon>(parent);

	//solid blocks
	render_solid_blocks = std::make_unique<SolidBlockRender>(parent);

	//block highlight
	render_highlight = std::make_unique<HighlightRender>(parent);

	//hud
	render_hud = std::make_unique<HUDRender>(parent);
}

Aggregator::~Aggregator()
{
}

bool Aggregator::Initialize()
{
	scene = presenter->QueryService<Scene*>("scene");

	if (!scene)
		return false;

	state_master = scene->GetStateMaster();
	model_manager = scene->GetModelManager();
	texture_manager = scene->GetTextureManager();
	buffer_master = scene->GetBufferMaster();

	bool _result = true;
	//initialize renderers
	_result &= render_pass->Initialize();
	_result &= render_sky->Initialize();
	_result &= render_dev->Initialize();
	_result &= render_highlight->Initialize();
	_result &= render_sun_moon->Initialize();
	_result &= render_solid_blocks->Initialize();
	_result &= render_hud->Initialize();

	//setup all render parameters
	//bind default objects
	buffer_master->BindDefaultObject(DefaultObjects::QUAD);
	buffer_master->BindDefaultObject(DefaultObjects::QUAD_FULLSCREEN);
	buffer_master->BindDefaultObject(DefaultObjects::QUAD_NORMAL);
	buffer_master->BindDefaultObject(DefaultObjects::BLOCK);
	buffer_master->BindDefaultObject(DefaultObjects::BLOCK_NORMAL);
	buffer_master->BindDefaultObject(DefaultObjects::SPHERE);
	buffer_master->BindDefaultObject(DefaultObjects::SPHERE_NORMAL);

	//bind default constants
	buffer_master->BindDefaultConstant(DefaultConstants::HUD_MATRIX);
	buffer_master->BindDefaultConstant(DefaultConstants::SUN_LIGHT_DATA);
	buffer_master->BindDefaultConstant(DefaultConstants::VIEW_PROJECTION_MATRIX);

	//bind  default samplers 
	state_master->BindDefaultTextureSampler(DefaultSampler::BILINEAR);

	//bind default textures and the atlas
	scene->GetTextureAtlas()->Bind();
	texture_manager->BindDefaultTextures();

	//setup states
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	presenter->SetRasterizerState(RasterizerMode::CULL_BACK_SOLID_CW);
	presenter->SetBlendMode(BlendMode::DISABLED);

	return _result;
}

void Aggregator::AggregateAllRenders()
{
	//hotpath
	
	//TODO: change based on scene mode
	//TODO: replace hardcoded order with name lookup/std::set/std::deque?

	//clear rendertargets
	context->ClearRenderTargetView(presenter->render_target_view, reinterpret_cast<const float*>(&RENDER_TARGET_DEFAULT_COLOR));
	context->ClearDepthStencilView(presenter->depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//context->ClearRenderTargetView(render_pass->GetRenderTarget()->GetTargetView(), reinterpret_cast<const float*>(&RENDER_TARGET_DEFAULT_COLOR));
	//context->ClearDepthStencilView(render_pass->GetRenderTarget()->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//update constants
	buffer_master->UpdateDefaultConstant(DefaultConstants::VIEW_PROJECTION_MATRIX);
	buffer_master->UpdateDefaultConstant(DefaultConstants::SUN_LIGHT_DATA);
	//we only update hud matrix when there is a change in hud parameters

	//bind render pass
	//render_pass->BindAsRenderTarget();

	//render sky
	presenter->SetRasterizerState(RasterizerMode::CULL_FRONT_SOLID_CW);
	buffer_master->BindDefaultIndexBuffer(DefaultObjects::SPHERE);
	render_sky->Render();
	presenter->SetRasterizerState(RasterizerMode::CULL_BACK_SOLID_CW);

	//render sun or moon
	//TODO: dynamic time of day
	//buffer_master->BindDefaultIndexBuffer(DefaultObjects::SPHERE);
	render_sun_moon->Render();

	//render dev
	buffer_master->BindDefaultIndexBuffer(DefaultObjects::BLOCK_NORMAL);
	render_dev->Render();

	//render solid blocks
	render_solid_blocks->Update();
	render_solid_blocks->Render();

	//enable alpha blending
	presenter->SetBlendMode(BlendMode::ENABLED);

	//render block highlights
	buffer_master->BindDefaultIndexBuffer(DefaultObjects::BLOCK);
	render_highlight->Render();

	//render hud
	buffer_master->BindDefaultIndexBuffer(DefaultObjects::QUAD);
	render_hud->Render();

	//disable alpha blending
	presenter->SetBlendMode(BlendMode::DISABLED);


	//render the pass
	/*buffer_master->BindDefaultIndexBuffer(DefaultObjects::QUAD_FULLSCREEN);
	context->OMSetRenderTargets(1, &presenter->render_target_view, presenter->depth_stencil_view);
	render_pass->Render();*/

	//restore defaults
}
