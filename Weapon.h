#pragma once
#include "Item.h"

class Weapon :
	public Item
{
public:
	enum WeaponType
	{
		SWORD,
		SPEAR,
		DAGGER,
		AXE,
		MACE,
		SHIELD,
		NUMBER
	};

	Weapon();
	Weapon(
		const Item& item_,
		int tier_,
		int damage_,
		WeaponType type_,
		int damageAddition_,
		int strAdditionPerc_,
		int dexAdditionPerc_,
		int shieldProbAddition_,
		int shieldDefPercentAddition_,
		const string& name_
	);
	Weapon(const Weapon&);
	Weapon& operator=(const Weapon&);
	virtual ~Weapon();

	unique_ptr<Item> clone() const;

	void update(int, int);
	void setName(const string&);

	int getTotalDamage() const;
	bool isCompatibleWith(WeaponType) const;

	int getTier() const;
	int getDamage() const;
	WeaponType getWeaponType() const;
	int getDamageAddition() const;
	int getStrengthAdditionPercentage() const;
	int getDexterityAdditionPercentage() const;
	int getShieldProbAddition() const;
	int getShieldDefPercentAddition() const;
	const string& getName() const;

	// Save and load
	void saveToFile(const string&);
	void loadFromFile(const string&);

private:
	int tier;
	int damage;

	WeaponType type;
	int damageAddition;
	int strAdditionPerc, dexAdditionPerc;

	int shieldProbAddition, shieldDefPercentAddition;

	string name;
};
