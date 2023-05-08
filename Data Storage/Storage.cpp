#include "Storage.h"
#include "json.h"
#include <algorithm>
#include <fstream>

namespace dat
{
	std::vector<CartItem>::iterator Cart::_find(const Product* prod)
	{
		return std::find_if(m_items.begin(), m_items.end(), [&](const CartItem& i) {return i.first == prod; });
	}
	std::vector<CartItem>::const_iterator Cart::_find(const Product* prod) const
	{
		return std::find_if(m_items.begin(), m_items.end(), [&](const CartItem& i) {return i.first == prod; });
	}
	const std::vector<CartItem>& Cart::getItems() const { return m_items; };
	int Cart::getCount(const Product* prod) const
	{
		auto ptr{ _find(prod) };
		if (ptr != m_items.end())
			return ptr->second;
		else
			return 0;
	}
	void Cart::editItem(const Product* prod, int count)
	{
		auto ptr{ _find(prod) };
		if (ptr != m_items.end())
		{
			ptr->second += count;
			if (count <= 0)
				m_items.erase(ptr);
		}
		else if (count > 0)
			m_items.push_back({ prod, count });
	}
	int Cart::getTotal() const
	{
		int total{};
		for (auto& i : m_items)
			total += (i.first->price * i.second);
		return total;
	}

	using json = nlohmann::json;

	/*== products.txt ==
	* prod_pizzas.json
	* prod_sides.json
	* prod_drinks.json
	* prod_desserts.json
	*/

	Product* FindProduct(v_prod& _prods, std::string_view _str)
	{
		auto ptr{ std::find_if(_prods.begin(), _prods.end(), [&](const Product& p) { return p.name == _str; }) };
		if (ptr == _prods.end())
			return nullptr;

		return ptr._Ptr;
	}
	Menu* FindMenu(v_menu& _menus, std::string_view _str)
	{
		auto ptr{ std::find_if(_menus.begin(), _menus.end(), [&](const Menu& p) { return p.title == _str; }) };
		if (ptr == _menus.end())
			return nullptr;

		return ptr._Ptr;
	}
	size_t LoadProducts(v_prod& _prods)
	{
		// you don't have to do it like this
		// I just don't like big files lol

		std::ifstream file{ "products.txt" };
		std::string str{};
		std::getline(file, str);

		while (file && str.size())
		{
			std::ifstream j_file{ str };
			json j_obj{};
			j_file >> j_obj;
			j_file.close();

			// bla bla

			std::getline(file, str);
		}
		
		file.close();
		return _prods.size();
	}
	size_t LoadMenus(v_menu& _menus, const v_prod& _prods)
	{
		// you don't have to do it like this
		// I just don't like big files lol

		std::ifstream file{ "menus.txt" };
		json j_obj{};
		file >> j_obj;
		file.close();

		// I'd recommend getting all the menu titles first,
		// then going through and getting the products and menus in each menu
		// that way the order the menus are listed in the file doesn't matter
		
		return _prods.size();
	}
}