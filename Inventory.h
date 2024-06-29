#pragma once
class Inventory
{
private:
	map<int, pair<unique_ptr<Item>, int>> items;

public:
	Inventory();
	Inventory(const map<int, pair<unique_ptr<Item>, int>>& items_);
	Inventory(const Inventory&);
	Inventory& operator=(const Inventory&);
	~Inventory();

	PairRef<unique_ptr<Item>&, int&> operator[](int id_);
	PairRef<const unique_ptr<Item>&, const int&> operator[](int id_) const;

	int size() const;
	unique_ptr<Item> extractItem(int id_);
	const Item* getItem(int id_) const;
	int getItemQuantity(int id_) const;
	Item::ItemType getItemType(int id_) const;

	void addItem(unique_ptr<Item>, int quantity_ = 1);
	void removeItem(int id_, int quantity_ = 1);

	// Save and load
	void saveToFile(const string&);
	void loadFromFile(const string&);
};
