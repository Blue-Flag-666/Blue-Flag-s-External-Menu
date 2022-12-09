// Blue-Flag's External Menu.cpp

#include "pch.hpp"
#include "Blue-Flag's External Menu.hpp"
#include "Memory.hpp"
#include "Menu.hpp"
#include "Renderer.hpp"
#include "Settings.hpp"

// 全局变量:
HINSTANCE             hInst;											// 当前实例
HWND                  OverlayHWND, TargetHWND;
Memory                GTA5;
Settings              settings;
Tabs                  tabs;
shared_ptr <Renderer> renderer;
int                   cur_tab = 0;

int APIENTRY wWinMain(_In_ const HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ const LPWSTR lpCmdLine,_In_ const int nShowCmd)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	ios::sync_with_stdio(false);
	ios_base::sync_with_stdio(false);
	cout.tie(nullptr);
	wcout.tie(nullptr);

	setlocale(LC_ALL, "chs");

	settings = Settings(L"Blue-Flag.toml");

	ParseCmdLine(lpCmdLine);

	if (!settings.SkipMemInit)
	{
		GTA5 = Memory(L"GTA5.exe");
	}

	MyRegisterClass(hInstance);

	TargetHWND = GTA5.hwnd();
	if (!settings.SkipMemInit && !TargetHWND)
	{
		return FALSE;
	}

	InitMenu(tabs, settings);

	// 执行应用程序初始化:
	if (!InitInstance(hInstance, nShowCmd))
	{
		return FALSE;
	}

	if (!settings.NoUI)
	{
		settings.UseOldD3D ? renderer = make_shared <RendererD3D9>(OverlayHWND, TargetHWND, settings) : renderer = make_shared <RendererD3D12>(OverlayHWND, TargetHWND, settings);
	}

	MoveWindow(OverlayHWND, 0, 0, settings.OverlayWidth, settings.OverlayHeight, true);

	CreateThread(nullptr, 0, KeysThread, nullptr, 0, nullptr);
	Sleep(100);
	CreateThread(nullptr, 0, FuncThread, nullptr, 0, nullptr);

	MSG msg;

	// 主消息循环:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast <int>(msg.wParam);
}

//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(const HINSTANCE hInstance)
{
	const WNDCLASSEXW wcex { sizeof(WNDCLASSEX),CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, hInstance,LoadIcon(hInstance,MAKEINTRESOURCE(IDI_BLUEFLAGSEXTERNALMENU)),LoadCursor(nullptr,IDC_ARROW), CreateSolidBrush(RGB(0, 0, 0)), OverlayTitle.c_str(), OverlayTitle.c_str(), nullptr };

	return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(const HINSTANCE hInstance, const int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	OverlayHWND = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW, OverlayTitle.c_str(), OverlayTitle.c_str(), WS_POPUP, 1, 1,CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!OverlayHWND)
	{
		return FALSE;
	}

	SetLayeredWindowAttributes(OverlayHWND, RGB(0, 0, 0), 0, LWA_COLORKEY);

	ShowWindow(OverlayHWND, nCmdShow);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam)
{
	switch (message)
	{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			if (!settings.NoUI)
			{
				renderer->render();
			}
			if (settings.ConsoleDebug)
			{
				ShowConsoleDebugMenu();
			}
			EndPaint(hWnd, &ps);
			break;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			break;
		}
		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	return 0;
}

void ParseCmdLine(const LPWSTR lpCmdLine, bool usingDefaultConfig, const bool secondTry)
{
	int           cnt       = 0;
	const LPWSTR* szArgList = CommandLineToArgvW(lpCmdLine, &cnt);
	for (int i = 0; i < cnt; i++)
	{
		if (!usingDefaultConfig && !wcscmp(szArgList[i], L"--default-config"))
		{
			MessageBox(nullptr, L"Using Default Config", OverlayTitle.c_str(), MB_OK);
			AllocCon();
			usingDefaultConfig = true;
			break;
		}
		if (!wcscmp(szArgList[i], L"--console-debug"))
		{
			MessageBox(nullptr, L"Console Debug Mode Enabled", OverlayTitle.c_str(), MB_OK);
			AllocCon();
			settings.ConsoleDebug = true;
		}
		if (!wcscmp(szArgList[i], L"--console"))
		{
			AllocCon();
		}
		if (!wcscmp(szArgList[i], L"--no-ui"))
		{
			MessageBox(nullptr, L"UI Disabled", OverlayTitle.c_str(), MB_OK);
			settings.NoUI = true;
		}
		if (!wcscmp(szArgList[i], L"--skip-memory-init"))
		{
			MessageBox(nullptr, L"Memory Init Skipped", OverlayTitle.c_str(), MB_OK);
			settings.AlwaysShow  = true;
			settings.SkipMemInit = true;
		}
	}
	if (usingDefaultConfig && !secondTry)
	{
		settings.initDefault();
		ParseCmdLine(lpCmdLine, true, true);
	}
}

