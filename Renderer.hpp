#pragma once

#include "pch.hpp"
#include "Settings.hpp"

namespace BF
{
	class Renderer
	{
		HWND                  TargetHWND {};
		shared_ptr <Settings> settings {};

		virtual void init(HWND hWnd) = 0;
		virtual void drawText(const wstring& str, int x, int y, int a, int r, int g, int b) const = 0;
		virtual void drawText(const wstring& str, int x, int y, int w, int h, int a, int r, int g, int b) const = 0;
		virtual void drawText(const wstring& str, int x, int y, D3DCOLOR color) const = 0;
		virtual void drawText(const wstring& str, int x, int y, int w, int h, D3DCOLOR color) const = 0;

		public:
			[[nodiscard]] const HWND& targetHWND() const
			{
				return TargetHWND;
			}

			[[nodiscard]] const shared_ptr <Settings>& getSettings() const
			{
				return settings;
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
		virtual void drawText(const wstring& str, int x, int y, int a, int r, int g, int b) const override;
		virtual void drawText(const wstring& str, int x, int y, int w, int h, int a, int r, int g, int b) const override;
		virtual void drawText(const wstring& str, int x, int y, D3DCOLOR color) const override;
		virtual void drawText(const wstring& str, int x, int y, int w, int h, D3DCOLOR color) const override;

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
		virtual void drawText(const wstring& str, int x, int y, int a, int r, int g, int b) const override;
		virtual void drawText(const wstring& str, int x, int y, int w, int h, int a, int r, int g, int b) const override;
		virtual void drawText(const wstring& str, int x, int y, D3DCOLOR color) const override;
		virtual void drawText(const wstring& str, int x, int y, int w, int h, D3DCOLOR color) const override;

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

		virtual void init(HWND hWnd) override;
		virtual void drawText(const wstring& str, int x, int y, int a, int r, int g, int b) const override;
		virtual void drawText(const wstring& str, int x, int y, int w, int h, int a, int r, int g, int b) const override;
		virtual void drawText(const wstring& str, int x, int y, D3DCOLOR color) const override;
		virtual void drawText(const wstring& str, int x, int y, int w, int h, D3DCOLOR color) const override;

		public:
			virtual void render() const override;

			RendererD3D9() = default;
			RendererD3D9(HWND overlayHWND, HWND targetHWND, Settings& s);
	};
}
