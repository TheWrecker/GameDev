
#ifndef PRESENTER_H
	#define PRESENTER_H

	#include <d3d11_1.h>
	#include <functional>
	#include <string>

	#include "interface_service.h"
	#include "../core/supervisor.h"

	enum class MultiSamplingType
	{
		NONE,
		MSAA
	};

	enum class RasterizerMode
	{
		CULL_NONE_SOLID_CW,
		CULL_NONE_SOLID_CCW,
		CULL_NONE_WIREFRAME_CW,
		CULL_NONE_WIREFRAME_CCW,
		CULL_BACK_SOLID_CW,
		CULL_BACK_SOLID_CCW,
		CULL_BACK_WIREFRAME_CW,
		CULL_BACK_WIREFRAME_CCW,
		CULL_FRONT_SOLID_CW,
		CULL_FRONT_SOLID_CCW,
		CULL_FRONT_WIREFRAME_CW,
		CULL_FRONT_WIREFRAME_CCW
	};

	enum class BlendMode
	{
		DISABLED,
		ENABLED
	};

	class Overlay;
	class Aggregator;

	class Presenter : public IService
	{
	public:
		Presenter(Supervisor* parent, unsigned int width, unsigned int height);
		~Presenter();

		bool Initialize() override;
		void Update() override;
		void Draw();
		void Present();

		void ToggleFullscreen();
		void BindDefaultRenderTargetAndStencil();
		bool SetMultiSampling(MultiSamplingType type, UINT count, UINT quality);
		bool SetDepthStencil(bool state, D3D11_TEXTURE2D_DESC* desc);
		bool SetRasterizerState(RasterizerMode);
		void SetBlendMode(BlendMode mode);

		Supervisor* GetSupervisor();
		Overlay* GetOverlay();
		Aggregator* GetAggregator();
		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetContext();
		unsigned int GetScreenWidth();
		unsigned int GetScreenHeight();

		template <typename type>
		type QueryService(const std::string& name);
	
	private:
		friend Aggregator;

		bool CreateSwapChain(bool isResize);
		bool CreateViewPort();
		void CreateRasterizerStates();
		void CreateBlendStates();

		Supervisor* supervisor;

		std::unique_ptr<Overlay> overlay;
		std::unique_ptr<Aggregator> aggregator;

		bool isFullscreen;
		unsigned int
			screen_width,
			screen_height;

		//d3d parameters
		//adapters
		ID3D11Device* device;
		ID3D11DeviceContext* context;
		IDXGIDevice* graphics_interface;
		IDXGIAdapter* graphics_adapter;
		IDXGIFactory2* graphics_factory;

		//swapchain
		IDXGISwapChain1* swapchain;
		DXGI_SWAP_CHAIN_DESC1 swapchain_desc;
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC swapchain_fullscreen_desc;

		//backbuffer and depthstencil
		ID3D11Texture2D* back_buffer,*depth_stencil;
		D3D11_TEXTURE2D_DESC back_buffer_desc,depth_stencil_desc;
		ID3D11DepthStencilView* depth_stencil_view;

		//rendertarget
		ID3D11RenderTargetView* render_target_view;
		D3D11_VIEWPORT view_port;

		//rasterizer
		ID3D11RasterizerState
			* rasterizer_state_NSCW,
			* rasterizer_state_NSCCW,
			* rasterizer_state_NWCW,
			* rasterizer_state_NWCCW,
			* rasterizer_state_BSCW,
			* rasterizer_state_BSCCW,
			* rasterizer_state_BWCW,
			* rasterizer_state_BWCCW,
			* rasterizer_state_FSCW,
			* rasterizer_state_FSCCW,
			* rasterizer_state_FWCW,
			* rasterizer_state_FWCCW;

		//pipeline states
		bool depth_stencil_enabled, multisampling_enabled, blend_enabled;

		//multisampling
		MultiSamplingType multisampling_type;
		UINT multisampling_count, multisampling_quality;

		//blend
		ID3D11BlendState *blend_state_enabled, *blend_state_disabled;
	};

	template<typename type>
	inline type Presenter::QueryService(const std::string& name)
	{
		return supervisor->GetService<type>(name);
	}

#endif // !PRESENTER_H
