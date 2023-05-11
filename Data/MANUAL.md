# Manual

## Data

The data for the program is made of of products and menus, listed in the appropriately named `.json` files.

### Products.json

The following shows the format for listing products

``` json
{
	"list":[
		{
			"name":"product 1",
			"price":1.99
		},
		{
			"name":"product 2",
			"price":2.99
		}
	]
}
```

+ **name** is the displayed name of the product
+ **proce** is the displayed price of the product

**NOTE:** All products mentioned in menus must be listed here.

### Menus.json

The following shows the format for listing products

``` json
{
	"list":[
		{
			"title": "menu 1",
			"products": [
				"product 1",
				"product 2"
			],
			"menus": [
				"menu 1"
			]
		}
	]
}
```

+ **title** is the displayed title of the menu
+ **products** is the list of products showin in the menu
+ **menus** is the list of menus linked to the menu

**NOTE:** All products listed in the menus must be defined in products.json

**NOTE:** All menus linked to in the menus must be defined in menus.json

**NOTE:** The maximum number of items in a menu (products and menu links combined) cannot be greater than 9. Any items more than that will not be displayed.

#### Options

There are two menus that have built in functionality, based on their titles, called "Cart" and "Checkout".

+ "Cart" will display the list of items in the user's cart and the total cost
+ "Checkout" will display only the total cost

Other products/menus may be added to these menus as desired, but this functionality is always enabled.

## User Experience

The user will be able to traverse the menus set up within the .json files and add or remove products from their cart using the green "Add" or red "remove" buttons. when they are done, they will be able to 