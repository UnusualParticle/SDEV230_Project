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

	void Product::SetStyles(const ElementStyle& itemStyle, const ElementStyle& addStyle, const ElementStyle& removeStyle)
	{
		s_item = itemStyle;
		s_add = addStyle;
		s_remove = removeStyle;
	}

	void Menu::AddProduct(const Product& product)
	{
		m_items.push_back(product);
	}

	void Menu::SetStyles(const ElementStyle& titleStyle, const ElementStyle& backStyle)
	{
		s_title = titleStyle;
		s_back = backStyle;
	}
}