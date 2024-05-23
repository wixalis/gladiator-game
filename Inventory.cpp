#include "stdafx.h"
#include "Inventory.h"

Inventory::Inventory() : items() { }

Inventory::Inventory(const unordered_map<int, pair<unique_ptr<Item>, int>> iitems) : items(iitems) { }

Inventory::Inventory(const Inventory& I) : items(I.items) { }

Inventory& Inventory::operator=(const Inventory& I)
{
	if (&I == this) return *this;

	items = I.items;

	return *this;
}

Inventory::~Inventory() { }

const unique_ptr<Item>& Inventory::getItem(int id) const
{
	auto it = items.find(id);
	if (it != items.end())
		return it->second.first; // Return item
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