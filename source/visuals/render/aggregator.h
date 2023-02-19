
#pragma once

#include <d3d11.h>
#include <vector>

#include "render_base.h"

class Scene;
class Presenter;

class Aggregator
{
public:
	Aggregator(Scene* scene);
	~Aggregator();

	void AggregateAllRenders();

private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	Scene* scene;
	Presenter* presenter;
	StateMaster* state_master;
	ModelManager* model_manager;
	TextureManager* texture_manager;
	BufferMaster* buffer_master;

	std::vector<std::unique_ptr<RenderBase>> renderers;
};