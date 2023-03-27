
#include <DirectXMath.h>

#include "util_funcs.h"
#include "../core/platform.h"
#include "../core/supervisor.h"
#include "render/aggregator.h"
#include "overlay.h"

#include "presenter.h"

Presenter::Presenter(Supervisor* parent)
	:supervisor(parent), depth_stencil_enabled(true), multisampling_enabled(false), isFullscreen(false), blend_enabled(false)
{
	HRESULT result = 0;
	UINT createDeviceFlags = 0;

#if defined (debug) || defined (_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // DEBUG

	//create directx device, context, and interfaces
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL selectedFeatureLevel;
	DXAssert(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
		&device, &selectedFeatureLevel, &context));
	DXAssert(device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&graphics_interface)));
	DXAssert(graphics_interface->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&graphics_adapter)));
	DXAssert(graphics_adapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&graphics_factory)));
}

Presenter::~Presenter()
{
	DXRelease(rasterizer_state_NSCW);
	DXRelease(rasterizer_state_NSCCW);
	DXRelease(rasterizer_state_NWCW);
	DXRelease(rasterizer_state_NWCCW);
	DXRelease(rasterizer_state_BSCW);
	DXRelease(rasterizer_state_BSCCW);
	DXRelease(rasterizer_state_BWCW);
	DXRelease(rasterizer_state_BWCCW);
	DXRelease(rasterizer_state_FSCW);
	DXRelease(rasterizer_state_FSCCW);
	DXRelease(rasterizer_state_FWCW);
	DXRelease(rasterizer_state_FWCCW);
	DXRelease(blend_state_disabled);
	DXRelease(blend_state_enabled);
	DXRelease(back_buffer);
	DXRelease(render_target_view);
	DXRelease(depth_stencil);
	DXRelease(depth_stencil_view);
	DXRelease(swapchain);
	DXRelease(graphics_factory);
	DXRelease(graphics_adapter);
	DXRelease(graphics_interface);
	if (context)
	{
		context->ClearState();
		context->Flush();
	}
	DXRelease(context);
	DXRelease(device);
}

void Presenter::Draw()
{
	aggregator->AggregateAllRenders();
	overlay->Draw();
}

void Presenter::Present()
{
	swapchain->Present(0, 0);
}

bool Presenter::Initialize()
{
#ifdef _WINDOWS
	//disable user-side fullscreen switching
	DXAssert(graphics_factory->MakeWindowAssociation(QueryService<Platform*>("platform")->GetWindowHandle(), DXGI_MWA_NO_ALT_ENTER));
#endif

	RetAssert(SetMultiSampling(MultiSamplingType::NONE, 1, 0));
	CreateRasterizerStates();
	RetAssert(SetRasterizerState(RasterizerMode::CULL_NONE_SOLID_CW));
	CreateBlendStates();
	SetBlendMode(BlendMode::DISABLED);

	overlay = std::make_unique<Overlay>(this);
	aggregator = std::make_unique<Aggregator>(this);

	//show all live d3d11device objects
	/*ID3D11Debug* _debug = nullptr;
	device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&_debug));
	supervisor->SetDebugQuery(_debug);*/

	return overlay->Initialize() && aggregator->Initialize();
}

void Presenter::Update()
{
	overlay->Update();
}

