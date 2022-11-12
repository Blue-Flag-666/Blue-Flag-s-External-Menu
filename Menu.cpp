#include "pch.hpp"
#include "Menu.hpp"

MenuItem::MenuItem(const string& str, const int t)
{
	name = str;
	type = t;
}

Action::Action(const string& str, const function <void()>& fun) : MenuItem(str, Action_t)
{
	func = fun;
}

Toggle::Toggle(const string& str, const function <void(Toggle&)>& fun): MenuItem(str, Toggle_t)
{
	func = fun;
	on   = false;
	Excute();
}

void Menu::add_action(const string& str, const function <void()>& fun)
{
	items.emplace_back(make_shared <Action>(str, fun));
}

void Menu::add_toggle(const string& str, const function <void(Toggle&)>& fun)
{
	items.emplace_back(make_shared <Toggle>(str, fun));
}

template <typename T> requires std::integral <T> || std::floating_point <T>
Range <T>::Range(const string& str, T init, T mi, T ma, T d, const function <void(T&)>& fun): MenuItem(str, getTypeName <Range <T> >())
{
	cur   = init;
	min   = mi;
	max   = ma;
	delta = d;
	func  = fun;
}

template <typename T> requires std::integral <T> || std::floating_point <T>
void Menu::add_range(const string& str, T init, T mi, T ma, T d, const function <void(T&)>& fun)
{
	items.emplace_back(make_shared <Range <T> >(str, init, mi, ma, d, fun));
}

shared_ptr <Submenu> Menu::add_submenu(const string& str, const function <void()>& fun)
{
	const auto x = make_shared <Submenu>(str, fun);
	items.emplace_back(x);
	return x;
}

Menu::Menu(const string& str, const int t) : MenuItem(str, t)
{
	cur_item = 0;
}

Submenu::Submenu(const string& str, const function <void()>& fun) : Menu(str, Submenu_t)
{
	func = fun;
}

MenuTab::MenuTab(const shared_ptr <Menu> m)
{
	menu_stack.push(m);
}

void BF::InitMenu(vector <shared_ptr <MenuTab> >& tabs, const Settings& settings)
{
	shared_ptr <Menu> t1(new Menu("主菜单"));
	tabs.push_back(make_shared <MenuTab>(t1));
	tabs.back()->menu_stack.push(shared_ptr(t1));
	shared_ptr <Menu> t2(new Menu("次要菜单"));
	tabs.push_back(make_shared <MenuTab>(t2));
	tabs.back()->menu_stack.push(shared_ptr(t2));
	const auto m1  = t1->add_submenu("1");
	const auto m2  = t1->add_submenu("2");
	const auto m3  = t1->add_submenu("3");
	const auto m4  = m1->add_submenu("11");
	const auto tmp = t1->add_submenu("tmp");
	tmp->add_action
		("t", []
		{
			cout << "t" << endl;
		});
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
	m1->add_action
		("11", []
		{
			cout << "11" << endl;
		});
	m1->add_toggle
		("t1", [](const Toggle& toggle)
		{
			if (toggle.IsOn())
			{
				cout << "toggled!" << endl;
			}
		});
	const auto m5 = m2->add_submenu("21");
	const auto m6 = m5->add_submenu("51");
	m6->add_range <int>
		("int1", 5, 0, 10, 1, [](const int x)
		{
			cout << x << endl;
		});
}
