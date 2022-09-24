#pragma once

#include "pch.hpp"

#include <string>

using std::wstring;

namespace BF
{
	class Memory
	{
		wstring  ProcessName;
		HANDLE   ProcessHandle;
		UINT     ProcessID;
		UINT_PTR BaseAddr;
		UINT     Size;
		public:
			[[nodiscard]] wstring name() const
			{
				return ProcessName;
			}

			[[nodiscard]] HANDLE handle() const
			{
				return ProcessHandle;
			}

			[[nodiscard]] UINT pid() const
			{
				return ProcessID;
			}

			[[nodiscard]] UINT_PTR baseAddr() const
			{
				return BaseAddr;
			}

			[[nodiscard]] UINT size() const
			{
				return Size;
			}

			HWND hwnd() const
			{
				
			}

			explicit Memory(const std::wstring& name);
			Memory();
	};
}
