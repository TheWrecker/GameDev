
#ifndef HIGHLIGHT_H
	#define HIGHLIGHT_H

	#include <d3d11.h>
	#include <memory>

	#include "../entities/block_solid.h"
	#include "render_base.h"

	class HighlightRender : public RenderBase
	{
	public:
		HighlightRender(Scene* scene);
		~HighlightRender();

		void Render() override;

	private:
		std::unique_ptr<ConstantBuffer<DefaultConstantStruct>> object_buffer;
		DirectX::XMMATRIX transformation_matrix;
	};

#endif // !HIGHLIGHT_H
