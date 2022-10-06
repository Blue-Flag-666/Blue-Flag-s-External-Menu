#pragma once

#include "pch.hpp"
#include "Settings.hpp"

namespace BF
{
	enum ItemType { MenuItem_t, Menu_t, Submenu_t, Action_t, Toggle_t };

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
			void Excute() const;

			Action() = default;
			explicit Action(const string& str, const function <void()>& fun);
	};

	class Toggle : public MenuItem
	{
		function <void(Toggle&)> func;
		bool                     on { false };
		public:
			[[nodiscard]] const bool& IsOn() const;
			bool                      toggle();
			void                      Excute();

			Toggle() = default;
			Toggle(const string& str, const function <void(Toggle&)>& fun);
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

			void clear();

			void                 add_action(const string& str, const function <void()>& fun);
			void                 add_toggle(const string& str, const function <void(Toggle&)>& fun);
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
		shared_ptr <Menu>          menu {};
		stack <shared_ptr <Menu> > menu_stack {};

		MenuTab() = default;
		explicit MenuTab(shared_ptr <Menu> m);
	};

	void InitMenu(vector <shared_ptr <MenuTab> >& tabs, const Settings& settings);
}
