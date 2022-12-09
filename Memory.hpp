#pragma once

#include "pch.hpp"
#include "Settings.hpp"

namespace BF
{
	class Trainer
	{
		public:
			clock_t keyTimer;

			bool         CheckKeyState(string_view key);
			virtual void CheckKeys();

			Trainer& operator=(const Trainer&) = default;
			Trainer& operator=(Trainer&&)      = default;

			Trainer();
			Trainer(const Trainer&) = default;
			Trainer(Trainer&&)      = default;
			virtual ~Trainer()      = default;
	};

	class Memory final : public Trainer
	{
		wstring  ProcessName;
		HANDLE   ProcessHandle;
		HWND     ProcessHWND;
		UINT     ProcessID;
		UINT_PTR BaseAddr;
		UINT     Size;

		public:
			[[nodiscard]] wstring_view name() const
			{
				return ProcessName;
			}

			[[nodiscard]] HANDLE handle() const
			{
				return ProcessHandle;
			}

			[[nodiscard]] HWND hwnd() const
			{
				return ProcessHWND;
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

			void CheckKeys(Settings& settings);

			Memory();
			explicit Memory(wstring_view name);
	};
}
