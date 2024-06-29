#pragma once

class Item
{
protected:
	static int currentID;
	int id;
public:
	enum ItemType
	{
		ARMOUR,
		WEAPON,
		GOLD,
		ITEM
	};

	Item();
	Item(ItemType, int value_ = 1);
	Item(const Item&);
	Item& operator=(const Item&);
	virtual ~Item();

	virtual unique_ptr<Item> clone() const;

	bool operator<(const Item&) const;

	// Save and load
	void saveToFile(const string&);
	void loadFromFile(const string&);

protected:
	ItemType itemType;
	int value;
	int price;

public:
	int getID() const;
	ItemType getItemType() const;
	int getValue() const;
	int getPrice() const;

	void setValue(int);
	void setPrice(int);

	void calculatePrice(int charisma_, bool isOwnedByPlayer_ = false);
};
