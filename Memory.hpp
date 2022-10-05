#pragma once

#include "pch.hpp"
#include "Settings.hpp"

namespace BF
{
	class Trainer
	{
		public:
			clock_t keyTimer;

			bool         CheckKeyState(int key);
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

			[[nodiscard]] HWND hwnd() const
			{
				return static_cast <HWND>(ProcessHandle);
			}

			void CheckKeys(Settings& settings);

			Memory();
			explicit Memory(const wstring& name);
	};
}
