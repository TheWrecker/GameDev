
#ifndef HUD_H
	#define HUD_H

	#include "render_base.h"

	class Presenter;

	class HUDRender : public RenderBase
	{
	public:
		HUDRender(Presenter* parent);
		~HUDRender();

		void Render() override;

	private:
	};

#endif // !HUD_H
