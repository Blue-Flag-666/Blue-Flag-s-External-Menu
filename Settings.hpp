#pragma once

#include "pch.hpp"

namespace BF
{
	class Settings
	{
		toml::table table;

		void initDefault();

		public:
			int               OverlayWidth {}, OverlayHeight {};
			bool              ActiveMenu {},   AlwaysShow {}, ConsoleDebug {}, KeysThreadKilled {}, KillMenu {}, NoUI {}, SkipMemInit {}, UseOldD3D {};
			wstring           FontName {};
			map <string, int> keys {};

			Settings();
			explicit Settings(const string& filename);
	};
}
