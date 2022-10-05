// Blue-Flag's External Menu.cpp : 定义应用程序的入口点。
//

#include "pch.hpp"

// 全局变量:
HINSTANCE hInst;											// 当前实例
wstring   OverlayTitle = L"Blue-Flag\'s External Menu";		// 标题
HWND      OverlayHWND, TargetHWND;

BF::Memory       GTA5;
BF::Settings     settings;
BF::RendererD3D9 renderer;

vector <BF::MenuTab*> tabs;
int                   cur_tab = 1;

int APIENTRY wWinMain(_In_ const HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ const LPWSTR lpCmdLine,_In_ const int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	settings = BF::Settings("Blue-Flag.toml");

	int           cnt;
	const LPWSTR* szArgList = CommandLineToArgvW(lpCmdLine, &cnt);
	for (int i = 0; i < cnt; i++)
	{
		OutputDebugString(szArgList[i]);
		if (!wcscmp(szArgList[i], L"--console-debug"))
		{
			MessageBox(nullptr, L"Console Debug Mode Enabled", L"Blue-Flag\'s External Menu", MB_OK);
			AllocCon();
			settings.ConsoleDebug = true;
		}
		if (!wcscmp(szArgList[i], L"--console"))
		{
			AllocCon();
		}
		if (!wcscmp(szArgList[i], L"--no-ui"))
		{
			MessageBox(nullptr, L"UI Disabled", L"Blue-Flag\'s External Menu", MB_OK);
			settings.NoUI = true;
		}
		if (!wcscmp(szArgList[i], L"--skip-memory-init"))
		{
			MessageBox(nullptr, L"Memory Init Skipped", L"Blue-Flag\'s External Menu", MB_OK);
			settings.SkipMemInit = true;
		}
	}

	if (!settings.SkipMemInit)
	{
		GTA5 = BF::Memory(L"GTA5.exe");
	}

	// TODO: 在此处放置代码

	MyRegisterClass(hInstance);

	TargetHWND = GTA5.hwnd();
	if (!settings.SkipMemInit && !TargetHWND)
	{
		return FALSE;
	}

	InitMenu(tabs, settings);

	// 执行应用程序初始化:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	if (!settings.NoUI)
	{
		renderer = BF::RendererD3D9(OverlayHWND, TargetHWND, settings.OverlayWidth, settings.OverlayHeight, settings);
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
				renderer.render();
			}
			if (settings.ConsoleDebug)
			{
				system("cls");  // NOLINT(concurrency-mt-unsafe)

				const auto menu  = tabs[cur_tab]->menu_stack.top();
				const auto items = menu->getItems();

				cout << menu->getName() << endl;
				for (int i = 0; i < items.size(); i++)
				{
					const auto x = items[i];
					cout << (i == menu->cur_item ? ">" : "") << "\t" << x->getName() << (x->getType() == BF::Menu_t || x->getType() == BF::Submenu_t ? ">>" : "") << endl;
				}
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

void MenuSelect()
{
	const auto cur_menu = tabs[cur_tab]->menu_stack.top();
	if (cur_menu->getItems().empty())
	{
		return;
	}
	switch (const auto cur_item = cur_menu->getItems()[cur_menu->cur_item]; cur_item->getType())
	{
		case BF::Menu_t:
		case BF::Submenu_t:
		{
			tabs[cur_tab]->menu_stack.push(static_cast <BF::Menu*>(cur_item));
			break;
		}
		case BF::Action_t:
		{
			static_cast <BF::Action*>(cur_item)->Excute();
		}
		default: ;
	}
}

void RefreshMenu()
{
	RedrawWindow(OverlayHWND, nullptr, nullptr, RDW_INTERNALPAINT);
}

void MenuItemUp()
{
	const auto cur_menu = tabs[cur_tab]->menu_stack.top();
	if (cur_menu->getItems().empty())
	{
		return;
	}
	cur_menu->cur_item = (cur_menu->cur_item - 1 + cur_menu->getItems().size()) % cur_menu->getItems().size();
}

void MenuItemDown()
{
	const auto cur_menu = tabs[cur_tab]->menu_stack.top();
	if (cur_menu->getItems().empty())
	{
		return;
	}
	cur_menu->cur_item = (cur_menu->cur_item + 1) % cur_menu->getItems().size();
}

void MenuTabLeft()
{
	cur_tab = (cur_tab - 1 + tabs.size()) % tabs.size();
}

void MenuTabRight()
{
	cur_tab = (cur_tab + 1) % tabs.size();
}

void MenuBack()
{
	if (tabs[cur_tab]->menu_stack.size() <= 1)
	{
		return;
	}
	tabs[cur_tab]->menu_stack.pop();
}

DWORD KeysThread(LPVOID lpParam)
{
	while (!settings.KillMenu)
	{
		GTA5.CheckKeys(settings);
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
