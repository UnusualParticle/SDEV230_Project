#include <iostream>
#include <sstream>

#include "UI/SFML_test.h"
#include "UI/UserInterface.h"
#include "UI/UI_Menu.h"
#include "UI/Styles.h"
#include "Data Storage/DrinkLoader.h"
#include "Data Storage/Storage.h"

// Styles.cpp is not implimented yet
void LoadStyles()
{
	using namespace style;
	LoadStyle("button", "Data/style_button.json");
	LoadStyle("background", "Data/style_background.json");
}

void TestDrinks()
{
    // put code here
    DrinkLoader loader;
    loader.LoadDrinks();
}

/* THIS IS TEMPORARY */
const ui::ElementStyle & getmenustyle()
{
	static ui::ElementStyle mystyle{};
	static bool isSet{};
	if (!isSet)
	{
		mystyle.regular.textColor = sf::Color::Red;
		mystyle.regular.fillColor = sf::Color{ 0xb7b7b7ff };
		mystyle.regular.outlineColor = sf::Color{ 0xe5e5e5ff };
	}

	return mystyle;
}

void SetupMenu(ui::Menu& _ui, const dat::Menu& _dat, const dat::Cart& _cart)
{
	_ui.SetTitle(_dat.title.c_str());
	_ui.SetStyles(&getmenustyle(), &getmenustyle(), &getmenustyle());

	ui::MenuItem prod{true};
	ui::MenuItem menu{false};
	for (const auto& p : _dat.products)
	{
		std::ostringstream str{};
		str << p->name << " $" << p->price;
		prod.setName(str.str().c_str());
		prod.setCount(_cart.getCount(p));
		prod.SetStyles(&getmenustyle(), &getmenustyle(), &getmenustyle(), &getmenustyle());
		_ui.AddItem(prod);
	}
	for (const auto& m : _dat.menus)
	{
		menu.setName(m->title.c_str());
		menu.SetStyles(&getmenustyle(), &getmenustyle(), &getmenustyle(), &getmenustyle());
		_ui.AddItem(menu);
	}

	_ui.Resize();
}
struct MenuReturn_t
{
	dat::Product* product{};
	int prodCount{};
	const dat::Menu* menu{};
};
MenuReturn_t RunMenu(sf::RenderWindow& window, const dat::Menu* _data, dat::Cart& cart)
{
	ui::Menu menu{};
	SetupMenu(menu, *_data, cart);
	size_t prodLimit{ _data->products.size() };
	size_t menuLimit{ prodLimit + _data->menus.size() };

	bool exit{};
	while (window.isOpen() && !exit)
	{
		sf::Event event{};
		while (window.pollEvent(event))
		{
			bool mouseEvent{ isMouseEvent(window, event) };
			if (event.type == sf::Event::Closed)
				window.close();
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
					if (menu[i].AddClicked())
						return { _data->products[i], 1, _data };
					else if (menu[i].RemoveClicked())
						return { _data->products[i], -1, _data };
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

	std::vector<dat::Menu> menus{};
	std::vector<dat::Product> products{};
	dat::Cart cart{};

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


	return 0;
}