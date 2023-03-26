
#ifndef RENDER_PASS_H
	#define RENDER_PASS_H

	#include <string>

	#include "render_base.h"

	class RenderTarget;
	class Presenter;

	class RenderPass : public RenderBase
	{
	public:
		RenderPass(Presenter* parent, const std::wstring& pixelShader);
		~RenderPass();

		void BindAsRenderTarget();
		void Render() override;

		RenderTarget* GetRenderTarget();

	private:
		std::unique_ptr<RenderTarget> render_target;
	};

#endif // !RENDER_PASS_H
