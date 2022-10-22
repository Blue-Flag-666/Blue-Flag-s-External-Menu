#pragma once

#include "pch.hpp"

// 此代码模块中包含的函数的前向声明:
ATOM             MyRegisterClass(HINSTANCE hInstance);
BOOL             InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void ShowConsoleDebugMenu();
void RefreshMenu();

void KillMenu();
void MenuSelect();
void MenuItemUp();
void MenuItemDown();
void MenuLeft();
void MenuRight();
void MenuTabLeft();
void MenuTabRight();
void MenuBack();

DWORD __stdcall KeysThread(LPVOID lpParam);
DWORD __stdcall FuncThread(LPVOID lpParam);
