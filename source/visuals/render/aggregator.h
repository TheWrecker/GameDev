
#ifndef AGGREGATOR_H
	#define AGGREGATOR_H

	#include <d3d11.h>
	#include <memory>

	class HUDRender;
	class HighlightRender;
	class ProxyRender;
	class SolidBlockRender;
	class DevRender;
	class SunMoon;
	class SkyRender;
	class RenderPass;
	class StateMaster;
	class ModelManager;
	class TextureManager;
	class BufferMaster;
	class Presenter;
	class Scene;

	class Aggregator
	{
	public:
		Aggregator(Presenter* parent);
		~Aggregator();

		bool Initialize();
		void AggregateAllRenders();

	private:
		friend class Overlay;
		friend Scene;

		ID3D11Device* device;
		ID3D11DeviceContext* context;
		Scene* scene;
		Presenter* presenter;
		StateMaster* state_master;
		ModelManager* model_manager;
		TextureManager* texture_manager;
		BufferMaster* buffer_master;

		std::unique_ptr<RenderPass> render_pass;
		std::unique_ptr<DevRender> render_dev;
		std::unique_ptr<SkyRender> render_sky;
		std::unique_ptr<SunMoon> render_sun_moon;
		std::unique_ptr<SolidBlockRender> render_solid_blocks;
		std::unique_ptr<ProxyRender> render_proxies;
		std::unique_ptr<HighlightRender> render_highlight;
		std::unique_ptr<HUDRender> render_hud;
	};

#endif // !AGGREGATOR_H
