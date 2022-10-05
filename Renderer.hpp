#pragma once

#include "pch.hpp"

#include <d3d12.h>
#include <d3d9.h>
#include <d3dx9.h>

using std::string;

namespace BF
{
	class RendererD3D12
	{
		HWND     TargetHWND {};
		int      OverlayWidth {}, OverlayHeight {};
		Settings settings;

		ID3D12Device* device;
	};

	class RendererD3D9
	{
		HWND     TargetHWND {};
		int      OverlayWidth {}, OverlayHeight {};
		Settings settings;

		IDirect3D9Ex*         object {};
		IDirect3DDevice9Ex*   device {};
		D3DPRESENT_PARAMETERS params {};
		ID3DXFont*            font {};

		void init(HWND hWnd);
		void drawText(const string& str, int x, int y, int a, int r, int g, int b) const;

		public:
			void render() const;
			RendererD3D9() = default;
			RendererD3D9(HWND overlayHWND, HWND targetHWND, int width, int height, const Settings& setting);
	};
}
