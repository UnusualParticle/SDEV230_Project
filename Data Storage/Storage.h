#pragma once

#include <string>
#include <vector>

namespace dat
{
	struct Product
	{
		std::string name{};
		int price{};
	};
	using CartItem = std::pair<const Product*, int>;
	class Cart
	{
	private:
		std::vector<CartItem> m_items;

		std::vector<CartItem>::iterator _find(const Product* prod);
		std::vector<CartItem>::const_iterator _find(const Product* prod) const;
	public:
		const std::vector<CartItem>& getItems() const;
		int getCount(const Product* prod) const;
		void editItem(const Product* prod, int count);
		int getTotal() const;
	};
	struct Menu
	{
		std::string title{};
		std::vector<const Product*> products{};
		std::vector<Menu*> menus{};
	};
	
	using v_prod = std::vector<Product>;
	using v_menu = std::vector<Menu>;
	size_t LoadProducts(v_prod& _prods);
	size_t LoadMenus(v_menu& _menus, const v_prod& _prods);
}