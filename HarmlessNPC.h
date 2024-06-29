#pragma once
#include "Statistics.h"
#include "NamedNPC.h"

class HarmlessNPC :
	public Statistics,
	public NamedNPC
{
private:
	int level;
	unique_ptr<Inventory> inventory;
public:
	HarmlessNPC();
	HarmlessNPC(int level_, const Statistics&, const NamedNPC&, unique_ptr<Inventory>);
	HarmlessNPC(const HarmlessNPC&);
	HarmlessNPC& operator=(const HarmlessNPC&);
	virtual ~HarmlessNPC();

	int getLevel() const;
	const unique_ptr<Inventory>& getInventory() const;

	void updateInventory();

	// Save and load
	void saveToFile(const string&) const;
	void loadFromFile(const string&);
};
