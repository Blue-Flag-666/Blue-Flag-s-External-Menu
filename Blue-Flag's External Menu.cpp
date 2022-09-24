// Blue-Flag's External Menu.cpp : 定义应用程序的入口点。
//

#include "pch.hpp"

// 全局变量:
HINSTANCE hInst;											// 当前实例
wstring   OverlayTitle = L"Blue-Flag\'s External Menu";		// 标题
HWND      OverlayHWND,   TargetHWND;
int       OverlayHeight, OverlayWidth;

BF::Memory GTA5;

// 此代码模块中包含的函数的前向声明:
ATOM             MyRegisterClass(HINSTANCE hInstance);
BOOL             InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain
(_In_ const HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ const int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	GTA5 = BF::Memory(L"GTA5.exe");
	// TODO: 在此处放置代码。

	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	MSG msg;

	// 主消息循环:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		MoveWindow(OverlayHWND, 0, 0, OverlayWidth, OverlayHeight,TRUE);
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
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style         = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc   = WndProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hInstance     = hInstance;
	wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BLUEFLAGSEXTERNALMENU));
	wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	wcex.lpszMenuName  = OverlayTitle.c_str();
	wcex.lpszClassName = OverlayTitle.c_str();
	wcex.hIconSm       = nullptr;

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

	OverlayHWND = CreateWindowExW(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED, OverlayTitle.c_str(), OverlayTitle.c_str(), WS_POPUP, 1, 1,CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
			HDC         hdc = BeginPaint(hWnd, &ps);
			// TODO: 在此处添加使用 hdc 的任何绘图代码...
			EndPaint(hWnd, &ps);
		}
		break;
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
