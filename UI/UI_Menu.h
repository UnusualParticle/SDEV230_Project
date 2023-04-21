#pragma once

#include "UserInterface.h"
#include <vector>

namespace ui
{
	class MenuItem : public Element_Base
	{
	private:
		Element m_item{ {},{} };
		Element m_add{ {},{} };
		Element m_remove{ {},{} };
		Element m_count{ {},{} };

		bool m_isProduct{};
	public:
		MenuItem() = delete;
		MenuItem(bool isProduct = false);
		~MenuItem() = default;

		void draw(sf::RenderTarget&, sf::RenderStates) const;
		void pollEvent(const sf::Event&);

		bool ItemClicked() const;
		bool AddClicked() const;
		bool RemoveClicked() const;

		void setSize(Vector = {});
		Vector getSize() const;
		void setPosition(Vector);
		Vector getPosition() const;
		void setName(const char*);
		void setCount(int);
		void SetStyles(const ElementStyle* itemStyle, const ElementStyle* addStyle, const ElementStyle* removeStyle, const ElementStyle* countStyle);
	};

	using MenuList_t = std::vector<MenuItem>;

	class Menu : public Element_Base
	{
	private:
		Element m_background{ {},{} };
		Element m_title{ {},{} };
		Element m_back{ {},{} };
		MenuList_t m_items{};
	public:
		Menu() = default;
		~Menu() = default;

		void draw(sf::RenderTarget&, sf::RenderStates) const;
		void pollEvent(const sf::Event&);
		void SetStyles(const ElementStyle* backgroundStyle, const ElementStyle* titleStyle, const ElementStyle* backStyle);

		MenuList_t::const_iterator begin() const;
		MenuList_t::const_iterator end() const;
		size_t size() const;
		MenuItem& operator[](size_t);
		const MenuItem& operator[](size_t) const;

		void AddItem(const MenuItem&);
		bool BackClicked() const;

		void Resize();
		void SetTitle(const char*);
	};

	namespace TEST_MENU
	{
		void RunTest();
	}
}