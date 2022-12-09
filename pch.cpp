// pch.cpp: 与预编译标头对应的源文件

#include "pch.hpp"

// 当使用预编译的头时，需要使用此源文件，编译才能成功。

namespace BF
{
	constexpr UINT joaat(const string_view str)
	{
		constexpr UINT MASK = 0xffffffff;
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
		constexpr UINT MASK = 0xffffffff;
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
		else if (str == "TAB")
		{
			return 0x09;
		}
		else if (str == "CLEAR")
		{
			return 0x0C;
		}
		else if (str == "ENTER" || str == "RETURN")
		{
			return 0x0D;
		}
		else if (str == "SHIFT")
		{
			return 0x10;
		}
		else if (str == "CTRL" || str == "CONTROL")
		{
			return 0x11;
		}
		else if (str == "ALT")
		{
			return 0x12;
		}
		else if (str == "PAUSE")
		{
			return 0x13;
		}
		else if (str == "CAPS" || str == "CAPSLOCK" || str == "CAPS LOCK" || str == "CAPITAL")
		{
			return 0x14;
		}
		else if (str == "ESC" || str == "ESCAPE")
		{
			return 0x1B;
		}
		else if (str == "SPACE")
		{
			return 0x20;
		}
		else if (str == "PGUP" || str == "PAGEUP")
		{
			return 0x21;
		}
		else if (str == "PGDN" || str == "PAGEDOWN")
		{
			return 0x22;
		}
		else if (str == "END")
		{
			return 0x23;
		}
		else if (str == "HOME")
		{
			return 0x24;
		}
		else if (str == "LEFT")
		{
			return 0x25;
		}
		else if (str == "UP")
		{
			return 0x26;
		}
		else if (str == "RIGHT")
		{
			return 0x27;
		}
		else if (str == "DOWN")
		{
			return 0x28;
		}
		else if (str == "INSERT")
		{
			return 0x2D;
		}
		else if (str == "DEL" || str == "DELETE")
		{
			return 0x2E;
		}
		else if (str == "0")
		{
			return 0x30;
		}
		else if (str == "1")
		{
			return 0x31;
		}
		else if (str == "2")
		{
			return 0x32;
		}
		else if (str == "3")
		{
			return 0x33;
		}
		else if (str == "4")
		{
			return 0x34;
		}
		else if (str == "5")
		{
			return 0x35;
		}
		else if (str == "6")
		{
			return 0x36;
		}
		else if (str == "7")
		{
			return 0x37;
		}
		else if (str == "8")
		{
			return 0x38;
		}
		else if (str == "9")
		{
			return 0x39;
		}
		else if (str == "A")
		{
			return 0x41;
		}
		else if (str == "B")
		{
			return 0x42;
		}
		else if (str == "C")
		{
			return 0x43;
		}
		else if (str == "D")
		{
			return 0x44;
		}
		else if (str == "E")
		{
			return 0x45;
		}
		else if (str == "F")
		{
			return 0x46;
		}
		else if (str == "G")
		{
			return 0x47;
		}
		else if (str == "H")
		{
			return 0x48;
		}
		else if (str == "I")
		{
			return 0x49;
		}
		else if (str == "J")
		{
			return 0x4A;
		}
		else if (str == "K")
		{
			return 0x4B;
		}
		else if (str == "L")
		{
			return 0x4C;
		}
		else if (str == "M")
		{
			return 0x4D;
		}
		else if (str == "N")
		{
			return 0x4E;
		}
		else if (str == "O")
		{
			return 0x4F;
		}
		else if (str == "P")
		{
			return 0x50;
		}
		else if (str == "Q")
		{
			return 0x51;
		}
		else if (str == "R")
		{
			return 0x52;
		}
		else if (str == "S")
		{
			return 0x53;
		}
		else if (str == "T")
		{
			return 0x54;
		}
		else if (str == "U")
		{
			return 0x55;
		}
		else if (str == "V")
		{
			return 0x56;
		}
		else if (str == "W")
		{
			return 0x57;
		}
		else if (str == "X")
		{
			return 0x58;
		}
		else if (str == "Y")
		{
			return 0x59;
		}
		else if (str == "Z")
		{
			return 0x5A;
		}
		else if (str == "NUM0")
		{
			return 0x60;
		}
		else if (str == "NUM1")
		{
			return 0x61;
		}
		else if (str == "NUM2")
		{
			return 0x62;
		}
		else if (str == "NUM3")
		{
			return 0x63;
		}
		else if (str == "NUM4")
		{
			return 0x64;
		}
		else if (str == "NUM5")
		{
			return 0x65;
		}
		else if (str == "NUM6")
		{
			return 0x66;
		}
		else if (str == "NUM7")
		{
			return 0x67;
		}
		else if (str == "NUM8")
		{
			return 0x68;
		}
		else if (str == "NUM9")
		{
			return 0x69;
		}
		else if (str == "NUM*")
		{
			return 0x6A;
		}
		else if (str == "NUM+")
		{
			return 0x6B;
		}
		else if (str == "NUM-")
		{
			return 0x6D;
		}
		else if (str == "NUM." || str == "NUMDEL")
		{
			return 0x6E;
		}
		else if (str == "NUM/")
		{
			return 0x6F;
		}
		else if (str == "F1")
		{
			return 0x70;
		}
		else if (str == "F2")
		{
			return 0x71;
		}
		else if (str == "F3")
		{
			return 0x72;
		}
		else if (str == "F4")
		{
			return 0x73;
		}
		else if (str == "F5")
		{
			return 0x74;
		}
		else if (str == "F6")
		{
			return 0x75;
		}
		else if (str == "F7")
		{
			return 0x76;
		}
		else if (str == "F8")
		{
			return 0x77;
		}
		else if (str == "F9")
		{
			return 0x78;
		}
		else if (str == "F10")
		{
			return 0x79;
		}
		else if (str == "F11")
		{
			return 0x7A;
		}
		else if (str == "F12")
		{
			return 0x7B;
		}
		else if (str == "NUMLOCK")
		{
			return 0x90;
		}
		else if (str == "SCROLL" || str == "SCROLLLOCK")
		{
			return 0x91;
		}
		else if (str == "LSHIFT")
		{
			return 0xA0;
		}
		else if (str == "RSHIFT")
		{
			return 0xA1;
		}
		else if (str == "LCTRL" || str == "LCONTROL")
		{
			return 0xA2;
		}
		else if (str == "RCTRL" || str == "RCONTROL")
		{
			return 0xA3;
		}
		else if (str == "LALT")
		{
			return 0xA4;
		}
		else if (str == "RALT")
		{
			return 0xA5;
		}
		return 0xFF;
	}
}
