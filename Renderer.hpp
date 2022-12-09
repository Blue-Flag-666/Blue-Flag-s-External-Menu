#pragma once

#include "pch.hpp"
#include "Settings.hpp"

namespace BF
{
	class Renderer
	{
		HWND                  TargetHWND {};
		shared_ptr <Settings> settings {};
		RendererSettings      rendererSettings;

		virtual void init(HWND hWnd) = 0;

		virtual void drawText(wstring_view str, int x, int y, D3DCOLOR color) const = 0;
		virtual void drawText(wstring_view str, int x, int y, int w, int h, D3DCOLOR color, DWORD flags = 0) const = 0;

		virtual void drawBox(int x, int y, int w, int h, D3DCOLOR color) const = 0;
		virtual void drawBoxBorder(int x, int y, int w, int h, int borderSize, D3DCOLOR color, D3DCOLOR borderColor) const = 0;

		virtual void drawImage(int x, int y, wstring_view name,LPWSTR res) const = 0;

		public:
			[[nodiscard]] const HWND targetHWND() const
			{
				return TargetHWND;
			}

			[[nodiscard]] const shared_ptr <Settings>& getSettings() const
			{
				return settings;
			}

			[[nodiscard]] const RendererSettings& getRendererSettings() const
			{
				return rendererSettings;
			}

			virtual void render() const = 0;

			Renderer& operator=(const Renderer&) = default;
			Renderer& operator=(Renderer&&)      = default;

			Renderer()                = default;
			Renderer(const Renderer&) = default;
			Renderer(Renderer&&)      = default;
			Renderer(HWND targetHWND, Settings& s);
			virtual ~Renderer() = default;
	};

	class RendererGDI final : public Renderer
	{
		// TODO

		virtual void init(HWND hWnd) override;

		virtual void drawText(wstring_view str, int x, int y, D3DCOLOR color) const override;
		virtual void drawText(wstring_view str, int x, int y, int w, int h, D3DCOLOR color, DWORD flags = 0) const override;

		virtual void drawBox(int x, int y, int w, int h, D3DCOLOR color) const override;
		virtual void drawBoxBorder(int x, int y, int w, int h, int borderSize, D3DCOLOR color, D3DCOLOR borderColor) const override;

		virtual void drawImage(int x, int y, wstring_view name,LPWSTR res) const override;

		public:
			virtual void render() const override;

			RendererGDI() = default;
			RendererGDI(HWND overlayHWND, HWND targetHWND, Settings& s);
	};

	class RendererD3D12 final : public Renderer
	{
		ID3D12Device* device {};
		// TODO

		virtual void init(HWND hWnd) override;

		virtual void drawText(wstring_view str, int x, int y, D3DCOLOR color) const override;
		virtual void drawText(wstring_view str, int x, int y, int w, int h, D3DCOLOR color, DWORD flags = 0) const override;

		virtual void drawBox(int x, int y, int w, int h, D3DCOLOR color) const override;
		virtual void drawBoxBorder(int x, int y, int w, int h, int borderSize, D3DCOLOR color, D3DCOLOR borderColor) const override;

		virtual void drawImage(int x, int y, wstring_view name,LPWSTR res) const override;

		public:
			virtual void render() const override;

			RendererD3D12() = default;
			RendererD3D12(HWND overlayHWND, HWND targetHWND, Settings& s);
	};

	class RendererD3D9 final : public Renderer
	{
		IDirect3D9Ex*         object {};
		IDirect3DDevice9Ex*   device {};
		D3DPRESENT_PARAMETERS params {};
		ID3DXFont*            font {};
		LPD3DXSPRITE          sprite {};

		virtual void init(HWND hWnd) override;

		virtual void drawText(wstring_view str, int x, int y, D3DCOLOR color) const override;
		virtual void drawText(wstring_view str, int x, int y, int w, int h, D3DCOLOR color, DWORD flags = 0) const override;

		virtual void drawBox(int x, int y, int w, int h, D3DCOLOR color) const override;
		virtual void drawBoxBorder(int x, int y, int w, int h, int borderSize, D3DCOLOR color, D3DCOLOR borderColor) const override;

		virtual void drawImage(int x, int y, wstring_view name,LPWSTR res) const override;

		public:
			virtual void render() const override;

			RendererD3D9() = default;
			RendererD3D9(HWND overlayHWND, HWND targetHWND, Settings& s);
	};
}
