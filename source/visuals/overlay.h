
#pragma once

#include <d3d11.h>

#include "interface_drawable.h"
#include"platfrom_defs.h"

class Overlay : public IDrawable
{
public:
	Overlay(ID3D11Device* device, ID3D11DeviceContext* context, SysWindowHandle windowHandle);
	~Overlay();

	void Show();
	void Hide();
	void Draw() override;
	void Update() override;

private:
	bool show;
};