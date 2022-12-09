#include "pch.hpp"
#include "Memory.hpp"
#include "Blue-Flag's External Menu.hpp"

typedef struct ST_WNDINFO
{
	HWND  hWnd;
	DWORD dwProcessId;
}         WNDINFO,* LPWNDINFO;

union LPWNDINFO_t
{
	LPWNDINFO info;
	LPARAM    ptr;

	explicit LPWNDINFO_t(const LPARAM p): ptr(p)
	{
	}
};

HWND GetProcessMainWnd(const DWORD dwProcessId)
{
	WNDINFO wndInfo { nullptr, dwProcessId };
	EnumWindows
		([](const HWND hWnd, const LPARAM lParam) ->BOOL
		{
			DWORD ProcessId = 0;
			GetWindowThreadProcessId(hWnd, &ProcessId);
			if (const auto pInfo = LPWNDINFO_t(lParam).info; ProcessId == pInfo->dwProcessId)
			{
				pInfo->hWnd = hWnd;
				return FALSE;
			}
			return TRUE;
		}, reinterpret_cast <LPARAM>(&wndInfo));

	return wndInfo.hWnd;
}

BOOL ListSystemProcesses(WCHAR szExeFile[MAX_PATH], const LPPROCESSENTRY32 PE32)
{
	const HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapshot == INVALID_HANDLE_VALUE)
	{
		MessageBox(nullptr, L"CreateToolhelp32Snapshot (of processes)", L"Error",MB_ICONSTOP | MB_ICONERROR | MB_ICONHAND);
		return FALSE;
	}
	PE32->dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(snapshot, PE32))
	{
		MessageBox(nullptr, L"Process32First", L"Error",MB_ICONSTOP | MB_ICONERROR | MB_ICONHAND);
		CloseHandle(snapshot);
		return FALSE;
	}
	do
	{
		if (lstrcmp(szExeFile, PE32->szExeFile) == 0)
		{
			CloseHandle(snapshot);
			return TRUE;
		}
	}
	while (Process32Next(snapshot, PE32));
	CloseHandle(snapshot);
	return FALSE;
}

BOOL ListProcessModules(const uint32_t dwProcessId, const WCHAR szModule[MAX_MODULE_NAME32 + 1], const LPMODULEENTRY32 ME32)
{
	const HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcessId);
	if (snapshot == INVALID_HANDLE_VALUE)
	{
		MessageBox(nullptr, L"CreateToolhelp32Snapshot (of modules)", L"Error",MB_ICONSTOP | MB_ICONERROR | MB_ICONHAND);
		return FALSE;
	}
	ME32->dwSize = sizeof(MODULEENTRY32);
	if (!Module32First(snapshot, ME32))
	{
		MessageBox(nullptr, L"Module32First", L"Error",MB_ICONSTOP | MB_ICONERROR | MB_ICONHAND);
		CloseHandle(snapshot);
		return FALSE;
	}
	do
	{
		if (lstrcmp(szModule, ME32->szModule) == 0)
		{
			CloseHandle(snapshot);
			return TRUE;
		}
	}
	while (Module32Next(snapshot, ME32));
	CloseHandle(snapshot);
	return FALSE;
}

bool Trainer::CheckKeyState(const string_view key)
{
	if (clock() - keyTimer > 100 && GetAsyncKeyState(StrToVK(key)) & 0x8000)
	{
		keyTimer = clock();
		return true;
	}
	return false;
}

void Trainer::CheckKeys()
{
}

Trainer::Trainer()
{
	keyTimer = clock();
}

void Memory::CheckKeys(Settings& settings)
{
	if (CheckKeyState(settings.Keys["ToggleMenu"]))
	{
		settings.ActiveMenu = !settings.ActiveMenu;
	}
	else if (CheckKeyState(settings.Keys["MenuSelect"]))
	{
		MenuSelect();
	}
	else if (CheckKeyState(settings.Keys["MenuUp"]))
	{
		MenuItemUp();
	}
	else if (CheckKeyState(settings.Keys["MenuDown"]))
	{
		MenuItemDown();
	}
	else if (CheckKeyState(settings.Keys["MenuLeft"]))
	{
		MenuLeft();
	}
	else if (CheckKeyState(settings.Keys["MenuRight"]))
	{
		MenuRight();
	}
	else if (CheckKeyState(settings.Keys["MenuTabLeft"]))
	{
		MenuTabLeft();
	}
	else if (CheckKeyState(settings.Keys["MenuTabRight"]))
	{
		MenuTabRight();
	}
	else if (CheckKeyState(settings.Keys["MenuBack"]))
	{
		MenuBack();
	}
	else
	{
		return;
	}
	RefreshMenu();
}

Memory::Memory()
{
	ProcessName   = L"";
	ProcessID     = NULL;
	ProcessHandle = nullptr;
	ProcessHWND   = nullptr;
	BaseAddr      = NULL;
	Size          = NULL;
}

Memory::Memory(const wstring_view name)
{
	ProcessName = name;

	PROCESSENTRY32 PE32;
	ListSystemProcesses(const_cast <WCHAR*>(name.data()), &PE32);
	if (!ListSystemProcesses(const_cast <WCHAR*>(name.data()), &PE32))
	{
		throw exception("Failed to find process id");
	}
	ProcessID     = PE32.th32ProcessID;
	ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, false, ProcessID);
	ProcessHWND   = GetProcessMainWnd(ProcessID);
	if (!ProcessHandle)
	{
		throw exception("Failed to open handle");
	}

	MODULEENTRY32 ME32;
	if (!ListProcessModules(ProcessID, name.data(), &ME32))
	{
		throw exception("Failed to get model info");
	}

	BaseAddr = reinterpret_cast <uintptr_t>(ME32.modBaseAddr);
	Size     = ME32.modBaseSize;
}
