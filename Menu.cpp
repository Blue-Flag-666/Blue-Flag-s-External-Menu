#include "pch.hpp"
#include "Menu.hpp"

MenuItem::MenuItem(const string_view str, const int t)
{
	name = str;
	type = t;
}

Action::Action(const string_view str, const function <void()>& fun) : MenuItem(str, Action_t)
{
	func = fun;
}

Toggle::Toggle(const string_view str, const function <void(Toggle&)>& fun): MenuItem(str, Toggle_t)
{
	func = fun;
	on   = false;
	Excute();
}

void Menu::add_action(const string_view str, const function <void()>& fun)
{
	items.emplace_back(make_shared <Action>(str, fun));
}

void Menu::add_toggle(const string_view str, const function <void(Toggle&)>& fun)
{
	items.emplace_back(make_shared <Toggle>(str, fun));
}

template <typename T> requires std::integral <T> || std::floating_point <T>
Range <T>::Range(const string_view str, T init, T mi, T ma, T d, const function <void(T&)>& fun): MenuItem(str, getTypeName <Range <T> >())
{
	cur   = init;
	min   = mi;
	max   = ma;
	delta = d;
	func  = fun;
}

template <typename T> requires std::integral <T> || std::floating_point <T>
void Menu::add_range(const string_view str, T init, T mi, T ma, T d, const function <void(T&)>& fun)
{
	items.emplace_back(make_shared <Range <T> >(str, init, mi, ma, d, fun));
}

shared_ptr <Submenu> Menu::add_submenu(const string_view str, const function <void()>& fun)
{
	const auto x = make_shared <Submenu>(str, fun);
	items.emplace_back(x);
	return x;
}

Menu::Menu(const string_view str, const int t) : MenuItem(str, t)
{
	cur_item = 0;
}

Submenu::Submenu(const string_view str, const function <void()>& fun) : Menu(str, Submenu_t)
{
	func = fun;
}

MenuTab::MenuTab(const shared_ptr <Menu> m)
{
	menu_stack.push(m);
}

shared_ptr <Menu> Tabs::add_tab(const string name)
{
	shared_ptr <Menu> menu(new Menu(name));
	tabs.emplace_back(make_shared <MenuTab>(menu));
	tabs.back()->menu_stack.emplace(menu);
	return menu;
}

void BF::InitMenu(Tabs& tabs, const Settings& settings)
{
	const auto t1  = tabs.add_tab("÷˜≤Àµ•");
	const auto tmp = t1->add_submenu("tmp");
	t1->add_action
		("add", [tmp]
		{
			for (int i = 1; i <= 100; i++)
			{
				tmp->add_action
					("1", []
					{
						cout << "11" << endl;
					});
			}
		});
	t1->add_action
		("clear", [tmp]
		{
			tmp->clear();
		});

	const auto t2 = tabs.add_tab("…Ë÷√");
}
