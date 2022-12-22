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
	using std::array;
	using std::cout;
	using std::endl;
	using std::exception;
	using std::floating_point;
	using std::function;
	using std::initializer_list;
	using std::integral;
	using std::ios;
	using std::ios_base;
	using std::make_pair;
	using std::make_shared;
	using std::map;
	using std::ofstream;
	using std::pair;
	using std::set;
	using std::shared_ptr;
	using std::stack;
	using std::static_pointer_cast;
	using std::string;
	using std::string_view;
	using std::to_string;
	using std::to_wstring;
	using std::vector;
	using std::wcout;
	using std::wstring;
	using std::wstring_view;

	constexpr auto MAX_MENU_ITEM = 100000;
	const wstring  OverlayTitle  = L"Blue-Flag\'s External Menu";		// 标题

	constexpr UINT joaat(string_view str);
	constexpr UINT joaat(wstring_view str);

	void AllocCon();
	void ClearConsole();

	string  to_string(wstring_view ws);
	wstring to_wstring(string_view s);

	int StrToVK(string_view str);
}

using namespace BF;
