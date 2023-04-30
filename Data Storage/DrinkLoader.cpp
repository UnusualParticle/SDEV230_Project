#include "DrinkLoader.h"
#include "json.h"
#include <iostream>
#include <fstream>
using json = nlohmann::json;

using namespace std;

struct Cup
{
    string size;
    double price;
};
struct Drink
{
    string name;
    string image;
};

void DrinkLoader::LoadDrinks()
{
    cout << "Loading Drinks...\n";

    ifstream file("Data/Orders/DrinksTest.json");

    json data = json::parse(file);

    json cups = data["cups"];
    json drinksArray = data["sodas"];

    vector<Cup> cupsVector;
    vector<Drink> drinksVector;

    for (const auto& cup : cups)
    {
        string size = cup["size"];
        double price = cup["price"];

        cupsVector.insert(cupsVector.end(), {size, price});
    }

    for (const auto& drink : drinksArray)
    {
        string name = drink["name"];
        string image = drink["image"];

        drinksVector.insert(drinksVector.end(), {name, image});
    }

    cout << "Cups: " << endl;
    for (const auto& cup : cupsVector)
    {
        cout << "Cup: " << cup.size << " " << cup.price << endl;
    }


    cout << "Drinks: " << endl;
    for (const auto& drink : drinksVector)
    {
        cout << "Drink: " << drink.name << " " << drink.image << endl;
    }
}