void Presenter::ToggleFullscreen()
{
	SysWindowHandle windowHandle = QueryService<Platform*>("platform")->GetWindowHandle();

#ifdef _WINDOWS
	static WINDOWPLACEMENT previousWindowPlacement = { sizeof(previousWindowPlacement) };
	static DWORD dwStyle = GetWindowLong(windowHandle, GWL_STYLE);
	if (!isFullscreen)
	{
		MONITORINFO monitorInfo = {};
		monitorInfo.cbSize = sizeof(MONITORINFO);
		if (GetWindowPlacement(windowHandle, &previousWindowPlacement) &&
			GetMonitorInfo(MonitorFromWindow(windowHandle, MONITOR_DEFAULTTOPRIMARY), &monitorInfo))
		{
			SetWindowLong(windowHandle, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
			SetWindowPos(windowHandle, HWND_TOP,
				monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top,
				monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
				monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
				SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
			isFullscreen = true;
		}
	}
	else
	{
		SetWindowLong(windowHandle, GWL_STYLE, dwStyle);
		SetWindowPlacement(windowHandle, &previousWindowPlacement);
		SetWindowPos(windowHandle, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		isFullscreen = false;
	}
#endif // _WINDOWS

	RetAssert(CreateSwapChain(true));
}

bool Presenter::SetMultiSampling(MultiSamplingType type, UINT count, UINT quality)
{
	assert(device);
	switch (type)
	{
	case MultiSamplingType::NONE:
	{
		//disable multisampling
		multisampling_enabled = false;
		break;
	}
	case MultiSamplingType::MSAA:
	{
		//check support for requested MSAA quality and count
		UINT _supportedQuality = 0;
		device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, count, &_supportedQuality);
		if ((_supportedQuality == 0) || (_supportedQuality < quality))
		{
			//TODO: maybe add a fallback MSAA? like 4x
			multisampling_enabled = false;
		}
		//enable multisampling
		multisampling_quality = quality;
		multisampling_enabled = true;
		break;
	}
	default:
		return false;
	}

	//set the type and rebuild the swapchain
	multisampling_type = type;
	return CreateSwapChain(false);
}

bool Presenter::CreateSwapChain(bool isResize)
{
	assert(graphics_factory || graphics_interface);
	if (isResize)
	{
		assert(swapchain);
		//release current resources
		context->OMSetRenderTargets(0, nullptr, nullptr);
		context->OMSetDepthStencilState(nullptr, UINT_MAX);
		DXRelease(render_target_view);
		DXRelease(depth_stencil_view);
		DXRelease(depth_stencil);
		DXRelease(back_buffer);
	}

	//fill-in swapchain description struct
	//TODO: make dynamic resoulution - done?
	//TODO: less Magic Numbers
	RECT _rect = {};
	GetClientRect(QueryService<Platform*>("platform")->GetWindowHandle(), &_rect);
	if (isResize)
	{
		swapchain_desc.Width = _rect.right - _rect.left;
		swapchain_desc.Height = _rect.bottom - _rect.top;
		DXAssert(swapchain->ResizeBuffers(0, swapchain_desc.Width, swapchain_desc.Height, DXGI_FORMAT_UNKNOWN, swapchain_desc.Flags));
	}
	else
	{
		ZeroMemory(&swapchain_desc, sizeof(swapchain_desc));
		swapchain_desc.Width = _rect.right - _rect.left;
		swapchain_desc.Height = _rect.bottom - _rect.top;
		swapchain_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapchain_desc.Stereo = false;
		swapchain_desc.Scaling = DXGI_SCALING_STRETCH;
		swapchain_desc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
		swapchain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapchain_desc.BufferCount = 1;
		swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		ZeroMemory(&swapchain_fullscreen_desc, sizeof(swapchain_fullscreen_desc));
		swapchain_fullscreen_desc.RefreshRate.Numerator = 60;
		swapchain_fullscreen_desc.RefreshRate.Denominator = 1;
		swapchain_fullscreen_desc.Windowed = true;
		swapchain_fullscreen_desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapchain_fullscreen_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

		//fill-in sampling info
		switch (multisampling_type)
		{
		case MultiSamplingType::NONE:
		{
			swapchain_desc.SampleDesc.Count = 1;
			swapchain_desc.SampleDesc.Quality = 0;
			break;
		}
		case MultiSamplingType::MSAA:
		{
			swapchain_desc.SampleDesc.Count = multisampling_count;
			swapchain_desc.SampleDesc.Quality = multisampling_quality;
			break;
		}
		default:
			return false;
		}

		//create the swap chain
		DXAssert(graphics_factory->CreateSwapChainForHwnd(graphics_interface, QueryService<Platform*>("platform")->GetWindowHandle(), 
			&swapchain_desc, &swapchain_fullscreen_desc, nullptr, &swapchain));
	}

	//get backbuffer
	DXAssert(swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D),	reinterpret_cast<void**>(&back_buffer)));
	back_buffer->GetDesc(&back_buffer_desc);

	//create render target view
	DXAssert(device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view));

	//create depthstencil view
	//TODO: less Magic Numbers
	if (depth_stencil_enabled)
	{
		ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));
		depth_stencil_desc.Width = swapchain_desc.Width;
		depth_stencil_desc.Height = swapchain_desc.Height;
		depth_stencil_desc.MipLevels = 1;
		depth_stencil_desc.ArraySize = 1;
		depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
		depth_stencil_desc.CPUAccessFlags = 0;
		depth_stencil_desc.SampleDesc.Count = swapchain_desc.SampleDesc.Count;
		depth_stencil_desc.SampleDesc.Quality = swapchain_desc.SampleDesc.Quality;

		return SetDepthStencil(true, &depth_stencil_desc) && CreateViewPort();
	}

	return SetDepthStencil(false, nullptr) && CreateViewPort();
}