void ShowConsoleDebugMenu()
{
	ClearConsole();

	const auto& menu  = tabs.tabs[cur_tab]->menu_stack.top();
	const auto& items = menu->getItems();

	cout << menu->getName() << endl;

	for (int i = 0; i < items.size(); i++)
	{
		cout << (i == menu->cur_item ? "> " : "  ") << items[i]->show();
	}
}

void RefreshMenu()
{
	RedrawWindow(OverlayHWND, nullptr, nullptr, RDW_INTERNALPAINT);
}

void KillMenu()
{
}

void MenuSelect()
{
	const auto& cur_menu = tabs.tabs[cur_tab]->menu_stack.top();
	if (cur_menu->getItems().empty())
	{
		return;
	}
	switch (const auto cur_item = cur_menu->getItems()[cur_menu->cur_item]; cur_item->getType())
	{
		case Action_t:
		{
			static_pointer_cast <Action>(cur_item)->Excute();
			break;
		}
		case Toggle_t:
		{
			const auto x = static_pointer_cast <Toggle>(cur_item);
			x->toggle();
			x->Excute();
			break;
		}
		case Range_int_t:
		{
			const auto x = static_pointer_cast <Range <int> >(cur_item);
			x->Excute();
			break;
		}
		case Range_float_t:
		{
			const auto x = static_pointer_cast <Range <float> >(cur_item);
			x->Excute();
			break;
		}
		case Menu_t:
		case Submenu_t:
		{
			tabs.tabs[cur_tab]->menu_stack.push(static_pointer_cast <Menu>(cur_item));
			break;
		}
		default: ;
	}
}

void MenuItemUp()
{
	const auto& cur_menu = tabs.tabs[cur_tab]->menu_stack.top();
	if (cur_menu->getItems().empty())
	{
		return;
	}
	cur_menu->cur_item = (static_cast <unsigned long long>(cur_menu->cur_item) - 1 + cur_menu->getItems().size()) % cur_menu->getItems().size() % MAX_MENU_ITEM;
}

void MenuItemDown()
{
	const auto& cur_menu = tabs.tabs[cur_tab]->menu_stack.top();
	if (cur_menu->getItems().empty())
	{
		return;
	}
	cur_menu->cur_item = (static_cast <unsigned long long>(cur_menu->cur_item) + 1) % cur_menu->getItems().size() % MAX_MENU_ITEM;
}

void MenuLeft()
{
	const auto& cur_menu = tabs.tabs[cur_tab]->menu_stack.top();
	switch (const auto cur_item = cur_menu->getItems()[cur_menu->cur_item]; cur_item->getType())
	{
		case Range_int_t:
		{
			const auto x = static_pointer_cast <Range <int> >(cur_item);
			x->left();
			break;
		}
		case Range_float_t:
		{
			const auto x = static_pointer_cast <Range <float> >(cur_item);
			x->left();
			break;
		}
		default: ;
	}
}

void MenuRight()
{
	const auto& cur_menu = tabs.tabs[cur_tab]->menu_stack.top();
	switch (const auto cur_item = cur_menu->getItems()[cur_menu->cur_item]; cur_item->getType())
	{
		case Range_int_t:
		{
			const auto x = static_pointer_cast <Range <int> >(cur_item);
			x->right();
			break;
		}
		case Range_float_t:
		{
			const auto x = static_pointer_cast <Range <float> >(cur_item);
			x->right();
			break;
		}
		default: ;
	}
}

void MenuTabLeft()
{
	cur_tab = (static_cast <unsigned long long>(cur_tab) - 1 + tabs.tabs.size()) % tabs.tabs.size() % MAX_MENU_ITEM;
}

void MenuTabRight()
{
	cur_tab = (static_cast <unsigned long long>(cur_tab) + 1) % tabs.tabs.size() % MAX_MENU_ITEM;
}

void MenuBack()
{
	if (tabs.tabs[cur_tab]->menu_stack.size() <= 1)
	{
		return;
	}
	tabs.tabs[cur_tab]->menu_stack.pop();
}

DWORD KeysThread(LPVOID lpParam)
{
	while (!settings.KillMenu)
	{
		if (_kbhit())
		{
			GTA5.CheckKeys(settings);
		}
		Sleep(1);
	}
	settings.KeysThreadKilled = true;
	return 0;
}

DWORD FuncThread(LPVOID lpParam)
{
	// TODO

	return 0;
}
