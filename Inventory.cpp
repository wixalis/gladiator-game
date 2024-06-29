#include "stdafx.h"
#include "Inventory.h"

extern string buf;

Inventory::Inventory() : items() { }

Inventory::Inventory(const map<int, pair<unique_ptr<Item>, int>>& iitems)
{
	for (const auto& pair : iitems)
	{
		// Create new unique_ptr and move it into pair
		unique_ptr<Item> newItem = pair.second.first ? pair.second.first->clone() : nullptr;
		items.emplace(newItem->getID(), make_pair(move(newItem), pair.second.second));
	}
}

Inventory::Inventory(const Inventory& I)
{
	for (const auto& pair : I.items)
	{
		// Create new unique_ptr and move it into pair
		unique_ptr<Item> newItem = pair.second.first ? pair.second.first->clone() : nullptr;
		items.emplace(newItem->getID(), make_pair(move(newItem), pair.second.second));
	}
}

Inventory& Inventory::operator=(const Inventory& I)
{
	if (&I == this) return *this;

	items.clear();

	for (const auto& pair : I.items)
	{
		// Create new unique_ptr and move it into pair
		unique_ptr<Item> newItem = pair.second.first ? pair.second.first->clone() : nullptr;
		items.emplace(newItem->getID(), make_pair(move(newItem), pair.second.second));
	}

	return *this;
}

Inventory::~Inventory() { }

PairRef<unique_ptr<Item>&, int&> Inventory::operator[](int index)
{
	int size = items.size();
	if (index >= size)
		throw out_of_range("Index out of range");

	auto it = items.begin();
	advance(it, index);
	return { it->second.first, it->second.second };
}

PairRef<const unique_ptr<Item>&, const int&> Inventory::operator[](int index) const
{
	int size = items.size();
	if (index >= size)
		throw out_of_range("Index out of range");

	auto it = items.begin();
	advance(it, index);
	return { it->second.first, it->second.second };
}

int Inventory::size() const
{
	return items.size();
}

unique_ptr<Item> Inventory::extractItem(int id)
{
	auto it = items.find(id);
	if (it != items.end())
	{
		unique_ptr<Item> item = move(it->second.first);
		items.erase(it);
		return item;
	}
	return nullptr;
}

const Item* Inventory::getItem(int id) const
{
	auto it = items.find(id);
	if (it != items.end())
		return it->second.first.get(); // Return item
	return nullptr; // Item not found - return nullptr
}

int Inventory::getItemQuantity(int id) const
{
	auto it = items.find(id);
	if (it != items.end())
		return it->second.second; // Return quantity
	return 0; // Item not found - return 0
}

Item::ItemType Inventory::getItemType(int id) const
{
	auto it = items.find(id);
	if (it != items.end())
		return it->second.first->getItemType(); // Return item type
	throw out_of_range("Item is not in inventory");
}

void Inventory::addItem(unique_ptr<Item> item, int quantity)
{
	// Get item unique ID
	int id = item->getID();

	// Search for this id in inventory
	auto it = items.find(id);

	if (it != items.end()) // If item is already in inventory - increase its quantity
		it->second.second += quantity;
	else if (items.size() >= MAX_INVENTORY_SIZE) // Check space in inventory
		throw out_of_range("Cannot add item. Inventory is full.");
	else // If item is not in inventory - add it
		items.emplace(id, make_pair(move(item), quantity));
}

void Inventory::removeItem(int id, int quantity)
{
	auto it = items.find(id);

	// Check if item is in inventory and there's enough to remove
	if (it != items.end() && it->second.second >= quantity)
	{
		// Remove specified quantity
		it->second.second -= quantity;
		if (it->second.second == 0) // If there's 0 left - remove item
			items.erase(it);
	}
}

void Inventory::saveToFile(const string& path)
{
	const string FILE_INVENTORY = "Inventory",
		FILE_ITEM = "_item",
		FORMAT = ".sav";

	// Opening file for save
	ofstream foutInv(path + FILE_INVENTORY + FORMAT, ios::binary);

	if (!foutInv)
		throw new exception("Error: Couldn't open file for inventory's saving");

	// Write inventory data
	int invSize = items.size();
	foutInv << invSize << " ";

	for (int i = 0; i < invSize; i++)
	{
		// Use iterator to get item by index
		auto it = items.begin();
		advance(it, i);
		foutInv << it->second.first->getItemType() << " " << it->second.second << " ";

		// Compose path to item's data file
		buf = path + FILE_INVENTORY + FILE_ITEM + to_string(i);
		// Write item data
		switch (it->second.first->getItemType())
		{
		case Item::ItemType::WEAPON:
			if (auto weapon = dynamic_cast<Weapon*>(it->second.first.get()))
				weapon->saveToFile(buf);
			else
				it->second.first->saveToFile(buf);
			break;

		case Item::ItemType::ARMOUR:
			if (auto armour = dynamic_cast<Armour*>(it->second.first.get()))
				armour->saveToFile(buf);
			else
				it->second.first->saveToFile(buf);
			break;

		default:
			it->second.first->saveToFile(buf);
			break;
		}
	}
	foutInv.close();
}

void Inventory::loadFromFile(const string& path)
{
	unique_ptr<Weapon> pWeapon;
	unique_ptr<Armour> pArmour;
	unique_ptr<Item> pItem;
	const string FILE_INVENTORY = "Inventory",
		FILE_ITEM = "_item",
		FORMAT = ".sav";

	// Opening file for save
	ifstream finInv(path + FILE_INVENTORY + FORMAT, ios::binary);

	if (!finInv)
		throw new exception("Error: Couldn't open file for inventory's loading");

	// Load inventory size
	int loadedInventorySize, itemType, itemQuantity;
	finInv >> loadedInventorySize;

	items.clear();
	// Load inventory items
	for (int i = 0; i < loadedInventorySize; i++)
	{
		finInv >> itemType >> itemQuantity;

		// Compose path to item's data file
		buf = path + FILE_INVENTORY + FILE_ITEM + to_string(i);
		// Write item data
		switch (itemType)
		{
		case Item::ItemType::WEAPON:
			pWeapon = make_unique<Weapon>();
			pWeapon->loadFromFile(buf);
			this->addItem(move(pWeapon), itemQuantity);
			break;

		case Item::ItemType::ARMOUR:
			pArmour = make_unique<Armour>();
			pArmour->loadFromFile(buf);
			this->addItem(move(pArmour), itemQuantity);
			pArmour.reset();
			break;

		default:
			pItem = make_unique<Item>();
			pItem->loadFromFile(buf);
			this->addItem(move(pItem), itemQuantity);
			pItem.reset();
			break;
		}
	}
	finInv.close();
}
