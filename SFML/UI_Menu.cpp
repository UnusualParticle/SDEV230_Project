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

		m_add.setSize({v.x/2,v.y/2});
		m_add.setPosition({ v.x - m_add.getSize().x, v.y });
		
		m_remove.setSize({ v.x / 2,v.y / 2 });
		m_remove.setPosition({ v.x - m_remove.getSize().x, v.y + m_remove.getSize().y });
	}
	Vector Product::getSize() const
	{
		return m_item.getSize();
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
	}
	void Menu::AddProduct(const Product& product)
	{
		m_items.push_back(product);
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
			mystyle.format |= Formats::fitText;
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