#pragma once

#include "pch.hpp"
#include "tomlplusplus\toml.hpp"

using std::map;
using std::string;
using std::wstring;

namespace BF
{
	class Settings
	{
		toml::table table;

		void initDefault();

		public:
			int               OverlayWidth {}, OverlayHeight {};
			bool              ActiveMenu {},   AlwaysShow {}, ConsoleDebug {}, KeysThreadKilled {}, KillMenu {}, NoUI {}, SkipMemInit {};
			wstring           FontName;
			map <string, int> keys;

			Settings();
			explicit Settings(const string& filename);
	};
}
