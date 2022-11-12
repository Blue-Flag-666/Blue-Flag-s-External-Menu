#pragma once

#include "pch.hpp"
#include "Settings.hpp"

namespace BF
{
	enum ItemType { Unknown=0b0, MenuItem_t=0b00001, Action_t=0b11, Toggle_t=0b101, Range_t=0b001001, Range_int_t=0b011001, Range_float_t=0b101001, Menu_t=0b1000001, Submenu_t=0b11000001 };

	class MenuItem
	{
		string name;
		int    type = MenuItem_t;
		public:
			[[nodiscard]] const string& getName() const
			{
				return name;
			}

			[[nodiscard]] const int& getType() const
			{
				return type;
			}

			[[nodiscard]] virtual string show() const
			{
				return "\t" + name + "\n";
			}

			MenuItem() = default;
			explicit MenuItem(const string& str, int t = MenuItem_t);
			virtual  ~MenuItem() = default;
	};

	class Action final : public MenuItem
	{
		function <void()> func;
		public:
			void Excute() const
			{
				func();
			}

			[[nodiscard]] virtual string show() const override
			{
				return "\t" + getName() + "\n";
			}

			Action() = default;
			explicit Action(const string& str, const function <void()>& fun);
	};

	class Toggle final : public MenuItem
	{
		function <void(Toggle&)> func;
		bool                     on = false;
		public:
			[[nodiscard]] const bool& IsOn() const
			{
				return on;
			}

			bool toggle()
			{
				return on = !on;
			}

			void Excute()
			{
				func(*this);
			}

			[[nodiscard]] virtual string show() const override
			{
				return (on ? "[*]\t" : "[ ]\t") + getName() + "\n";
			}

			Toggle() = default;
			Toggle(const string& str, const function <void(Toggle&)>& fun);
	};

	template <typename T> requires std::integral <T> || std::floating_point <T>
	class Range final : public MenuItem
	{
		function <void(T&)> func;
		T                   cur, min, max, delta;
		public:
			void left()
			{
				cur = cur - delta;
				if (cur < min)
				{
					cur = min;
				}
			}

			void right()
			{
				cur = cur + delta;
				if (cur > max)
				{
					cur = max;
				}
			}

			void Excute()
			{
				func(cur);
			}

			[[nodiscard]] T& value()
			{
				return cur;
			}

			[[nodiscard]] virtual string show() const override
			{
				return "\t" + getName() + "\t< " + to_string(cur) + " >\n";
			}

			Range(const string& str, T init, T mi, T ma, T d, const function <void(T&)>& fun);
	};

	class Submenu;

	class Menu : public MenuItem
	{
		vector <shared_ptr <MenuItem> > items {};
		public:
			int cur_item = 0;

			[[nodiscard]] const vector <shared_ptr <MenuItem> >& getItems()
			{
				return items;
			}

			void clear()
			{
				items.clear();
			}

			[[nodiscard]] virtual string show() const override
			{
				return "\t" + getName() + "\t>>\n";
			}

			void add_action(const string& str, const function <void()>& fun);
			void add_toggle(const string& str, const function <void(Toggle&)>& fun);
			template <typename T> requires std::integral <T> || std::floating_point <T>
			void                 add_range(const string& str, T init, T mi, T ma, T d, const function <void(T&)>& fun);
			shared_ptr <Submenu> add_submenu(const string& str, const function <void()>& fun = nullptr);

			Menu() = default;
			explicit Menu(const string& str, int t = Menu_t);
	};

	class Submenu final : public Menu
	{
		function <void()> func;
		public:
			Submenu() = default;
			explicit Submenu(const string& str, const function <void()>& fun);
	};

	struct MenuTab
	{
		stack <shared_ptr <Menu> > menu_stack {};

		MenuTab() = default;
		explicit MenuTab(shared_ptr <Menu> m);
	};

	void InitMenu(vector <shared_ptr <MenuTab> >& tabs, const Settings& settings);

	template <typename T> ItemType getTypeName()
	{
		return Unknown;
	}

	template <> inline ItemType getTypeName <MenuItem>()
	{
		return MenuItem_t;
	}

	template <> inline ItemType getTypeName <Action>()
	{
		return Action_t;
	}

	template <> inline ItemType getTypeName <Toggle>()
	{
		return Toggle_t;
	}

	template <> inline ItemType getTypeName <Range <int> >()
	{
		return Range_int_t;
	}

	template <> inline ItemType getTypeName <Range <float> >()
	{
		return Range_float_t;
	}

	template <> inline ItemType getTypeName <Menu>()
	{
		return Menu_t;
	}

	template <> inline ItemType getTypeName <Submenu>()
	{
		return Submenu_t;
	}
}
