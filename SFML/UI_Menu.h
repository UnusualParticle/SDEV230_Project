#pragma once

#include "UserInterface.h"
#include <vector>

namespace ui
{
	class Product : public Element_Base
	{
	private:
		Element m_item{};
		Element m_add{};
		Element m_remove{};
	public:
		Product() = default;
		~Product() = default;

		void draw(sf::RenderTarget&, sf::RenderStates) const;
		void pollEvent(const sf::Event&);
		void SetStyles(const ElementStyle* itemStyle, const ElementStyle* addStyle, const ElementStyle* removeStyle);

		bool AddClicked() const;
		bool RemoveClicked() const;
		void setSize(Vector = {});
		Vector getSize() const;
	};

	using ProdList_t = std::vector<Product>;

	class Menu : public Element_Base
	{
	private:
		Element m_background{};
		Element m_title{};
		Element m_back{};
		ProdList_t m_items{};
	public:
		Menu() = default;
		~Menu() = default;

		void draw(sf::RenderTarget&, sf::RenderStates) const;
		void pollEvent(const sf::Event&);
		void SetStyles(const ElementStyle* backgroundStyle, const ElementStyle* titleStyle, const ElementStyle* backStyle);

		void AddProduct(const Product&);
		bool BackClicked() const;

		void Resize();
		void SetTitle(const char*);
	};

	namespace TEST_MENU
	{
		void RunTest();
	}
}