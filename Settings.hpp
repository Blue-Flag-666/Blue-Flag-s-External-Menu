#pragma once

#include "pch.hpp"

namespace BF
{
	class BaseSettings
	{
		public:
			wstring      Filename;
			virtual void initDefault() = 0;
			virtual void savetofile() const = 0;

			explicit BaseSettings(wstring_view filename);
			virtual  ~BaseSettings() = default;
	};

	class Settings final : public BaseSettings
	{
		public:
			int                  OverlayWidth = 1920, OverlayHeight = 1080;
			bool                 ActiveMenu   = true, AlwaysShow    = false, ConsoleDebug = false, KeysThreadKilled = false, KillMenu = false, NoUI = false, SkipMemInit = false, UseOldD3D = true;
			map <string, string> Keys {};

			virtual void initDefault() override;
			virtual void savetofile() const override;

			explicit Settings(wstring_view filename = L"Blue-Flag.toml");
	};

	class RendererSettings final : public BaseSettings
	{
		public:
			int     MenuX    = 50, MenuY = 50, MenuWidth = 300, MenuHeight = 100, BorderSize = 2;
			wstring FontName = L"Arial";

			virtual void initDefault() override;
			virtual void savetofile() const override;

			explicit RendererSettings(wstring_view filename = L"Default.toml");
	};
}
