#include <iostream>

#include "SFML/SFML_test.h"
#include "SFML/UserInterface.h"
#include "SFML/UI_Menu.h"

int main()
{
	// put code here
	std::cout << "\n== Hello World! ==\n";
	SFML_TEST();
	ui::TEST_UI::RunTest();
	ui::TEST_MENU::RunTest();
	return 0;
}