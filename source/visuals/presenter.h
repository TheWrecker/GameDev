
#pragma once

#include <d3d11_1.h>
#include <functional>

#include "interface_drawable.h"
#include "interface_service.h"
#include "overlay.h"
#include "scene.h"
#include "../core/service_manager.h"
#include "../core/supervisor.h"

enum class MultiSamplingType
{
	NONE,
	MSAA
};

enum class CullMode
{
	CULL_NONE,
	CULL_BACK,
	CULL_FRONT
};

enum class RenderMode
{
	SINGLE_PASS_NO_STENCIL,
	SINGLE_PASS_WITH_STENCIL,
	MULTI_PASS
};

class Presenter : public IService, public IDrawable
{
public:
	Presenter(Supervisor* parent);
	~Presenter();

	void Draw() override;
	void Update() override;
	void ToggleFullscreen();
	bool SetMultiSampling(MultiSamplingType type, UINT count, UINT quality);
	bool SetDepthStencil(bool state, D3D11_TEXTURE2D_DESC* desc);
	bool SetRasterizerState(CullMode cullmodle, bool wireframe, bool frontCCW);
	bool SetRenderMode(RenderMode mode);

	Supervisor* GetSupervisor();
	Scene* GetActiveScene();
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();

	template <typename type>
	type QueryService(const std::string& name);
	
private:
	bool CreateSwapChain(bool isResize);
	bool CreateViewPort();
	void Func_ClearAndRenderWithStencil();
	void Func_ClearAndRenderNoStencil();
	void Func_OnlyRenderDeferClear();

	std::unique_ptr<Scene> scene;
	std::unique_ptr<Overlay> overlay;
	std::function<void()> render_function;
    Supervisor* supervisor;
	bool isFullscreen;

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
	ID3D11RasterizerState* rasterizer_state;
	D3D11_RASTERIZER_DESC rasterizer_state_desc;

	//pipeline states
	bool depth_stencil_enabled, multisampling_enabled;

	//multisampling
	MultiSamplingType multisampling_type;
	UINT multisampling_count, multisampling_quality;
};

template<typename type>
inline type Presenter::QueryService(const std::string& name)
{
	return dynamic_cast<type>(supervisor->Services()->GetService(name));
}
