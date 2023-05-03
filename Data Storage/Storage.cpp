#include "Storage.h"

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
	const std::vector<CartItem>& Cart::getItems() { return m_items; };
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
}