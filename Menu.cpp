#include "pch.hpp"

BF::MenuItem::MenuItem(const string& str, const int t)
{
	name = str;
	type = t;
}

BF::Submenu* BF::Menu::add_submenu(const string& str, const function <void()>& fun)
{
	const auto x = new Submenu(str, fun);
	items.push_back(x);
	return x;
}

void BF::Menu::add_action(const string& str, const function <void()>& fun)
{
	const auto x = new Action(str, fun);
	items.push_back(x);
}

void BF::Menu::clear()
{
	for (const auto x : items)
	{
		delete x;
	}
	items.clear();
}

BF::Menu::Menu(const string& str, const int t) : MenuItem(str, Menu_t)
{
	cur_item = 0;
}

BF::Menu::~Menu()
{
	clear();
}

BF::Submenu::Submenu(const string& str, const function <void()>& fun) : Menu(str, Submenu_t)
{
	func = fun;
}

void BF::Action::Excute() const
{
	func();
}

BF::Action::Action(const string& str, const function <void()>& fun) : MenuItem(str, Action_t)
{
	func = fun;
}

BF::MenuTab::MenuTab(Menu* m)
{
	menu = m;
}

BF::MenuTab::~MenuTab()
{
	delete menu;
}

void BF::InitMenu(vector <MenuTab*> &tabs, const Settings& settings)
{
	const auto t1 = new Menu("主菜单");
	tabs.push_back(new MenuTab(t1));
	tabs.back()->menu_stack.push(t1);
	const auto t2 = new Menu("次要菜单");
	tabs.push_back(new MenuTab(t2));
	tabs.back()->menu_stack.push(t2);
	const auto m1 = t1->add_submenu("1");
	const auto m2 = t1->add_submenu("2");
	const auto m3 = t1->add_submenu("3");
	const auto m4 = m1->add_submenu("11");
	m1->add_action
		("11", []
		{
			cout << "11" << endl;
		});
	const auto m5 = m2->add_submenu("21");
	const auto m6 = m5->add_submenu("51");
	m2->clear();
}
