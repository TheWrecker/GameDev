
#ifndef HIGHLIGHT_H
	#define HIGHLIGHT_H

	#include <memory>

	#include "../elements/buffer_constant.h"
	#include "render_base.h"

	class Player;

	class HighlightRender : public RenderBase
	{
	public:
		HighlightRender(Presenter* parent);
		~HighlightRender();

		bool Initialize() override;
		void Render() override;

	private:
		Player* player;

		std::unique_ptr<ConstantBuffer<DefaultConstantStruct>> object_buffer;
		DirectX::XMMATRIX transformation_matrix;
	};

#endif // !HIGHLIGHT_H
