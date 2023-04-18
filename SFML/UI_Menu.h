#pragma once

#include "UserInterface.h"
#include <vector>

namespace ui
{
	class Product : Element_Base
	{
	private:
		static ElementStyle s_item;
		static ElementStyle s_add;
		static ElementStyle s_remove;

		Element m_item{ {},{},{}, s_item };
		Element m_add{ {},{},{}, s_add };
		Element m_remove{ {},{},{}, s_remove };
	public:
		Product() = default;
		~Product() = default;

		void draw(sf::RenderTarget&, sf::RenderStates) const;
		void pollEvent(const sf::Event&);
		static void SetStyles(const ElementStyle& itemStyle, const ElementStyle& addStyle, const ElementStyle& removeStyle);

		bool AddClicked() const;
		bool RemoveClicked() const;
	};
	using ProdList_t = std::vector<Product>;

	class Menu : Element_Base
	{
	private:
		static ElementStyle s_title;
		static ElementStyle s_back;

		Element m_title{};
		Element m_back{};
		ProdList_t m_items{};
	public:
		Menu() = default;
		~Menu() = default;

		void draw(sf::RenderTarget&, sf::RenderStates) const;
		void pollEvent(const sf::Event&);
		static void SetStyles(const ElementStyle& titleStyle, const ElementStyle& backStyle);

		void AddProduct(const Product&);
		bool BackClicked() const;
	};
}