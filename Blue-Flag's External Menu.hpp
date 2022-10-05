#pragma once

#include "pch.hpp"

// 此代码模块中包含的函数的前向声明:
ATOM             MyRegisterClass(HINSTANCE hInstance);
BOOL             InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void MenuSelect();
void RefreshMenu();
void MenuItemUp();
void MenuItemDown();
void MenuTabLeft();
void MenuTabRight();
void MenuBack();

DWORD __stdcall KeysThread(LPVOID lpParam);
DWORD __stdcall FuncThread(LPVOID lpParam);
