#include "UI_Menu.h"
#include <fstream>

namespace ui
{
	void Product::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(m_item, states);
		target.draw(m_add, states);
		target.draw(m_remove, states);
	}
	void Product::pollEvent(const sf::Event& event)
	{
		m_add.pollEvent(event);
		m_remove.pollEvent(event);
	}
	bool Product::AddClicked() const
	{
		return m_add.onClick();
	}
	bool Product::RemoveClicked() const
	{
		return m_remove.onClick();
	}
	void Product::setSize(Vector v)
	{
		if (v == Vector{ 0,0 })
			v = Vector{ Settings::windowSize.x,Settings::windowSize.y / 10 };

		m_item.setSize(v);
		Vector v_button{ v.y / 2,v.y / 2 };
		m_add.setSize(v_button);
		m_remove.setSize(v_button);

		setPosition(m_item.getPosition());
	}
	Vector Product::getSize() const
	{
		return m_item.getSize();
	}
	void Product::setPosition(Vector v)
	{
		m_item.setPosition(v);

		Vector v_button{
			m_item.getPosition().x + m_item.getSize().x - m_add.getSize().x,
			m_item.getPosition().y
		};
		m_add.setPosition({ v_button.x, v_button.y });
		m_remove.setPosition({ v_button.x, v_button.y + m_remove.getSize().y });
	}
	Vector Product::getPosition() const
	{
		return m_item.getPosition();
	}
	void Product::setName(const char* str)
	{
		m_item.setString(str);
	}

	void Product::SetStyles(const ElementStyle* itemStyle, const ElementStyle* addStyle, const ElementStyle* removeStyle)
	{
		m_item.assignStyle(itemStyle);
		m_add.assignStyle(addStyle);
		m_remove.assignStyle(removeStyle);
	}

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

	ProdList_t::const_iterator Menu::begin() const
	{
		return m_items.begin();
	}
	ProdList_t::const_iterator Menu::end() const
	{
		return m_items.end();
	}

	void Menu::AddProduct(const Product& product)
	{
		m_items.push_back(product);

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

	namespace TEST_MENU
	{
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
		ElementStyle makeStyle()
		{
			ElementStyle mystyle{};

			mystyle.regular.fillColor = sf::Color::Black;
			mystyle.hover.fillColor = sf::Color{ 100,100,100 };
			mystyle.click.fillColor = sf::Color{ 50,50,50 };
			mystyle.select.outlineColor = sf::Color{ 100,100,255 };
			mystyle.select.fillColor = sf::Color::Black;
			mystyle.select.outlineThickness = -3.f;
			mystyle.select.padding = Vector{ 10,10 };

			return mystyle;
		}
		void RunTest()
		{
			sf::RenderWindow window;
			windowSetup(window);
			initialize(window);

			ElementStyle mystyle{ makeStyle() };
			Menu myMenu{};
			//myMenu.Resize();
			myMenu.SetStyles(&mystyle, &mystyle, &mystyle);
			myMenu.SetTitle("Pete's Pizzaria");

			for (size_t i{}; i < 4; ++i)
			{
				Product myProduct{};
				myProduct.setName("Pizza Pizza");
				myProduct.SetStyles(&mystyle, &mystyle, &mystyle);
				myMenu.AddProduct(myProduct);
			}
			myMenu.Resize();

			Element element({ 100,100 }, { 100,100 }, "Hello World!", &mystyle);
			element.resetSize();
			bool exit{};
			while (!exit)
			{
				sf::Event event{};
				while (window.pollEvent(event))
				{
					bool mouseEvent{ isMouseEvent(window, event) };
					if (event.type == sf::Event::Closed || onEscapePress())
						exit = true;
					else if (mouseEvent)
					{
						element.pollEvent(event);
						myMenu.pollEvent(event);
					}
					for (const auto& p : myMenu)
					{
						if (p.AddClicked())
							int lmao{};
						if (p.RemoveClicked())
							int rofl{};
					}
				}

				window.clear();
				window.draw(myMenu);
				window.draw(element);
				window.display();
			}

			window.close();

		}
	}
}