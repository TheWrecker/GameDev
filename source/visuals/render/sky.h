
#ifndef SKY_H
	#define SKY_H

	#include <DirectXMath.h>

	#include "../elements/buffer_constant.h"
	#include "render_base.h"

	class SkyRender : public RenderBase
	{
	public:
		SkyRender(Presenter* parent);
		~SkyRender();

		void Render() override;

	private:
		DirectX::XMMATRIX scale_matrix;

		std::unique_ptr<ConstantBuffer<DefaultConstantStruct>> per_frame_buffer;
	};

#endif // !SKY_H
