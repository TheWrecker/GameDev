
#ifndef DEPTH_PASS_H
	#define DEPTH_PASS_H

	#include <memory>

	#include "render_base.h"

	class DepthMap;
	class Presenter;

	//TODO: depth pass shouldnt be BaseRender?

	class DepthPass : public RenderBase
	{
	public:
		DepthPass(Presenter* parent, unsigned int width, unsigned int height);
		~DepthPass();

		void Render() override {};
		void Clear();
		void BindAsStencilTarget();
		void BindAsShaderResource();

		DepthMap* GetDepthMap();

	private:
		std::unique_ptr<DepthMap> depth_map;
	};

#endif // !DEPTH_PASS_H
