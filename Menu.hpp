#pragma once

#include "pch.hpp"
#include "Settings.hpp"

namespace BF
{
	enum ItemType { MenuItem_t, Action_t, Toggle_t, Range_int_t, Range_float_t, Menu_t, Submenu_t };

	class MenuItem
	{
		string name { "" };
		int    type { MenuItem_t };
		public:
			[[nodiscard]] const string& getName() const
			{
				return name;
			}

			[[nodiscard]] const int& getType() const
			{
				return type;
			}

			MenuItem() = default;
			explicit MenuItem(const string& str, int t = MenuItem_t);
	};

	class Action : public MenuItem
	{
		function <void()> func;
		public:
			void Excute() const
			{
				func();
			}

			Action() = default;
			explicit Action(const string& str, const function <void()>& fun);
	};

	class Toggle : public MenuItem
	{
		function <void(Toggle&)> func;
		bool                     on { false };
		public:
			[[nodiscard]] const bool& IsOn() const
			{
				return on;
			}

			bool toggle()
			{
				on = !on;
				return on;
			}

			void Excute()
			{
				func(*this);
			}

			Toggle() = default;
			Toggle(const string& str, const function <void(Toggle&)>& fun);
	};

	template <typename T> class Range : public MenuItem
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

			Range(const string& str, T init, T mi, T ma, T d, function <void(T&)> fun);
	};

	class Submenu;

	class Menu : public MenuItem
	{
		vector <shared_ptr <MenuItem> > items {};
		public:
			int cur_item { 0 };

			[[nodiscard]] const vector <shared_ptr <MenuItem> >& getItems()
			{
				return items;
			}

			void clear()
			{
				items.clear();
			}

			void                    add_action(const string& str, const function <void()>& fun);
			void                    add_toggle(const string& str, const function <void(Toggle&)>& fun);
			template <typename T> void add_range(const string& str, T init, T mi, T ma, T d, function <void(T&)> fun);
			shared_ptr <Submenu>    add_submenu(const string& str, const function <void()>& fun = nullptr);

			Menu& operator=(const Menu&) = default;
			Menu& operator=(Menu&&)      = default;

			Menu()            = default;
			Menu(const Menu&) = default;
			Menu(Menu&&)      = default;
			explicit Menu(const string& str, int t = Menu_t);

			~Menu()
			{
				items.clear();
			}
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
}
