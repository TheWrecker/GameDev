
#pragma once

#include <d3d11.h>

#include "interface_drawable.h"
#include"defs_platform.h"

class Sun;
class Aggregator;
class Scene;
class Presenter;
class SystemTicker;

class Overlay : public IDrawable
{
public:
	Overlay(Presenter* parent);
	~Overlay();

	void Show();
	void Hide();
	void Draw() override;
	void Update() override;

private:
	Sun* sun;
	Aggregator* aggregator;
	Scene* scene;
	Presenter* presenter;
	SystemTicker* ticker;
	bool show;
};