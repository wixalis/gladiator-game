#include "stdafx.h"
#include "Item.h"

int Item::currentID = 1; // Initializing static variable
// All gold should have id of 0 and value of 1

Item::Item() : id(currentID++), itemType(ITEM), value(BASIC_ITEM_VALUE), price(value) { }

Item::Item(ItemType iitemType, int vvalue) : itemType(iitemType)
{
	if (itemType == GOLD)
	{
		id = 0;
		value = 1;
		price = 1;
	}
	else
	{
		id = currentID++;
		value = vvalue;
		price = value;
	}
}

Item::Item(const Item& I) : itemType(I.itemType)
{
	if (itemType == GOLD)
	{
		id = 0;
		value = 1;
		price = 1;
	}
	else
	{
		id = I.id;
		value = I.value;
		price = I.price;
	}
}

Item& Item::operator=(const Item& I)
{
	if (&I == this) return *this;

	itemType = I.itemType;
	if (I.itemType == GOLD)
	{
		id = 0;
		value = 1;
		price = 1;
	}
	else
	{
		id = I.id;
		value = I.value;
		price = I.price;
	}

	return *this;
}

Item::~Item() { }

unique_ptr<Item> Item::clone() const
{
	return make_unique<Item>(*this);
}

bool Item::operator<(const Item& other) const
{
	if (itemType != other.itemType)
		return itemType < other.itemType;
	return value < other.value;
}

void Item::saveToFile(const string& path)
{
	const string FORMAT = ".sav";
	// Opening file for save
	ofstream fout(path + FORMAT, ios::binary);

	if (!fout)
		throw new exception("Error: Couldn't open file for item's saving");

	// Write item data
	fout << id << " " << itemType << " " << value << " " << price;

	fout.close();
}

void Item::loadFromFile(const string& path)
{
	const string FORMAT = ".sav";
	// Opening file for load
	ifstream fin(path + FORMAT, ios::binary);

	if (!fin)
		throw new exception("Error: Couldn't open file for item's loading");

	// Read item data
	int loadedItemType;
	fin >> id >> loadedItemType >> value >> price;

	itemType = static_cast<Item::ItemType>(loadedItemType);

	fin.close();
}

int Item::getID() const
{
	return id;
}

Item::ItemType Item::getItemType() const
{
	return itemType;
}

int Item::getValue() const
{
	return value;
}

int Item::getPrice() const
{
	return price;
}

void Item::setValue(int n)
{
	value = n;
}

void Item::setPrice(int n)
{
	price = n;
}

void Item::calculatePrice(int charisma, bool isOwnedByPlayer)
{
	// Calculate multiplier based on player's charisma and owner of the item
	int adjustmentFactor;

	if (isOwnedByPlayer)
	{
		if (charisma <= 50)
			adjustmentFactor = 100;
		else
			adjustmentFactor = 100 + (MAX_PRICE_MULTIPLIER * (charisma - 50) / 50);
	}
	else
		adjustmentFactor = 170 + (MAX_PRICE_MULTIPLIER * (50 - charisma) / 50);

	// Decrease all prices
	adjustmentFactor -= 30;

	price = (value * adjustmentFactor + 50) / 100;
}
