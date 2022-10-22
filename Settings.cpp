#include "pch.hpp"
#include "Settings.hpp"

inline void BF::Settings::initDefault()
{
	OverlayWidth  = 1920;
	OverlayHeight = 1080;

	ActiveMenu       = true;
	AlwaysShow       = true;
	ConsoleDebug     = false;
	KeysThreadKilled = false;
	KillMenu         = false;
	NoUI             = false;
	SkipMemInit      = false;
	UseOldD3D        = true;

	FontName = L"Arial";

	keys["ToggleMenu"]   = StrToVK("NUM-");
	keys["MenuSelect"]   = StrToVK("NUM5");
	keys["MenuUp"]       = StrToVK("NUM8");
	keys["MenuDown"]     = StrToVK("NUM2");
	keys["MenuLeft"]     = StrToVK("NUM4");
	keys["MenuRight"]    = StrToVK("NUM6");
	keys["MenuTabLeft"]  = StrToVK("NUM7");
	keys["MenuTabRight"] = StrToVK("NUM9");
	keys["MenuBack"]     = StrToVK("NUM0");
}

void BF::Settings::savetofile()
{
	toml::table table;
	table.insert("1", 1);
	ofstream savetofile(filename);
	savetofile << toml::toml_formatter(toml::parse_result(table)) << endl;
}

BF::Settings::Settings()
{
	initDefault();
}

BF::Settings::Settings(const wstring& filename)
{
	if (!exists(filename))
	{
		initDefault();
		savetofile();
	}
	try
	{
		auto table = toml::parse_file(filename);
		// TODO
	}
	catch (const toml::parse_error& err)
	{
		const auto str = to_wstring(err.description().data()) + L"\n(error occurred at line " + std::to_wstring(err.source().begin.line) + L", column " + std::to_wstring(err.source().begin.column) + L" of '" + to_wstring(*err.source().path) + L"')";
		MessageBox(nullptr, str.c_str(), L"Parsing Failed",MB_OK);
		initDefault();
	}
}
