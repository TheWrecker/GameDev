
#ifndef SUN_MOON_H
	#define SUN_MOON_H

	#include "../elements/buffer_constant.h"
	#include "render_base.h"

	class Sun;
	class Presenter;

	class SunMoon : public RenderBase
	{
	public:
		SunMoon(Presenter* parent);
		~SunMoon();

		bool Initialize() override;
		void Render() override;

	private:
		Sun* sun;
		std::unique_ptr<ConstantBuffer<DefaultConstantStruct>> per_frame_buffer;
	};

#endif // !SUN_MOON_H
