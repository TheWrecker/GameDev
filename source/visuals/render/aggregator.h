
#pragma once

#include <d3d11.h>
#include <deque>

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
	std::deque<RenderBase*> renderers;
};