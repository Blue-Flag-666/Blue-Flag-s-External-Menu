#include "pch.hpp"
#include "Renderer.hpp"
#include "Blue-Flag's External Menu.hpp"

Renderer::Renderer(const HWND targetHWND, Settings& s)
{
	TargetHWND = targetHWND;
	settings   = shared_ptr <Settings>(&s);
}

void RendererGDI::init(HWND hWnd) {}

void RendererGDI::drawText(const wstring_view str, int x, int y, D3DCOLOR color) const {}

void RendererGDI::drawText(const wstring_view str, int x, int y, int w, int h, D3DCOLOR color, DWORD flags) const {}

void RendererGDI::drawBox(int x, int y, int w, int h, D3DCOLOR color) const {}

void RendererGDI::drawBoxWithBorder(int x, int y, int w, int h, int borderSize, D3DCOLOR color, D3DCOLOR borderColor) const {}

void RendererGDI::drawImage(int x, int y, const wstring_view name, const LPWSTR res = nullptr) const {}

void RendererGDI::render() const {}

RendererGDI::RendererGDI(const HWND overlayHWND, const HWND targetHWND, Settings& s): Renderer(targetHWND, s)
{
	init(overlayHWND);
}

void RendererD3D12::init(HWND hWnd) {}

void RendererD3D12::drawText(const wstring_view str, int x, int y, D3DCOLOR color) const {}

void RendererD3D12::drawText(const wstring_view str, int x, int y, int w, int h, D3DCOLOR color, DWORD flags) const {}

void RendererD3D12::drawBox(int x, int y, int w, int h, D3DCOLOR color) const {}

void RendererD3D12::drawBoxWithBorder(int x, int y, int w, int h, int borderSize, D3DCOLOR color, D3DCOLOR borderColor) const {}

void RendererD3D12::drawImage(int x, int y, const wstring_view name, const LPWSTR res = nullptr) const {}

void RendererD3D12::render() const {}

RendererD3D12::RendererD3D12(const HWND overlayHWND, const HWND targetHWND, Settings& s): Renderer(targetHWND, s)
{
	init(overlayHWND);
}

void RendererD3D9::init(const HWND hWnd)
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
	D3DXCreateFont(device, 50, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, getRendererSettings().FontName.c_str(), &font);
	D3DXCreateSprite(device, &sprite);
}

void RendererD3D9::drawText(const wstring_view str, const int x, const int y, const D3DCOLOR color) const
{
	RECT rect = { x, y };
	font->DrawText(nullptr, str.data(), static_cast <int>(str.size()), &rect, DT_NOCLIP, color);
}

void RendererD3D9::drawText(const wstring_view str, const int x, const int y, const int w, const int h, const D3DCOLOR color, DWORD flags) const
{
	RECT rect = { x, y, x + w, y + h };
	font->DrawText(nullptr, str.data(), static_cast <int>(str.size()), &rect, DT_NOCLIP, color);
}

void RendererD3D9::drawBox(const int x, const int y, const int w, const int h, const D3DCOLOR color) const
{
	const D3DRECT rect = { x, y, x + w, y + h };
	device->Clear(1, &rect, D3DCLEAR_TARGET, color, 1.f, 0);
}

void RendererD3D9::drawBoxWithBorder(const int x, const int y, const int w, const int h, const int borderSize, const D3DCOLOR color, const D3DCOLOR borderColor) const
{
	drawBox(x, y, w, h, borderColor);
	drawBox(x + borderSize, y + borderSize, w - borderSize * 2, h - borderSize * 2, color);
}

void RendererD3D9::drawImage(const int x, const int y, const wstring_view name, const LPWSTR res = nullptr) const
{
	LPDIRECT3DTEXTURE9 texture;
	if (res)
	{
		D3DXCreateTextureFromResource(device, nullptr, res, &texture);
	}
	else
	{
		D3DXCreateTextureFromFile(device, name.data(), &texture);
	}
	const auto pos = D3DXVECTOR3(static_cast <float>(x), static_cast <float>(y), 0);
	sprite->Draw(texture, nullptr, nullptr, &pos, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void RendererD3D9::render() const
{
	if (device == nullptr)
	{
		return;
	}

	device->Clear(0, nullptr, D3DCLEAR_TARGET, 0, 1.0f, 0);
	device->BeginScene();
	sprite->Begin(D3DXSPRITE_ALPHABLEND);

	if (const auto settings = getSettings(); settings->ActiveMenu && (settings->AlwaysShow || targetHWND() == GetForegroundWindow()))
	{
		drawBoxWithBorder(getRendererSettings().MenuX, getRendererSettings().MenuY, getRendererSettings().MenuWidth, getRendererSettings().MenuHeight, getRendererSettings().BorderSize, D3DCOLOR_ARGB(255, 56, 120, 226),D3DCOLOR_ARGB(255, 1, 1, 1));
		drawText(OverlayTitle, getRendererSettings().MenuX + 10, getRendererSettings().MenuY + 10, 300, 25, D3DCOLOR_ARGB(255, 1, 1, 1),DT_CENTER | DT_VCENTER);

		drawImage(250, 250, L"",MAKEINTRESOURCE(IDB_HEADER_BLACK));
	}

	sprite->End();
	device->EndScene();
	device->PresentEx(nullptr, nullptr, nullptr, nullptr, 0);
}

RendererD3D9::RendererD3D9(const HWND overlayHWND, const HWND targetHWND, Settings& s): Renderer(targetHWND, s)
{
	init(overlayHWND);
}
