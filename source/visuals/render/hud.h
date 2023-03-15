
#ifndef HUD_H
	#define HUD_H

	#include "render_base.h"

	class BasicCamera;

	class HUDRender : public RenderBase
	{
	public:
		HUDRender(Scene* scene);
		~HUDRender();

		void Render() override;

	private:
	};

#endif // !HUD_H
