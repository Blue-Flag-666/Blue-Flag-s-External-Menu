#include "pch.hpp"

void BF::RendererD3D9::init(const HWND hWnd)
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &object)))
	{
		exit(EXIT_FAILURE);  // NOLINT(concurrency-mt-unsafe)
	}

CreateDeviceRetry:ZeroMemory(&params, sizeof params);

	params.BackBufferWidth  = OverlayWidth;
	params.BackBufferHeight = OverlayHeight;
	params.BackBufferFormat = D3DFMT_A8R8G8B8;
	if (SUCCEEDED(object->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL, D3DFMT_A8R8G8B8, true,D3DMULTISAMPLE_16_SAMPLES, NULL)))
	{
		params.MultiSampleType = D3DMULTISAMPLE_16_SAMPLES;
	}
	else
	{
		params.MultiSampleType = D3DMULTISAMPLE_NONE;
		OutputDebugString(L"MultiSample Not Supported\n");
		MessageBox(nullptr, L"MultiSample Not Supported", L"Blue-Flag\'s External Menu", MB_OK);
	}
	params.SwapEffect             = D3DSWAPEFFECT_DISCARD;
	params.hDeviceWindow          = hWnd;
	params.Windowed               = true;
	params.EnableAutoDepthStencil = true;
	params.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &params, nullptr, &device)))
	{
		if (MessageBox(nullptr, L"Create D3D Device Failed", L"Blue-Flag\'s External Menu", MB_RETRYCANCEL) == IDRETRY)
		{
			goto CreateDeviceRetry;  // NOLINT(cppcoreguidelines-avoid-goto, hicpp-avoid-goto)
		}
	}
	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, true);
	D3DXCreateFont(device, 50, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, settings.FontName.c_str(), &font);
}

void BF::RendererD3D9::drawText(const string& str, const int x, const int y, const int a, const int r, const int g, const int b) const
{
	RECT rect = { x, y };
	font->DrawTextA(nullptr, str.c_str(), static_cast <int>(str.size()), &rect, DT_NOCLIP, D3DCOLOR_ARGB(a, r, g, b));
}

void BF::RendererD3D9::render() const
{
	OutputDebugString(L"Renderer Called\n");

	if (device == nullptr)
	{
		return;
	}

	device->Clear(0, nullptr, D3DCLEAR_TARGET, 0, 1.0f, 0);
	device->BeginScene();

	if (settings.ActiveMenu && (settings.AlwaysShow || TargetHWND == GetForegroundWindow()))
	{
		drawText("Blue-Flag\'s External Menu", OverlayWidth / 10, OverlayHeight / 10, 255, 30, 144, 255);
	}

	device->EndScene();
	device->PresentEx(nullptr, nullptr, nullptr, nullptr, 0);
}

BF::RendererD3D9::RendererD3D9(const HWND overlayHWND, const HWND targetHWND, const int width, const int height, const Settings& setting)
{
	OverlayWidth  = width;
	OverlayHeight = height;
	TargetHWND    = targetHWND;
	settings      = setting;
	init(overlayHWND);
}
