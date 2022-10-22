#pragma once

#include "pch.hpp"

namespace BF
{
	class Settings
	{
		void initDefault();

		public:
			int               OverlayWidth {}, OverlayHeight {};
			bool              ActiveMenu {},   AlwaysShow {}, ConsoleDebug {}, KeysThreadKilled {}, KillMenu {}, NoUI {}, SkipMemInit {}, UseOldD3D {};
			wstring           filename{L"Blue-Flag.toml"},FontName {};
			map <string, int> keys {};

			void savetofile();

			Settings();
			explicit Settings(const wstring& filename);
	};
}
