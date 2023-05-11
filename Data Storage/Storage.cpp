#include "Storage.h"
#include "json.h"
#include <algorithm>
#include <fstream>
#include <memory>

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
	double Cart::getTotal() const
	{
		double total{};
		for (auto& i : m_items)
			total += (i.first->price * i.second);
		return total;
	}

	using json = nlohmann::json;

	const Product* FindProduct(const v_prod& _prods, std::string_view _str)
	{
		auto ptr{ std::find_if(_prods.begin(), _prods.end(), [&](const Product& p) { return p.name == _str; }) };
		if (ptr == _prods.end())
			return nullptr;

        return std::addressof(*ptr);
	}
	Menu* FindMenu(v_menu& _menus, std::string_view _str)
	{
		auto ptr{ std::find_if(_menus.begin(), _menus.end(), [&](const Menu& p) { return p.title == _str; }) };
		if (ptr == _menus.end())
			return nullptr;

		return std::addressof(*ptr);
	}
	size_t LoadProducts(v_prod& _prods)
	{
		// I'm just doing one file since they wont be "that" big

		std::ifstream file{ "Data/products.json" };

        json data = json::parse(file);

        json productsList = data["list"];

        for (auto& product : productsList)
        {
            Product prod{};
            prod.name = product["name"];
            prod.price = product["price"];
            _prods.push_back(prod);
        }

        file.close();

		return _prods.size();
	}
	size_t LoadMenus(v_menu& _menus, const v_prod& _prods)
	{
        std::ifstream file{ "Data/menus.json" };

        json data = json::parse(file);

        json menusList = data["list"];

        for (auto& menuJson : menusList)
        {
            Menu menu{};

            menu.title = menuJson["title"];

            for (auto& product : menuJson["products"])
            {
                std::string productString = product;
                const Product* prod = FindProduct(_prods, productString);
				assert(prod);
                menu.products.push_back(prod);
            }

            _menus.push_back(menu);
        }

		for (auto& menuJson : menusList)
		{
			Menu& menu{ *FindMenu(_menus, menuJson["title"]) };
			
			for (auto& subMenu : menuJson["menus"])
			{
				std::string subMenuString = subMenu;
				Menu* menuPtr = FindMenu(_menus, subMenuString);
				assert(menuPtr);
				menu.menus.push_back(menuPtr);
			}
		}

        file.close();
		
		return _menus.size();
	}
}