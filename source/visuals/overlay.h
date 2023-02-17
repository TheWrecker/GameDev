
#pragma once

#include <d3d11.h>

#include "interface_drawable.h"
#include"defs_platform.h"

class Presenter;
class SystemTicker;

class Overlay : public IDrawable
{
public:
	Overlay(Presenter* parent, ID3D11Device* device, ID3D11DeviceContext* context);
	~Overlay();

	void Show();
	void Hide();
	void Draw() override;
	void Update() override;

private:
	Presenter* presenter;
	SystemTicker* ticker;
	bool show;
};