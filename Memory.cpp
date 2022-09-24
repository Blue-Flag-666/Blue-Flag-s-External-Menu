#include "pch.hpp"

constexpr UINT joaat(const std::string& str)
{
	constexpr UINT MASK = 0xffffffff;
	UINT           hash = 0;
	std::transform(str.begin(), str.end(), str.begin(), tolower);
	for (const auto i : str)
	{
		hash = hash + static_cast <unsigned char>(i);
		hash = hash + (hash << 10);
		hash = hash ^ (hash & MASK) >> 6;
	}
	hash = hash + (hash << 3);
	hash = hash ^ (hash & MASK) >> 11;
	hash = hash + (hash << 15);
	return hash & MASK;
}

constexpr UINT joaat(const std::wstring& str)
{
	constexpr UINT MASK = 0xffffffff;
	UINT           hash = 0;
	std::transform(str.begin(), str.end(), str.begin(), tolower);
	for (const auto i : str)
	{
		hash = hash + static_cast <unsigned char>(i);
		hash = hash + (hash << 10);
		hash = hash ^ (hash & MASK) >> 6;
	}
	hash = hash + (hash << 3);
	hash = hash ^ (hash & MASK) >> 11;
	hash = hash + (hash << 15);
	return hash & MASK;
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

BF::Memory::Memory(const wstring& name)
{
	ProcessName = name;

	PROCESSENTRY32 PE32;
	ListSystemProcesses(const_cast <WCHAR*>(name.c_str()), &PE32);
	if (!ListSystemProcesses(const_cast <WCHAR*>(name.c_str()), &PE32))
	{
		throw std::exception("Failed to find process id");
	}
	ProcessID     = PE32.th32ProcessID;
	ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);
	if (!ProcessHandle)
	{
		throw std::exception("Failed to open handle");
	}

	MODULEENTRY32 ME32;
	if (ListProcessModules(ProcessID, name.c_str(), &ME32) == FALSE)
	{
		throw std::exception("Failed to get model info");
	}

	BaseAddr = reinterpret_cast <uintptr_t>(ME32.modBaseAddr);
	Size     = ME32.modBaseSize;
}

BF::Memory::Memory()
{
	ProcessName   = L"";
	ProcessID     = NULL;
	ProcessHandle = nullptr;
	BaseAddr      = NULL;
	Size          = NULL;
}
