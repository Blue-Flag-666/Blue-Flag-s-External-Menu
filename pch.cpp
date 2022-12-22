// pch.cpp: 与预编译标头对应的源文件

#include "pch.hpp"

// 当使用预编译的头时，需要使用此源文件，编译才能成功。

namespace BF
{
	constexpr UINT joaat(const string_view str)
	{
		constexpr auto MASK = 0xffffffff;
		unsigned int   hash = 0;
		for_each(str, tolower);
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

	constexpr UINT joaat(const wstring_view str)
	{
		constexpr auto MASK = 0xffffffff;
		unsigned int   hash = 0;
		for_each(str, tolower);
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

	void AllocCon()
	{
		AllocConsole();
		FILE* stream;
		freopen_s(&stream, "CON", "r", stdin);			// NOLINT(cert-err33-c)
		freopen_s(&stream, "CON", "w", stdout);		// NOLINT(cert-err33-c)
		freopen_s(&stream, "CON", "w", stderr);		// NOLINT(cert-err33-c)
	}

	void ClearConsole()
	{
		const HANDLE               hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbi;

		if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
		{
			return;
		}

		const SMALL_RECT scrollRect { 0, 0, csbi.dwSize.X, csbi.dwSize.Y };
		const COORD      scrollTarget { 0, static_cast <short>(-csbi.dwSize.Y) };
		const CHAR_INFO  fill { ' ', csbi.wAttributes };

		ScrollConsoleScreenBuffer(hConsole, &scrollRect, nullptr, scrollTarget, &fill);

		csbi.dwCursorPosition.X = 0;
		csbi.dwCursorPosition.Y = 0;

		SetConsoleCursorPosition(hConsole, csbi.dwCursorPosition);
	}

	string to_string(const wstring_view ws)
	{
		return string { CW2A(ws.data()) };
	}

	wstring to_wstring(const string_view s)
	{
		return wstring { CA2W(s.data()) };
	}

	int StrToVK(const string_view str)
	{
		for_each(str, toupper);

		if (str == "BACKSPACE")
		{
			return 0x08;
		}
		if (str == "TAB")
		{
			return 0x09;
		}
		if (str == "CLEAR")
		{
			return 0x0C;
		}
		if (str == "ENTER" || str == "RETURN")
		{
			return 0x0D;
		}
		if (str == "SHIFT")
		{
			return 0x10;
		}
		if (str == "CTRL" || str == "CONTROL")
		{
			return 0x11;
		}
		if (str == "ALT")
		{
			return 0x12;
		}
		if (str == "PAUSE")
		{
			return 0x13;
		}
		if (str == "CAPS" || str == "CAPSLOCK" || str == "CAPS LOCK" || str == "CAPITAL")
		{
			return 0x14;
		}
		if (str == "ESC" || str == "ESCAPE")
		{
			return 0x1B;
		}
		if (str == "SPACE")
		{
			return 0x20;
		}
		if (str == "PGUP" || str == "PAGEUP")
		{
			return 0x21;
		}
		if (str == "PGDN" || str == "PAGEDOWN")
		{
			return 0x22;
		}
		if (str == "END")
		{
			return 0x23;
		}
		if (str == "HOME")
		{
			return 0x24;
		}
		if (str == "LEFT")
		{
			return 0x25;
		}
		if (str == "UP")
		{
			return 0x26;
		}
		if (str == "RIGHT")
		{
			return 0x27;
		}
		if (str == "DOWN")
		{
			return 0x28;
		}
		if (str == "INSERT")
		{
			return 0x2D;
		}
		if (str == "DEL" || str == "DELETE")
		{
			return 0x2E;
		}
		if (str == "0")
		{
			return 0x30;
		}
		if (str == "1")
		{
			return 0x31;
		}
		if (str == "2")
		{
			return 0x32;
		}
		if (str == "3")
		{
			return 0x33;
		}
		if (str == "4")
		{
			return 0x34;
		}
		if (str == "5")
		{
			return 0x35;
		}
		if (str == "6")
		{
			return 0x36;
		}
		if (str == "7")
		{
			return 0x37;
		}
		if (str == "8")
		{
			return 0x38;
		}
		if (str == "9")
		{
			return 0x39;
		}
		if (str == "A")
		{
			return 0x41;
		}
		if (str == "B")
		{
			return 0x42;
		}
		if (str == "C")
		{
			return 0x43;
		}
		if (str == "D")
		{
			return 0x44;
		}
		if (str == "E")
		{
			return 0x45;
		}
		if (str == "F")
		{
			return 0x46;
		}
		if (str == "G")
		{
			return 0x47;
		}
		if (str == "H")
		{
			return 0x48;
		}
		if (str == "I")
		{
			return 0x49;
		}
		if (str == "J")
		{
			return 0x4A;
		}
		if (str == "K")
		{
			return 0x4B;
		}
		if (str == "L")
		{
			return 0x4C;
		}
		if (str == "M")
		{
			return 0x4D;
		}
		if (str == "N")
		{
			return 0x4E;
		}
		if (str == "O")
		{
			return 0x4F;
		}
		if (str == "P")
		{
			return 0x50;
		}
		if (str == "Q")
		{
			return 0x51;
		}
		if (str == "R")
		{
			return 0x52;
		}
		if (str == "S")
		{
			return 0x53;
		}
		if (str == "T")
		{
			return 0x54;
		}
		if (str == "U")
		{
			return 0x55;
		}
		if (str == "V")
		{
			return 0x56;
		}
		if (str == "W")
		{
			return 0x57;
		}
		if (str == "X")
		{
			return 0x58;
		}
		if (str == "Y")
		{
			return 0x59;
		}
		if (str == "Z")
		{
			return 0x5A;
		}
		if (str == "NUM0")
		{
			return 0x60;
		}
		if (str == "NUM1")
		{
			return 0x61;
		}
		if (str == "NUM2")
		{
			return 0x62;
		}
		if (str == "NUM3")
		{
			return 0x63;
		}
		if (str == "NUM4")
		{
			return 0x64;
		}
		if (str == "NUM5")
		{
			return 0x65;
		}
		if (str == "NUM6")
		{
			return 0x66;
		}
		if (str == "NUM7")
		{
			return 0x67;
		}
		if (str == "NUM8")
		{
			return 0x68;
		}
		if (str == "NUM9")
		{
			return 0x69;
		}
		if (str == "NUM*")
		{
			return 0x6A;
		}
		if (str == "NUM+")
		{
			return 0x6B;
		}
		if (str == "NUM-")
		{
			return 0x6D;
		}
		if (str == "NUM." || str == "NUMDEL")
		{
			return 0x6E;
		}
		if (str == "NUM/")
		{
			return 0x6F;
		}
		if (str == "F1")
		{
			return 0x70;
		}
		if (str == "F2")
		{
			return 0x71;
		}
		if (str == "F3")
		{
			return 0x72;
		}
		if (str == "F4")
		{
			return 0x73;
		}
		if (str == "F5")
		{
			return 0x74;
		}
		if (str == "F6")
		{
			return 0x75;
		}
		if (str == "F7")
		{
			return 0x76;
		}
		if (str == "F8")
		{
			return 0x77;
		}
		if (str == "F9")
		{
			return 0x78;
		}
		if (str == "F10")
		{
			return 0x79;
		}
		if (str == "F11")
		{
			return 0x7A;
		}
		if (str == "F12")
		{
			return 0x7B;
		}
		if (str == "NUMLOCK")
		{
			return 0x90;
		}
		if (str == "SCROLL" || str == "SCROLLLOCK")
		{
			return 0x91;
		}
		if (str == "LSHIFT")
		{
			return 0xA0;
		}
		if (str == "RSHIFT")
		{
			return 0xA1;
		}
		if (str == "LCTRL" || str == "LCONTROL")
		{
			return 0xA2;
		}
		if (str == "RCTRL" || str == "RCONTROL")
		{
			return 0xA3;
		}
		if (str == "LALT")
		{
			return 0xA4;
		}
		if (str == "RALT")
		{
			return 0xA5;
		}
		return 0xFF;
	}
}
