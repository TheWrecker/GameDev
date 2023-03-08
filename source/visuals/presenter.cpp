
#include <DirectXMath.h>

#include "../core/platform.h"
#include "../core/service_manager.h"
#include "../core/supervisor.h"
#include "util_funcs.h"

#include "presenter.h"

Presenter::Presenter(Supervisor* parent)
	:supervisor(parent), depth_stencil_enabled(true), multisampling_enabled(false), isFullscreen(false)
{
	HRESULT result = 0;
	UINT createDeviceFlags = 0;

#if defined (debug) || defined (_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // DEBUG

	//create directx device, context, and interfaces
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL selectedFeatureLevel;
	DXAssert(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags,	featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION,
		&device, &selectedFeatureLevel, &context));
	DXAssert(device->QueryInterface(__uuidof(IDXGIDevice), 	reinterpret_cast<void**>(&graphics_interface)));
	DXAssert(graphics_interface->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&graphics_adapter)));
	DXAssert(graphics_adapter->GetParent(__uuidof(IDXGIFactory),	reinterpret_cast<void**>(&graphics_factory)));

#ifdef _WINDOWS
	//disable user-side fullscreen switching
	DXAssert(graphics_factory->MakeWindowAssociation(supervisor->Services()->QueryService<Platform*>("platform")->GetWindowHandle(), DXGI_MWA_NO_ALT_ENTER));
#endif

	RetAssert(SetMultiSampling(MultiSamplingType::NONE, 1, 0));
	RetAssert(SetRasterizerState(CullMode::CULL_NONE, false, false));

	scene = std::make_unique<Scene>(this);
	scene->SwitchMode(SceneMode::DEVELOPEMENT);
	overlay = std::make_unique<Overlay>(this);
	//overlay->Show();

	//show all live d3d11device objects
	/*ID3D11Debug* _debug = nullptr;
	device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&_debug));
	supervisor->SetDebugQuery(_debug);*/
}

Presenter::~Presenter()
{
	DXRelease(back_buffer);
	DXRelease(render_target_view);
	DXRelease(depth_stencil);
	DXRelease(depth_stencil_view);
	DXRelease(rasterizer_state);
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
	scene->Draw();
	overlay->Draw();
	swapchain->Present(0, 0);
}

void Presenter::Update()
{
	scene->Update();
	overlay->Update();
}

void Presenter::ToggleFullscreen()
{
	SysWindowHandle windowHandle = supervisor->Services()->QueryService<Platform*>("platform")->GetWindowHandle();

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
	GetClientRect(supervisor->Services()->QueryService<Platform*>("platform")->GetWindowHandle(), &_rect);
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
		DXAssert(graphics_factory->CreateSwapChainForHwnd(graphics_interface, supervisor->Services()->QueryService<Platform*>("platform")->GetWindowHandle(), 
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

bool Presenter::SetRasterizerState(CullMode cullmodle, bool wireframe, bool frontCCW)
{
	assert(context);
	DXRelease(rasterizer_state);

	//fill-in rasterizer desc
	ZeroMemory(&rasterizer_state_desc, sizeof(rasterizer_state_desc));

	//culling
	switch (cullmodle)
	{
	case CullMode::CULL_NONE: rasterizer_state_desc.CullMode = D3D11_CULL_NONE;
		break;
	case CullMode::CULL_BACK: rasterizer_state_desc.CullMode = D3D11_CULL_BACK;
		break;
	case CullMode::CULL_FRONT: rasterizer_state_desc.CullMode = D3D11_CULL_FRONT;
		break;
	default:
		assert(false);
	}

	//wireframe
	if (wireframe)
		rasterizer_state_desc.FillMode = D3D11_FILL_WIREFRAME;
	else
		rasterizer_state_desc.FillMode = D3D11_FILL_SOLID;

	//front winding order
	rasterizer_state_desc.FrontCounterClockwise = frontCCW;

	//create rasterizer state
	DXAssert(device->CreateRasterizerState(&rasterizer_state_desc, &rasterizer_state));
	context->RSSetState(rasterizer_state);
	return true;
}

Supervisor* Presenter::GetSupervisor()
{
	return supervisor;
}

Scene* Presenter::GetActiveScene()
{
	return scene.get();
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
