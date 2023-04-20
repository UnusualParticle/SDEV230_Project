#pragma once

#include "UserInterface.h"
#include <map>
#include <string>

// Styles.cpp is not implimented yet
namespace style
{
	using string = std::string;
	using StyleMap_t = std::map<string, ui::ElementStyle>;
	inline StyleMap_t StyleMap{};

	std::pair<StyleMap_t::iterator, bool> LoadStyle(const string& key, const string& filename);
}