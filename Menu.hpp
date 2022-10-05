#pragma once

#include "pch.hpp"
#include "Settings.hpp"

namespace BF
{
	enum ItemType { MenuItem_t, Menu_t, Submenu_t, Action_t };

	class MenuItem
	{
		string name {};
		int    type {};
		public:
			[[nodiscard]] string getName() const
			{
				return name;
			}

			[[nodiscard]] int getType() const
			{
				return type;
			}

			MenuItem& operator=(const MenuItem&) = default;
			MenuItem& operator=(MenuItem&&)      = default;

			MenuItem()                = default;
			MenuItem(const MenuItem&) = default;
			MenuItem(MenuItem&&)      = default;
			explicit MenuItem(const string& str, int t = MenuItem_t);
			~MenuItem() = default;
	};

	class Submenu;

	class Menu : public MenuItem
	{
		vector <shared_ptr <MenuItem> > items {};
		public:
			int cur_item {};

			[[nodiscard]] vector <shared_ptr <MenuItem> > getItems()
			{
				return items;
			}

			shared_ptr <Submenu> add_submenu(const string& str, const function <void()>& fun = nullptr);
			void                 add_action(const string& str, const function <void()>& fun);
			void                 clear();

			Menu& operator=(const Menu&) = default;
			Menu& operator=(Menu&&)      = default;

			Menu()            = default;
			Menu(const Menu&) = default;
			Menu(Menu&&)      = default;
			explicit Menu(const string& str, int t = Menu_t);
			~Menu() = default;
	};

	class Submenu final : public Menu
	{
		function <void()> func;
		public:
			Submenu() = default;
			explicit Submenu(const string& str, const function <void()>& fun);
	};

	class Action final : public MenuItem
	{
		function <void()> func;
		public:
			void Excute() const;
			Action() = default;
			explicit Action(const string& str, const function <void()>& fun);
	};

	struct MenuTab
	{
		shared_ptr <Menu>          menu {};
		stack <shared_ptr <Menu> > menu_stack;
		MenuTab&                   operator=(const MenuTab&) = default;
		MenuTab&                   operator=(MenuTab&&)      = default;

		MenuTab()               = default;
		MenuTab(const MenuTab&) = default;
		MenuTab(MenuTab&&)      = default;
		explicit MenuTab(shared_ptr <Menu> m);
	};

	void InitMenu(vector <shared_ptr <MenuTab> >& tabs, const Settings& settings);
}
