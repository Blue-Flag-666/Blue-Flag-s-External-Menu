#include "pch.hpp"
#include "Renderer.hpp"
#include "Blue-Flag's External Menu.hpp"

BF::Renderer::Renderer(const HWND targetHWND, Settings& s)
{
	TargetHWND = targetHWND;
	settings   = shared_ptr <Settings>(&s);
}

void BF::RendererGDI::init(HWND hWnd)
{
}

void BF::RendererGDI::drawText(const wstring& str, int x, int y, int a, int r, int g, int b) const
{
}

void BF::RendererGDI::drawText(const wstring& str, int x, int y, int w, int h, int a, int r, int g, int b) const
{
}

void BF::RendererGDI::drawText(const wstring& str, int x, int y, D3DCOLOR color) const
{
}

void BF::RendererGDI::drawText(const wstring& str, int x, int y, int w, int h, D3DCOLOR color) const
{
}

void BF::RendererGDI::render() const
{
}

BF::RendererGDI::RendererGDI(const HWND overlayHWND, const HWND targetHWND, Settings& s): Renderer(targetHWND, s)
{
	init(overlayHWND);
}

void BF::RendererD3D12::init(HWND hWnd)
{
}

void BF::RendererD3D12::drawText(const wstring& str, int x, int y, int a, int r, int g, int b) const
{
}

void BF::RendererD3D12::drawText(const wstring& str, int x, int y, int w, int h, int a, int r, int g, int b) const
{
}

void BF::RendererD3D12::drawText(const wstring& str, int x, int y, D3DCOLOR color) const
{
}

void BF::RendererD3D12::drawText(const wstring& str, int x, int y, int w, int h, D3DCOLOR color) const
{
}

void BF::RendererD3D12::render() const
{
}

BF::RendererD3D12::RendererD3D12(const HWND overlayHWND, const HWND targetHWND, Settings& s): Renderer(targetHWND, s)
{
	init(overlayHWND);
}

void BF::RendererD3D9::init(const HWND hWnd)
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &object)))
	{
		exit(EXIT_FAILURE);  // NOLINT(concurrency-mt-unsafe)
	}

	ZeroMemory(&params, sizeof params);

	const auto& settings = getSettings();

	params.BackBufferWidth  = settings->OverlayWidth;
	params.BackBufferHeight = settings->OverlayHeight;
	params.BackBufferFormat = D3DFMT_A8R8G8B8;
	if (SUCCEEDED(object->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL, D3DFMT_A8R8G8B8, true,D3DMULTISAMPLE_16_SAMPLES, NULL)))
	{
		params.MultiSampleType = D3DMULTISAMPLE_16_SAMPLES;
	}
	else
	{
		params.MultiSampleType = D3DMULTISAMPLE_NONE;
		OutputDebugString(L"MultiSample Not Supported\n");
		MessageBox(nullptr, L"MultiSample Not Supported", OverlayTitle.c_str(), MB_OK);
	}
	params.SwapEffect             = D3DSWAPEFFECT_DISCARD;
	params.hDeviceWindow          = hWnd;
	params.Windowed               = true;
	params.EnableAutoDepthStencil = true;
	params.AutoDepthStencilFormat = D3DFMT_D16;

	if (FAILED(object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &params, nullptr, &device)))
	{
		MessageBox(nullptr, L"Create D3D Device Failed", OverlayTitle.c_str(), MB_RETRYCANCEL);
		KillMenu();
	}
	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, true);
	D3DXCreateFont(device, 50, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, settings->FontName.c_str(), &font);
}

void BF::RendererD3D9::drawText(const wstring& str, const int x, const int y, const int a, const int r, const int g, const int b) const
{
	RECT rect = { x, y };
	font->DrawTextW(nullptr, str.c_str(), static_cast <int>(str.size()), &rect, DT_NOCLIP, D3DCOLOR_ARGB(a, r, g, b));
}

void BF::RendererD3D9::drawText(const wstring& str, const int x, const int y, const int w, const int h, const int a, const int r, const int g, const int b) const
{
	RECT rect = { x, y, x + w, y + h };
	font->DrawTextW(nullptr, str.c_str(), static_cast <int>(str.size()), &rect, DT_NOCLIP, D3DCOLOR_ARGB(a, r, g, b));
}

void BF::RendererD3D9::drawText(const wstring& str, const int x, const int y, const D3DCOLOR color) const
{
	RECT rect = { x, y };
	font->DrawTextW(nullptr, str.c_str(), static_cast <int>(str.size()), &rect, DT_NOCLIP, color);
}

void BF::RendererD3D9::drawText(const wstring& str, const int x, const int y, const int w, const int h, const D3DCOLOR color) const
{
	RECT rect = { x, y, x + w, y + h };
	font->DrawTextW(nullptr, str.c_str(), static_cast <int>(str.size()), &rect, DT_NOCLIP, color);
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

	if (const auto settings = getSettings(); settings->ActiveMenu && (settings->AlwaysShow || targetHWND() == GetForegroundWindow()))
	{
		drawText(L"OK", settings->OverlayWidth / 10, settings->OverlayHeight / 10, 255, 30, 144, 255);
	}

	device->EndScene();
	device->PresentEx(nullptr, nullptr, nullptr, nullptr, 0);
}

BF::RendererD3D9::RendererD3D9(const HWND overlayHWND, const HWND targetHWND, Settings& s): Renderer(targetHWND, s)
{
	init(overlayHWND);
}
