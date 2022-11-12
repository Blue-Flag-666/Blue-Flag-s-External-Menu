// pch.hpp

#pragma once

#include <algorithm>
#include <AtlBase.h>
#include <atlconv.h>
#include <conio.h>
#include <ctime>
#include <d3d12.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <DirectXMath.h>
#include <dxgi1_6.h>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <shellapi.h>
#include <spdlog.h>
#include <stack>
#include <stdexcept>
#include <string>
#include <TlHelp32.h>
#include <toml.hpp>
#include <vector>
#include <windows.h>
#include <wrl.h>

#include "framework.hpp"
#include "Resource.h"

namespace BF
{
	namespace filesystem = std::filesystem;
	using namespace std::ranges;
	using std::cout;
	using std::endl;
	using std::exception;
	using std::function;
	using std::make_shared;
	using std::map;
	using std::ofstream;
	using std::set;
	using std::shared_ptr;
	using std::stack;
	using std::static_pointer_cast;
	using std::string;
	using std::to_string;
	using std::to_wstring;
	using std::vector;
	using std::wcout;
	using std::wstring;

	constexpr auto MAX_MENU_ITEM = 100000;
	const wstring  OverlayTitle  = L"Blue-Flag\'s External Menu";		// 标题

	constexpr unsigned int joaat(const string& str);
	constexpr unsigned int joaat(const wstring& str);

	void AllocCon();

	string  to_string(const wstring& ws);
	wstring to_wstring(const string& s);

	int StrToVK(const string& str);
}

using namespace BF;