bool Presenter::CreateViewPort()
{
	assert(device || context);
	//assign render target and depthstencil views to outputmerger
	context->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	//fill-in viewport info
	view_port.TopLeftX = 0.0f;
	view_port.TopLeftY = 0.0f;
	view_port.Width = static_cast<float>(swapchain_desc.Width);
	view_port.Height = static_cast<float>(swapchain_desc.Height);
	view_port.MinDepth = 0.0f;
	view_port.MaxDepth = 1.0f;

	//assign viewport to rasterizer
	context->RSSetViewports(1, &view_port);

	return true;
}

void Presenter::CreateRasterizerStates()
{
	D3D11_RASTERIZER_DESC _rasterizer_desc = {};
	ZeroMemory(&_rasterizer_desc, sizeof(_rasterizer_desc));

	// Cull None, Solid Fill, Clockwise
	_rasterizer_desc.CullMode = D3D11_CULL_NONE;
	_rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	_rasterizer_desc.FrontCounterClockwise = false;
	DXAssert(device->CreateRasterizerState(&_rasterizer_desc, &rasterizer_state_NSCW));

	// Cull None, Solid Fill, Counter-Clockwise
	_rasterizer_desc.CullMode = D3D11_CULL_NONE;
	_rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	_rasterizer_desc.FrontCounterClockwise = true;
	DXAssert(device->CreateRasterizerState(&_rasterizer_desc, &rasterizer_state_NSCCW));

	// Cull None, Wireframe, Clockwise
	_rasterizer_desc.CullMode = D3D11_CULL_NONE;
	_rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
	_rasterizer_desc.FrontCounterClockwise = false;
	DXAssert(device->CreateRasterizerState(&_rasterizer_desc, &rasterizer_state_NWCW));

	// Cull None, Wireframe, Counter Clockwise
	_rasterizer_desc.CullMode = D3D11_CULL_NONE;
	_rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
	_rasterizer_desc.FrontCounterClockwise = true;
	DXAssert(device->CreateRasterizerState(&_rasterizer_desc, &rasterizer_state_NWCCW));

	// Cull Back, Solid Fill, Clockwise
	_rasterizer_desc.CullMode = D3D11_CULL_BACK;
	_rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	_rasterizer_desc.FrontCounterClockwise = false;
	DXAssert(device->CreateRasterizerState(&_rasterizer_desc, &rasterizer_state_BSCW));

	// Cull Back, Solid Fill, Counter-Clockwise
	_rasterizer_desc.CullMode = D3D11_CULL_BACK;
	_rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	_rasterizer_desc.FrontCounterClockwise = true;
	DXAssert(device->CreateRasterizerState(&_rasterizer_desc, &rasterizer_state_BSCCW));

	// Cull Back, Wireframe, Clockwise
	_rasterizer_desc.CullMode = D3D11_CULL_BACK;
	_rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
	_rasterizer_desc.FrontCounterClockwise = false;
	DXAssert(device->CreateRasterizerState(&_rasterizer_desc, &rasterizer_state_BWCW));

	// Cull Back, Wireframe, Counter Clockwise
	_rasterizer_desc.CullMode = D3D11_CULL_BACK;
	_rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
	_rasterizer_desc.FrontCounterClockwise = true;
	DXAssert(device->CreateRasterizerState(&_rasterizer_desc, &rasterizer_state_BWCCW));

	// Cull Front, Solid Fill, Clockwise
	_rasterizer_desc.CullMode = D3D11_CULL_FRONT;
	_rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	_rasterizer_desc.FrontCounterClockwise = false;
	DXAssert(device->CreateRasterizerState(&_rasterizer_desc, &rasterizer_state_FSCW));

	// Cull Front, Solid Fill, Counter-Clockwise
	_rasterizer_desc.CullMode = D3D11_CULL_FRONT;
	_rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	_rasterizer_desc.FrontCounterClockwise = true;
	DXAssert(device->CreateRasterizerState(&_rasterizer_desc, &rasterizer_state_FSCCW));

	// Cull Front, Wireframe, Clockwise
	_rasterizer_desc.CullMode = D3D11_CULL_FRONT;
	_rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
	_rasterizer_desc.FrontCounterClockwise = false;
	DXAssert(device->CreateRasterizerState(&_rasterizer_desc, &rasterizer_state_FWCW));

	// Cull Front, Wireframe, Counter Clockwise
	_rasterizer_desc.CullMode = D3D11_CULL_FRONT;
	_rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
	_rasterizer_desc.FrontCounterClockwise = true;
	DXAssert(device->CreateRasterizerState(&_rasterizer_desc, &rasterizer_state_FWCCW));
}

