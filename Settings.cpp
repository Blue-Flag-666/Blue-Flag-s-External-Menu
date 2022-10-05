#include "pch.hpp"

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

	FontName = L"Arial";

	keys["ToggleMenu"] = StrToVK("NUM-");
	keys["MenuSelect"] = StrToVK("NUM5");
	keys["MenuUp"]     = StrToVK("NUM8");
	keys["MenuDown"]   = StrToVK("NUM2");
	keys["MenuLeft"]   = StrToVK("NUM7");
	keys["MenuRight"]  = StrToVK("NUM9");
	keys["MenuBack"]   = StrToVK("NUM0");
}

BF::Settings::Settings()
{
	initDefault();
}

BF::Settings::Settings(const string& filename)
{
	try
	{
		table = toml::parse_file(filename);
	}
	catch (const toml::parse_error& err)
	{
		MessageBox(nullptr, to_wstring(err.what()).c_str(), L"Parsing Failed",MB_OK);
		initDefault();
	}
	// TODO: 在此处放置代码
}
