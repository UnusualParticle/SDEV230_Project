#include "Styles.h"
#include "UI_Menu.h"
// #include "json"
#include <fstream>
#include <map>
#include <string>

// TEMPORARY
class _json
{
public:
	_json& operator[](std::string)
	{
		return *this;
	}
	bool has(std::string)
	{
		return true;
	}
	int getint()
	{
		return {};
	}
	float getfloat()
	{
		return {};
	}
};
std::istream& operator>>(std::istream& stream, _json& j)
{
	return stream;
}

namespace style
{
	void LoadStateStyle(ui::StateStyle& style, _json& j_obj, ui::StateStyle& backup = ui::Settings::style.regular)
	{
		if (j_obj.has("bg-color"))
			style.fillColor = sf::Color(j_obj["bg-color"].getint());
		else
			style.fillColor = backup.fillColor;

		if (j_obj.has("text-color"))
			style.textColor = sf::Color(j_obj["text-color"].getint());
		else
			style.textColor = backup.textColor;

		if (j_obj.has("text-color"))
			style.outlineColor = sf::Color(j_obj["border-color"].getint());
		else
			style.outlineColor = backup.outlineColor;

		if (j_obj.has("text-color"))
			style.outlineThickness = j_obj["border-size"].getfloat();
		else
			style.outlineThickness = backup.outlineThickness;
	}
	void LoadStyle(ui::ElementStyle& style, _json& j_obj)
	{
		LoadStateStyle(style.regular, j_obj["default"]);
		LoadStateStyle(style.hover, j_obj["hover"], style.regular);
		LoadStateStyle(style.click, j_obj["click"], style.regular);
		LoadStateStyle(style.select, j_obj["click"], style.regular);
	}

	std::pair<StyleMap_t::iterator, bool> LoadStyle(const string& key, string& filename)
	{
		std::ifstream file{ filename };
		if (!file.is_open())
			return {};

		_json j_obj{};
		file >> j_obj;
		file.close();

		ui::ElementStyle style;
		LoadStyle(style, j_obj);
		return StyleMap.insert({ key, style });
	}
}