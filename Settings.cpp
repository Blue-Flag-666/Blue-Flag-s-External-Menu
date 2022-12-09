#include "pch.hpp"
#include "Settings.hpp"

BaseSettings::BaseSettings(const wstring_view filename)
{
	Filename = filename;
}

void Settings::initDefault()
{
	OverlayWidth  = 1920;
	OverlayHeight = 1080;

	ActiveMenu       = true;
	AlwaysShow       = false;
	ConsoleDebug     = false;
	KeysThreadKilled = false;
	KillMenu         = false;
	NoUI             = false;
	SkipMemInit      = false;
	UseOldD3D        = true;

	Keys["ToggleMenu"]   = "NUM-";
	Keys["MenuSelect"]   = "NUM5";
	Keys["MenuUp"]       = "NUM8";
	Keys["MenuDown"]     = "NUM2";
	Keys["MenuLeft"]     = "NUM4";
	Keys["MenuRight"]    = "NUM6";
	Keys["MenuTabLeft"]  = "NUM7";
	Keys["MenuTabRight"] = "NUM9";
	Keys["MenuBack"]     = "NUM0";
}

void Settings::savetofile() const
{
	const auto table = toml::table
	{
		{
			"Menu",
			toml::table
			{
				{ "OverlayWidth", OverlayWidth },
				{ "OverlayHeight", OverlayHeight },
				{ "AlwaysShow", AlwaysShow },
				{ "ConsoleDebug", ConsoleDebug },
				{ "NoUI", NoUI },
				{ "UseOldD3D", UseOldD3D },
			}
		},
		{
			"Keys",
			toml::table
			{
				{ "ToggleMenu", Keys.at("ToggleMenu") },
				{ "MenuSelect", Keys.at("MenuSelect") },
				{ "MenuUp", Keys.at("MenuUp") },
				{ "MenuDown", Keys.at("MenuDown") },
				{ "MenuLeft", Keys.at("MenuLeft") },
				{ "MenuRight", Keys.at("MenuRight") },
				{ "MenuTabLeft", Keys.at("MenuTabLeft") },
				{ "MenuTabRight", Keys.at("MenuTabRight") },
				{ "MenuBack", Keys.at("MenuBack") },
			}
		}
	};
	ofstream savetofile(Filename);
	savetofile << table << endl;
}

Settings::Settings(const wstring_view filename): BaseSettings(filename)
{
	if (!filesystem::exists(filename))
	{
		initDefault();
		savetofile();
	}
	try
	{
		auto table    = toml::parse_file(filename);
		OverlayWidth  = table["Menu"]["OverlayWidth"].value <int>().value();
		OverlayHeight = table["Menu"]["OverlayHeight"].value <int>().value();

		ActiveMenu       = true;
		AlwaysShow       = table["Menu"]["AlwaysShow"].value <bool>().value();
		ConsoleDebug     = table["Menu"]["ConsoleDebug"].value <bool>().value();
		KeysThreadKilled = false;
		KillMenu         = false;
		NoUI             = table["Menu"]["NoUI"].value <bool>().value();
		SkipMemInit      = false;
		UseOldD3D        = table["Menu"]["UseOldD3D"].value <bool>().value();

		Keys["ToggleMenu"]   = table["Keys"]["ToggleMenu"].value <string>().value();
		Keys["MenuSelect"]   = table["Keys"]["MenuSelect"].value <string>().value();
		Keys["MenuUp"]       = table["Keys"]["MenuUp"].value <string>().value();
		Keys["MenuDown"]     = table["Keys"]["MenuDown"].value <string>().value();
		Keys["MenuLeft"]     = table["Keys"]["MenuLeft"].value <string>().value();
		Keys["MenuRight"]    = table["Keys"]["MenuRight"].value <string>().value();
		Keys["MenuTabLeft"]  = table["Keys"]["MenuTabLeft"].value <string>().value();
		Keys["MenuTabRight"] = table["Keys"]["MenuTabRight"].value <string>().value();
		Keys["MenuBack"]     = table["Keys"]["MenuBack"].value <string>().value();
	}
	catch (const toml::parse_error& err)
	{
		const auto str = to_wstring(err.description().data()) + L"\n(error occurred at line " + to_wstring(err.source().begin.line) + L", column " + to_wstring(err.source().begin.column) + L" of '" + to_wstring(*err.source().path) + L"')";
		MessageBox(nullptr, str.c_str(), L"Parsing Failed",MB_OK);
		initDefault();
	}
}

void RendererSettings::initDefault()
{
	MenuX      = 50;
	MenuY      = 50;
	MenuWidth  = 300;
	MenuHeight = 100;
	BorderSize = 2;
	FontName   = L"Arial";
}

void RendererSettings::savetofile() const
{
	const auto table = toml::table
	{
		{
			"Menu",
			toml::table
			{
				{ "MenuX", MenuX },
				{ "MenuY", MenuY },
				{ "MenuWidth", MenuWidth },
				{ "MenuHeight", MenuHeight },
				{ "BorderSize", BorderSize },
				{ "FontName", FontName },
			}
		}
	};
	if (!filesystem::exists("Themes"))
	{
		filesystem::create_directory("Themes");
	}
	ofstream savetofile(L"Themes\\" + Filename);
	savetofile << table << endl;
}

RendererSettings::RendererSettings(const wstring_view filename): BaseSettings(filename)
{
	if (!filesystem::exists(L"Themes\\" + wstring(filename)))
	{
		initDefault();
		savetofile();
	}
	try
	{
		auto table = toml::parse_file(filename);
		MenuX      = table["Menu"]["MenuX"].value <int>().value();
		MenuY      = table["Menu"]["MenuY"].value <int>().value();
		MenuWidth  = table["Menu"]["MenuWidth"].value <int>().value();
		MenuHeight = table["Menu"]["MenuHeight"].value <int>().value();
		BorderSize = table["Menu"]["BorderSize"].value <int>().value();
		FontName   = table["Menu"]["FontName"].value <wstring>().value();
	}
	catch (const toml::parse_error& err)
	{
		const auto str = to_wstring(err.description().data()) + L"\n(error occurred at line " + to_wstring(err.source().begin.line) + L", column " + to_wstring(err.source().begin.column) + L" of '" + to_wstring(*err.source().path) + L"')";
		MessageBox(nullptr, str.c_str(), L"Parsing Failed",MB_OK);
		initDefault();
	}
}
