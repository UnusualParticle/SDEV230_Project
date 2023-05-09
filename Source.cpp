#include <iostream>
#include <sstream>

#include "UI/SFML_test.h"
#include "UI/UserInterface.h"
#include "UI/UI_Menu.h"
#include "UI/Styles.h"
#include "Data Storage/DrinkLoader.h"
#include "Data Storage/Storage.h"

void TestDrinks()
{
    // put code here
    DrinkLoader loader;
    loader.LoadDrinks();
}

void SetupMenu(ui::Menu& _ui, const dat::Menu* _dat, const dat::Cart& _cart)
{
	_ui.SetTitle(_dat->title.c_str());
	_ui.SetStyles(&ui::getmenustyle(), &ui::getmenustyle(), &ui::getmenustyle());

	ui::MenuItem prod{true};
	ui::MenuItem menu{false};
	for (const auto& p : _dat->products)
	{
		std::ostringstream str{};
		str << p->name << " $" << p->price;
		prod.setName(str.str().c_str());
		prod.setCount(_cart.getCount(p));
		prod.SetStyles(&ui::getstyle(), &ui::getaddstyle(), &ui::getremovestyle(), &ui::getcountstyle());
		_ui.AddItem(prod);
	}
	for (const auto& m : _dat->menus)
	{
		menu.setName(m->title.c_str());
		menu.SetStyles(&ui::getstyle(), &ui::getmenustyle(), &ui::getmenustyle(), &ui::getmenustyle());
		_ui.AddItem(menu);
	}

	_ui.Resize();
}
void SetupCart(ui::Menu& _ui, const dat::Menu* _dat, const dat::Cart& _cart)
{
	_ui.SetTitle("Cart");
	_ui.SetStyles(&ui::getmenustyle(), &ui::getmenustyle(), &ui::getmenustyle());

	ui::MenuItem prod{ true };
	ui::MenuItem menu{ false };
	for (const auto& p : _cart.getItems())
	{
		std::ostringstream str{};
		str << p.first->name << " $" << p.first->price;
		prod.setName(str.str().c_str());
		prod.setCount(p.second);
		prod.SetStyles(&ui::getstyle(), &ui::getaddstyle(), &ui::getremovestyle(), &ui::getcountstyle());
		_ui.AddItem(prod);
	}
	for (const auto& m : _dat->menus)
	{
		menu.setName(m->title.c_str());
		menu.SetStyles(&ui::getstyle(), &ui::getmenustyle(), &ui::getmenustyle(), &ui::getmenustyle());
		_ui.AddItem(menu);
	}

	_ui.Resize();
}
struct MenuReturn_t
{
	const dat::Product* product{};
	int prodCount{};
	const dat::Menu* menu{};
};
MenuReturn_t RunMenu(sf::RenderWindow& window, const dat::Menu* _data, const dat::Cart& cart)
{
	bool _iscart{ _data->title == "Cart" };

	ui::Menu menu{};
	if (_iscart)
		SetupCart(menu, _data, cart);
	else
		SetupMenu(menu, _data, cart);
	size_t prodLimit{ menu.size() - _data->menus.size()};
	size_t menuLimit{ menu.size() };

	bool exit{};
	while (window.isOpen() && !exit)
	{
		sf::Event event{};
		while (window.pollEvent(event))
		{
			bool mouseEvent{ isMouseEvent(window, event) };
			if (event.type == sf::Event::Closed)
				return {};
			else if (onEscapePress())
				exit = true;
			else if (mouseEvent)
				menu.pollEvent(event);
			else if (event.type == sf::Event::Resized)
			{
				ui::Settings::updateWindow(window, event.size);
				menu.Resize();
			}

			if (mouseEvent)
			{
				for (size_t i{}; i < prodLimit; ++i)
				{
					const dat::Product* _p{};
					if (_iscart)
						_p = cart.getItems()[i].first;
					else
						_p = _data->products[i];

					if (menu[i].AddClicked())
						return { _p, 1, _data };
					else if (menu[i].RemoveClicked())
						return { _p, -1, _data };
				}
				for (size_t i{ prodLimit }; i < menuLimit; ++i)
				{
					if (menu[i].ItemClicked())
						return { nullptr, 0, _data->menus[i - prodLimit] };
				}
			}
		}

		window.clear();
		window.draw(menu);
		window.display();
	}

	return {};
}

int main()
{
	// put code here
	std::cout << "\n== Hello World! ==\n";
    TestDrinks();
	//SFML_TEST();
	//ui::TEST_UI::RunTest();
	ui::TEST_MENU::RunTest();

	sf::RenderWindow window{};

	dat::v_prod products{};
	dat::v_menu menus{};
	dat::Cart cart{};

	dat::LoadProducts(products);
	dat::LoadMenus(menus, products);
	if (menus.size() == 0)
	{
		std::cout << "== ERR: NO MENUS LOADED ==";
		return 1;
	}

	MenuReturn_t nextmenu{nullptr, 0, &menus[0]};
	while (nextmenu.menu)
	{
		nextmenu = RunMenu(window, nextmenu.menu, cart);
		if (nextmenu.prodCount != 0)
			cart.editItem(nextmenu.product, nextmenu.prodCount);
	}
	window.close();

	return 0;
}