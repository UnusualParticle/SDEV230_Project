#include <iostream>
#include <iomanip>
#include <sstream>

#include "UI/SFML_test.h"
#include "UI/UserInterface.h"
#include "UI/UI_Menu.h"
#include "Data Storage/Storage.h"

std::ostream& operator<<(std::ostream& out, const dat::Product& prod)
{
	out << prod.name << " $" << std::fixed << std::setprecision(2) << prod.price;
	return out;
}
void addProduct(ui::Menu& _ui, const dat::Product* p, int count)
{
	static ui::MenuItem prod{ true };
	static bool done{};
	if (!done)
	{
		prod.SetStyles(&ui::getstyle(), &ui::getaddstyle(), &ui::getremovestyle(), &ui::getcountstyle());
		done = true;
	}

	std::ostringstream str{};
	str << *p;
	prod.setName(str.str().c_str());
	prod.setCount(count);

	_ui.AddItem(prod);
}
void addMenu(ui::Menu& _ui, const dat::Menu* m)
{
	static ui::MenuItem menu{ false };
	static bool done{};
	if (!done)
	{
		menu.SetStyles(&ui::getstyle(), &ui::getmenustyle(), &ui::getmenustyle(), &ui::getmenustyle());
		done = true;
	}

	menu.setName(m->title.c_str());
	_ui.AddItem(menu);
}
void addTotal(ui::Menu& _ui, const dat::Cart& _cart)
{
	dat::Menu menu{};
	dat::Product total{ "Total:",_cart.getTotal() };
	std::ostringstream str{};
	str << total;
	menu.title = str.str();
	addMenu(_ui, &menu);
}
void SetupMenu(ui::Menu& _ui, const dat::Menu* _dat, const dat::Cart& _cart)
{
	_ui.SetTitle(_dat->title.c_str());
	_ui.SetStyles(&ui::getmenustyle(), &ui::getmenustyle(), &ui::getmenustyle());

	ui::MenuItem prod{true};
	ui::MenuItem menu{false};
	for (const auto& p : _dat->products)
		addProduct(_ui, p, _cart.getCount(p));
	for (const auto& m : _dat->menus)
		addMenu(_ui, m);

	_ui.Resize();
}
void SetupCart(ui::Menu& _ui, const dat::Menu* _dat, const dat::Cart& _cart)
{
	_ui.SetTitle("Cart");
	_ui.SetStyles(&ui::getmenustyle(), &ui::getmenustyle(), &ui::getmenustyle());

	size_t curProd{};
	size_t maxProds{ 9 - _dat->menus.size() };

	ui::MenuItem prod{ true };
	ui::MenuItem menu{ false };
	for (const auto& p : _cart.getItems())
	{
		addProduct(_ui, p.first, p.second);
		++curProd;
		if (curProd >= maxProds)
			break;
	}
	for (const auto& m : _dat->menus)
		addMenu(_ui, m);
	addTotal(_ui, _cart);

	_ui.Resize();
}
void SetupCheckout(ui::Menu& _ui, const dat::Menu* _dat, const dat::Cart& _cart)
{
	_ui.SetTitle("Checkout");
	_ui.SetStyles(&ui::getmenustyle(), &ui::getmenustyle(), &ui::getmenustyle());

	addTotal(_ui, _cart);

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
	bool _ischeckout{ _data->title == "Checkout" };

	ui::Menu menu{};
	if (_iscart)
		SetupCart(menu, _data, cart);
	else if(_ischeckout)
		SetupCheckout(menu, _data, cart);
	else
		SetupMenu(menu, _data, cart);
	size_t menuLimit{ menu.size() };
	if (_iscart || _ischeckout)
		--menuLimit;
	size_t prodLimit{ menuLimit - _data->menus.size()};

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
	//SFML_TEST();
	//ui::TEST_UI::RunTest();
	//ui::TEST_MENU::RunTest();

	sf::RenderWindow window{};
	ui::windowSetup(window, "Pete's Pizzaria");
	ui::initialize(window);

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

	MenuReturn_t nextmenu{nullptr, 0, &menus.front()};
	while (nextmenu.menu)
	{
		nextmenu = RunMenu(window, nextmenu.menu, cart);
		if (nextmenu.prodCount != 0)
			cart.editItem(nextmenu.product, nextmenu.prodCount);
	}
	window.close();

	return 0;
}