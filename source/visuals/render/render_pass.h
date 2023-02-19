
#pragma once

#include <string>

#include "render_base.h"

class RenderTarget;

class RenderPass : public RenderBase
{
public:
	RenderPass(Scene* scene, const std::wstring& pixelShader);
	~RenderPass();

	void BindAsRenderTarget();
	void Render() override;

	RenderTarget* GetRenderTarget();

private:
	std::unique_ptr<RenderTarget> render_target;
};
