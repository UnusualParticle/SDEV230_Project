#include "UI_Menu.h"
#include <fstream>

// temporary
#include <utility>
#include <vector>

namespace ui
{
	MenuItem::MenuItem(bool isProduct)
		: m_isProduct(isProduct)
	{
		m_item.setStates(States::canHover | States::canClick);
		m_add.setStates(States::canHover | States::canClick);
		m_remove.setStates(States::canHover | States::canClick);
		m_count.setStates(0);
	}
	// virtual
	void MenuItem::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_item, states);
		if (m_isProduct)
		{
			target.draw(m_add, states);
			target.draw(m_remove, states);
			target.draw(m_count, states);
		}
	}
	void MenuItem::pollEvent(const sf::Event& event)
	{
		if (!m_isProduct)
			m_item.pollEvent(event);
		else
		{
			m_add.pollEvent(event);
			m_remove.pollEvent(event);
			//m_count.pollEvent(event);
		}
	}
	// onclick
	bool MenuItem::ItemClicked() const
	{
		if (m_isProduct)
			return false;
		return m_item.onClick();
	}
	bool MenuItem::AddClicked() const
	{
		if (!m_isProduct)
			return false;
		return m_add.onClick();
	}
	bool MenuItem::RemoveClicked() const
	{
		if (!m_isProduct)
			return false;
		return m_remove.onClick();
	}
	// get:set
	void MenuItem::setSize(Vector v)
	{
		if (v == Vector{ 0,0 })
			v = Vector{ Settings::windowSize.x,Settings::windowSize.y / 10 };

		m_item.setSize(v);
		Vector v_button{ v.y / 2,v.y / 2 };
		m_add.setSize(v_button);
		m_remove.setSize(v_button);
		m_count.setSize(v_button);

		setPosition(m_item.getPosition());
	}
	Vector MenuItem::getSize() const
	{
		return m_item.getSize();
	}
	void MenuItem::setPosition(Vector v)
	{
		m_item.setPosition(v);

		Vector v_button{
			m_item.getPosition().x + m_item.getSize().x - m_add.getSize().x,
			m_item.getPosition().y
		};
		m_add.setPosition({ v_button.x, v_button.y });
		m_remove.setPosition({ v_button.x, v_button.y + m_remove.getSize().y });
		m_count.setPosition({ v_button.x - m_remove.getSize().x, v_button.y + (m_remove.getSize().y / 2) });
	}
	Vector MenuItem::getPosition() const
	{
		return m_item.getPosition();
	}
	void MenuItem::setName(const char* str)
	{
		m_item.setString(str);
	}
	void MenuItem::setCount(int count)
	{
		m_count.setString(std::to_string(count));
	}
	void MenuItem::SetStyles(const ElementStyle* itemStyle, const ElementStyle* addStyle, const ElementStyle* removeStyle, const ElementStyle* countStyle)
	{
		m_item.assignStyle(itemStyle);
		m_add.assignStyle(addStyle);
		m_remove.assignStyle(removeStyle);
		m_count.assignStyle(countStyle);
	}
	// virtual
	void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_background, states);
		target.draw(m_back, states);
		target.draw(m_title, states);
		for (auto& i : m_items)
			target.draw(i, states);
	}
	void Menu::pollEvent(const sf::Event& event)
	{
		m_back.pollEvent(event);
		m_title.pollEvent(event);
		for (auto& i : m_items)
			i.pollEvent(event);
	}
	// vector
	MenuList_t::const_iterator Menu::begin() const
	{
		return m_items.begin();
	}
	MenuList_t::const_iterator Menu::end() const
	{
		return m_items.end();
	}
	size_t Menu::size() const { return m_items.size(); }
	MenuItem& Menu::operator[](size_t _idx) { return m_items[_idx]; }
	const MenuItem& Menu::operator[](size_t _idx) const { return m_items[_idx]; };
	void Menu::AddItem(const MenuItem& item)
	{
		m_items.push_back(item);

		Vector pos{};
		auto& p{ m_items.back() };
		if (m_items.size() > 1)
		{
			auto prev{ ++m_items.rbegin() };
			pos = prev->getPosition();
			pos.y += prev->getSize().y;
		}
		else
			pos = { 0,Settings::windowSize.y / 10 };
		p.setSize();
		p.setPosition(pos);
	}
	// set
	void Menu::SetStyles(const ElementStyle* backgroundStyle, const ElementStyle* titleStyle, const ElementStyle* backStyle)
	{
		m_background.assignStyle(backgroundStyle);
		m_title.assignStyle(titleStyle);
		m_back.assignStyle(backStyle);
	}
	void Menu::Resize()
	{
		m_background.setSize(Settings::windowSize);
		m_title.setSize({ Settings::windowSize.x, m_title.getSize().y });
		for(auto& i : m_items)
			i.setSize({ Settings::windowSize.x, i.getSize().y });
	}
	void Menu::SetTitle(const char* str)
	{
		m_title.setString(str);
	}


	void windowSetup(sf::Window& window)
	{
		unsigned int width{ 1280 };
		unsigned int height{ 720 };
		std::string title{ "Menu Elements work!" };

		window.create({ width, height }, title);
		ui::Settings::windowSize = sf::Vector2f(window.getSize());
	}
	void initialize(sf::RenderWindow& window)
	{
		ui::Settings::font.loadFromFile("data/arial.ttf");

		auto& w{ ui::Settings::windowSize };
		ui::Element loading{ {0,0},{w.x / 4,w.y / 4},"Loading..." };
		window.clear();
		window.draw(loading);
	}
	const ElementStyle& getstyle()
	{
		static ElementStyle mystyle{};
		static bool isSet{};
		if (!isSet)
		{
			mystyle.regular.textColor = sf::Color::Black;
			mystyle.regular.fillColor = sf::Color{ 0xb7b7b7ff };
			mystyle.regular.outlineColor = sf::Color{ 0xe5e5e5ff };

			mystyle.hover.textColor = sf::Color::Black;
			mystyle.hover.fillColor = sf::Color{ 0xe5e5e5ff };
			mystyle.hover.outlineColor = sf::Color{ 0xe5e5e5ff };

			mystyle.click.textColor = sf::Color::Black;
			mystyle.click.fillColor = sf::Color{ 0xefc2c2ff };
			mystyle.click.outlineColor = sf::Color{ 0xe5e5e5ff };
		}

		return mystyle;
	}
	const ElementStyle& getcountstyle()
	{
		static ElementStyle mystyle{};
		static bool isSet{};
		if (!isSet)
		{
			mystyle.regular.textColor = sf::Color::Black;
			mystyle.regular.fillColor = sf::Color{ 0xb7b7b7ff };
			mystyle.regular.outlineColor = sf::Color{ 0xe5e5e5ff };
			mystyle.regular.characterSize = 20;
		}

		return mystyle;
	}
	const ElementStyle& getmenustyle()
	{
		static ElementStyle mystyle{};
		static bool isSet{};
		if (!isSet)
		{
			mystyle.regular.textColor = sf::Color::Red;
			mystyle.regular.fillColor = sf::Color{ 0xb7b7b7ff };
			mystyle.regular.outlineColor = sf::Color{ 0xe5e5e5ff };
		}

		return mystyle;
	}
	const ElementStyle& getaddstyle()
	{
		static ElementStyle mystyle{};
		static bool isSet{};
		if (!isSet)
		{
			mystyle.regular.textColor = sf::Color::Black;
			mystyle.regular.fillColor = sf::Color{ 0x40f43dff };
			mystyle.regular.outlineColor = sf::Color{ 0xe5e5e5ff };

			mystyle.hover.textColor = sf::Color::Black;
			mystyle.hover.fillColor = sf::Color{ 0x76f774ff };
			mystyle.hover.outlineColor = sf::Color{ 0xe5e5e5ff };

			mystyle.click.textColor = sf::Color::Black;
			mystyle.click.fillColor = sf::Color{ 0x4cc94ff };
			mystyle.click.outlineColor = sf::Color{ 0xe5e5e5ff };
		}

		return mystyle;
	}
	const ElementStyle& getremovestyle()
	{
		static ElementStyle mystyle{};
		static bool isSet{};
		if (!isSet)
		{
			mystyle.regular.textColor = sf::Color::Black;
			mystyle.regular.fillColor = sf::Color{ 0xe84747ff };
			mystyle.regular.outlineColor = sf::Color{ 0xe5e5e5ff };

			mystyle.hover.textColor = sf::Color::Black;
			mystyle.hover.fillColor = sf::Color{ 0xe87a7aff };
			mystyle.hover.outlineColor = sf::Color{ 0xe5e5e5ff };

			mystyle.click.textColor = sf::Color::Black;
			mystyle.click.fillColor = sf::Color{ 0xc64d4dff };
			mystyle.click.outlineColor = sf::Color{ 0xe5e5e5ff };
		}

		return mystyle;
	}

	namespace TEST_MENU
	{

		enum class MOpt_t
		{
			none,
			pizzas,
			drinks,
			cart,
			back,
			cheesepizza,
			pepperonipizza,
			cocacola,
			rootbeer,
			checkout,
			TOTAL
		};
		const char* GetMenuStr(MOpt_t opt)
		{
			static char strlist[(size_t)MOpt_t::TOTAL][64]
			{
				"None",
				"Pizzas",
				"Drinks",
				"Cart",
				"Back",
				"Cheese Pizza",
				"Pepperoni Pizza",
				"Coca Cola",
				"Root Beer",
				"Checkout"
			};

			return strlist[(size_t)opt];
		}
		using CartItem_t = std::pair<MOpt_t, int>;
		using Cart_t = std::vector<CartItem_t>;
		void EditCart(Cart_t& cart, CartItem_t item)
		{
			auto ptr{ std::find_if(cart.begin(), cart.end(),
				[&](const CartItem_t& i)
				{ return i.first == item.first; }
			) };

			if (ptr != cart.end())
			{
				ptr->second += item.second;
				if(ptr->second<=0)
					cart.erase(ptr);

			}
			else if (item.second>0)
				cart.push_back(item);
		}
		CartItem_t GetCartItem(Cart_t& cart, MOpt_t option)
		{
			auto ptr{ std::find_if(cart.begin(), cart.end(),
				[&](const CartItem_t& i)
				{ return i.first == option; }
			) };

			if (ptr != cart.end())
				return *ptr;
			else
				return { option,0 };
		}

		void AddItemToMenu(Menu& menu, CartItem_t item, bool isProduct = true)
		{
			MenuItem menuitem{ isProduct };
			menuitem.setName(GetMenuStr(item.first));
			if (item.second != 0)
				menuitem.setCount(item.second);
			menuitem.SetStyles(&getstyle(), &getaddstyle(), &getremovestyle(), &getcountstyle());

			menu.AddItem(menuitem);
		}
		CartItem_t ParentMenu(sf::RenderWindow& window, Cart_t& cart)
		{
			Menu mymenu{};
			mymenu.SetTitle("Main Menu");
			mymenu.SetStyles(&getmenustyle(), &getmenustyle(), &getstyle());
			AddItemToMenu(mymenu, GetCartItem(cart, MOpt_t::pizzas), false);
			AddItemToMenu(mymenu, GetCartItem(cart, MOpt_t::drinks), false);
			AddItemToMenu(mymenu, GetCartItem(cart, MOpt_t::cart), false);
			mymenu.Resize();

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
						mymenu.pollEvent(event);
					else if (event.type == sf::Event::Resized)
					{
						Settings::updateWindow(window, event.size);
						mymenu.Resize();
					}

					for (size_t i{}; i < mymenu.size(); ++i)
					{
						if (mymenu[i].ItemClicked())
						{
							switch (i)
							{
							case 0:
								return { MOpt_t::pizzas, 0};
							case 1:
								return { MOpt_t::drinks, 0};
							case 2:
								return { MOpt_t::cart, 0 };
							}
						}
					}
				}

				window.clear();
				window.draw(mymenu);
				window.display();
			}
		}
		CartItem_t PizzaMenu(sf::RenderWindow& window, Cart_t& cart)
		{
			Menu mymenu{};
			mymenu.SetTitle("Pizzas");
			mymenu.SetStyles(&getmenustyle(), &getmenustyle(), &getstyle());
			AddItemToMenu(mymenu, GetCartItem(cart, MOpt_t::cheesepizza));
			AddItemToMenu(mymenu, GetCartItem(cart, MOpt_t::pepperonipizza));
			AddItemToMenu(mymenu, GetCartItem(cart, MOpt_t::back), false);
			mymenu.Resize();

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
						mymenu.pollEvent(event);
					else if (event.type == sf::Event::Resized)
					{
						Settings::updateWindow(window, event.size);
						mymenu.Resize();
					}

					for (size_t i{}; i < mymenu.size(); ++i)
					{
						if (mymenu[i].ItemClicked())
							return { MOpt_t::back, 0 };

						switch (i)
						{
						case 0:
							if (mymenu[i].AddClicked())
								return { MOpt_t::cheesepizza, 1 };
							if (mymenu[i].RemoveClicked())
								return { MOpt_t::cheesepizza, -1 };
							break;
						case 1:
							if (mymenu[i].AddClicked())
								return { MOpt_t::pepperonipizza, 1 };
							if (mymenu[i].RemoveClicked())
								return { MOpt_t::pepperonipizza, -1 };
							break;
						}
					}
				}

				window.clear();
				window.draw(mymenu);
				window.display();
			}

			return { MOpt_t::none,0 };
		}
		CartItem_t DrinksMenu(sf::RenderWindow& window, Cart_t& cart)
		{
			Menu mymenu{};
			mymenu.SetTitle("Drinks");
			mymenu.SetStyles(&getmenustyle(), &getmenustyle(), &getstyle());
			AddItemToMenu(mymenu, GetCartItem(cart, MOpt_t::cocacola));
			AddItemToMenu(mymenu, GetCartItem(cart, MOpt_t::rootbeer));
			AddItemToMenu(mymenu, GetCartItem(cart, MOpt_t::back), false);
			mymenu.Resize();

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
						mymenu.pollEvent(event);
					else if (event.type == sf::Event::Resized)
					{
						Settings::updateWindow(window, event.size);
						mymenu.Resize();
					}

					for (size_t i{}; i < mymenu.size(); ++i)
					{
						if (mymenu[i].ItemClicked())
							return { MOpt_t::back,0 };
						
						switch (i)
						{
						case 0:
							if (mymenu[i].AddClicked())
								return { MOpt_t::cocacola,1 };
							if (mymenu[i].RemoveClicked())
								return { MOpt_t::cocacola,-1 };
							break;
						case 1:
							if (mymenu[i].AddClicked())
								return { MOpt_t::rootbeer,1 };
							if (mymenu[i].RemoveClicked())
								return { MOpt_t::rootbeer,-1 };
							break;
						}
					}
				}

				window.clear();
				window.draw(mymenu);
				window.display();
			}
			return { MOpt_t::none,0 };
		}
		CartItem_t CartMenu(sf::RenderWindow& window, Cart_t& cart)
		{
			Menu mymenu{};
			mymenu.SetTitle("Cart");
			mymenu.SetStyles(&getmenustyle(), &getmenustyle(), &getstyle());

			for (size_t i{}; i < cart.size(); ++i)
				AddItemToMenu(mymenu, GetCartItem(cart, cart[i].first));
			AddItemToMenu(mymenu, GetCartItem(cart, MOpt_t::back), false);
			AddItemToMenu(mymenu, GetCartItem(cart, MOpt_t::checkout), false);
			mymenu.Resize();

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
						mymenu.pollEvent(event);
					else if (event.type == sf::Event::Resized)
					{
						Settings::updateWindow(window, event.size);
						mymenu.Resize();
					}

					for (size_t i{}; i < mymenu.size(); ++i)
					{
						if (mymenu[i].ItemClicked())
						{
							if (i == mymenu.size() - 2)
								return { MOpt_t::back,0 };
							else
								return { MOpt_t::checkout,0 };
						}
						if (mymenu[i].AddClicked())
							return { cart[i].first, 1 };
						if (mymenu[i].RemoveClicked())
							return { cart[i].first, -1 };
					}
				}

				window.clear();
				window.draw(mymenu);
				window.display();
			}
			return { MOpt_t::none,0 };
		}

		void RunTest()
		{
			sf::RenderWindow window;
			windowSetup(window);
			initialize(window);

			CartItem_t option{};
			Cart_t cart{};
			bool exit{};
			while (window.isOpen() && !exit)
			{
				sf::Event event{};
				while (window.pollEvent(event))
				{
					bool mouseEvent{ isMouseEvent(window, event) };
					if (event.type == sf::Event::Closed || onEscapePress())
						exit = true;
				}

				switch (option.first)
				{
				case MOpt_t::none:
				case MOpt_t::back:
				case MOpt_t::TOTAL:
					option = ParentMenu(window, cart);
					break;
				case MOpt_t::pizzas:
					option = PizzaMenu(window, cart);
					break;
				case MOpt_t::drinks:
					option = DrinksMenu(window, cart);
					break;
				case MOpt_t::cart:
					option = CartMenu(window, cart);
					if (option.second != 0)
					{
						EditCart(cart, option);
						option.first = MOpt_t::cart;
					}
					break;
				case MOpt_t::cheesepizza:
				case MOpt_t::pepperonipizza:
					EditCart(cart, option);
					option = PizzaMenu(window, cart);
					break;
				case MOpt_t::cocacola:
				case MOpt_t::rootbeer:
					EditCart(cart, option);
					option = DrinksMenu(window, cart);
					break;
				case MOpt_t::checkout:
					window.close();
					break;
				}

				window.clear();
			}

			window.close();
		}
	}
}