void Presenter::CreateBlendStates()
{
	D3D11_BLEND_DESC _blend_desc = {};
	
	//blend disabled
	ZeroMemory(&_blend_desc, sizeof(_blend_desc));
	_blend_desc.RenderTarget[0].BlendEnable = FALSE;
	_blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	DXAssert(device->CreateBlendState(&_blend_desc, &blend_state_disabled));
	
	//blend enabled
	ZeroMemory(&_blend_desc, sizeof(_blend_desc));
	_blend_desc.AlphaToCoverageEnable = FALSE;
	_blend_desc.IndependentBlendEnable = FALSE;
	_blend_desc.RenderTarget[0].BlendEnable = TRUE;
	_blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	_blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	_blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	_blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	_blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
	_blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	_blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	DXAssert(device->CreateBlendState(&_blend_desc, &blend_state_enabled));
}

bool Presenter::SetDepthStencil(bool state, D3D11_TEXTURE2D_DESC* desc)
{
	assert(device);
	if (state && desc)
	{
		//create depthstencil texture
		DXAssert(device->CreateTexture2D(desc, nullptr, &depth_stencil));

		//create depthstencilview
		DXAssert(device->CreateDepthStencilView(depth_stencil, nullptr, &depth_stencil_view));
	}
	else
	{
		//unassign depthstencil views from outputmerger
		context->OMSetDepthStencilState(nullptr, UINT_MAX);
		depth_stencil_view = nullptr;
	}

	//return success
	depth_stencil_enabled = state;
	return true;
}

bool Presenter::SetRasterizerState(RasterizerMode mode)
{
	assert(context);

	switch (mode)
	{
		case RasterizerMode::CULL_NONE_SOLID_CW: context->RSSetState(rasterizer_state_NSCW);
			break;
		case RasterizerMode::CULL_NONE_SOLID_CCW: context->RSSetState(rasterizer_state_NSCCW);
			break;
		case RasterizerMode::CULL_NONE_WIREFRAME_CW: context->RSSetState(rasterizer_state_NWCW);
			break;
		case RasterizerMode::CULL_NONE_WIREFRAME_CCW: context->RSSetState(rasterizer_state_NWCCW);
			break;
		case RasterizerMode::CULL_BACK_SOLID_CW: context->RSSetState(rasterizer_state_BSCW);
			break;
		case RasterizerMode::CULL_BACK_SOLID_CCW: context->RSSetState(rasterizer_state_BSCCW);
			break;
		case RasterizerMode::CULL_BACK_WIREFRAME_CW: context->RSSetState(rasterizer_state_BWCW);
			break;
		case RasterizerMode::CULL_BACK_WIREFRAME_CCW: context->RSSetState(rasterizer_state_BWCCW);
			break;
		case RasterizerMode::CULL_FRONT_SOLID_CW: context->RSSetState(rasterizer_state_FSCW);
			break;
		case RasterizerMode::CULL_FRONT_SOLID_CCW: context->RSSetState(rasterizer_state_FSCCW);
			break;
		case RasterizerMode::CULL_FRONT_WIREFRAME_CW: context->RSSetState(rasterizer_state_FWCW);
			break;
		case RasterizerMode::CULL_FRONT_WIREFRAME_CCW: context->RSSetState(rasterizer_state_FWCCW);
			break;
		default:
		{
			assert(false);
			return false;
		}
	}

	return true;
}

void Presenter::SetBlendMode(BlendMode mode)
{
	float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	unsigned int blend_mask = 0xffffffff;
	switch (mode)
	{
		case BlendMode::DISABLED:
		{
			context->OMSetBlendState(blend_state_disabled, blend_factor, blend_mask);
			blend_enabled = false;
			break;
		}
		case BlendMode::ENABLED:
		{
			context->OMSetBlendState(blend_state_enabled, blend_factor, blend_mask);
			blend_enabled = true;
			break;
		}
		default:
			break;
	}
}

Supervisor* Presenter::GetSupervisor()
{
	return supervisor;
}

Overlay* Presenter::GetOverlay()
{
	return overlay.get();
}

Aggregator* Presenter::GetAggregator()
{
	return aggregator.get();
}

ID3D11Device* Presenter::GetDevice()
{
	return device;
}

ID3D11DeviceContext* Presenter::GetContext()
{
	return context;
}

unsigned int Presenter::GetScreenWidth()
{
	return swapchain_desc.Width;
}

unsigned int Presenter::GetScreenHeight()
{
	return swapchain_desc.Height;
}
