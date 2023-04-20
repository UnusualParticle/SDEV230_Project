#include <iostream>

#include "UI/SFML_test.h"
#include "UI/UserInterface.h"
#include "UI/UI_Menu.h"
#include "UI/Styles.h"

// Styles.cpp is not implimented yet
void LoadStyles()
{
	using namespace style;
	LoadStyle("button", "Data/style_button.json");
	LoadStyle("background", "Data/style_background.json");
}

int main()
{
	// put code here
	std::cout << "\n== Hello World! ==\n";
	SFML_TEST();
	ui::TEST_UI::RunTest();
	ui::TEST_MENU::RunTest();
	return 0;
}