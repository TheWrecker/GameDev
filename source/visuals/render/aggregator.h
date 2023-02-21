
#pragma once

#include <d3d11.h>
#include <memory>

#include "render_base.h"

class DiffuseLighting;
class DevRender;
class SkyRender;
class RenderPass;
class Scene;
class Presenter;

class Aggregator
{
public:
	Aggregator(Scene* scene);
	~Aggregator();

	void AggregateAllRenders();

private:
	friend class Overlay;

	ID3D11Device* device;
	ID3D11DeviceContext* context;
	Scene* scene;
	Presenter* presenter;
	StateMaster* state_master;
	ModelManager* model_manager;
	TextureManager* texture_manager;
	BufferMaster* buffer_master;

	std::unique_ptr<RenderPass> render_pass;
	std::unique_ptr<DevRender> render_dev;
	std::unique_ptr<SkyRender> render_sky;
	std::unique_ptr< DiffuseLighting> render_lighting_diffuse;
};