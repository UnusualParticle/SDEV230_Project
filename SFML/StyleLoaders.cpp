#include "StyleLoaders.h"
#include "UI_Menu.h"
// #include "json"
#include <fstream>
#include <string>
using string = std::string;

// TEMPORARY
class _json
{
public:
	_json& operator[](string)
	{
		return *this;
	}
	bool has(string)
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

void LoadStateStyle(ui::StateStyle& style, _json& j_obj, ui::StateStyle& backup = ui::Settings::style.regular)
{
	if(j_obj.has("bg-color"))
		style.fillColor = sf::Color(j_obj["bg-color"].getint());
	else
		style.fillColor = backup.fillColor;

	if(j_obj.has("text-color"))
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

void LoadProductStyles()
{
	std::ifstream file{"data/style_product.json"};
	_json j_obj{};
	file >> j_obj;
	file.close();

	ui::ElementStyle background{};
	ui::ElementStyle add{};
	ui::ElementStyle remove{};

	LoadStyle(background, j_obj["background"]);
	LoadStyle(add, j_obj["add"]);
	LoadStyle(remove, j_obj["remove"]);

	//ui::Product::SetStyles(&background, &add, &remove);
}
void LoadMenuStyles()
{
	std::ifstream file{ "data/style_menu.json" };
	_json j_obj{};
	file >> j_obj;
	file.close();

	ui::ElementStyle background{};
	ui::ElementStyle title{};
	ui::ElementStyle back{};

	LoadStyle(background, j_obj["background"]);
	LoadStyle(title, j_obj["add"]);
	LoadStyle(back, j_obj["remove"]);

	//ui::Menu::SetStyles(&background, &title, &back);
}

void StyleLoader()
{
	LoadProductStyles();
}