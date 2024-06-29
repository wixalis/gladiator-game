#pragma once
#include "Item.h"

class Armour :
	public Item
{
public:
	enum ArmourType { LIGHT, HEAVY, NUMBER };
	enum Stat
	{
		DEFENSE,
		STR_ADDITION_PERC,
		DEX_ADDITION_PERC,
		EVASION_PROB_ADDITION,
		STUN_PROB_SUBSTRACTION
	};

	Armour();
	Armour(
		const Item&,
		int tier_,
		int defense_,
		ArmourType,
		int defenseAddition_,
		int strAdditionPerc_,
		int dexAdditionPerc_,
		int evasionProbAddition_,
		int stunProbSubtraction_
	);
	Armour(const Armour&);
	Armour& operator=(const Armour&);
	virtual ~Armour();

	unique_ptr<Item> clone() const;

	void update(int, int);
	int convertDefenseToPercentage(int) const;
	int getTotalDefense() const;

	int getTier() const;
	int getDefense() const;
	ArmourType getArmourType() const;
	int getDefAddition() const;
	int getStrengthAdditionPercentage() const;
	int getDexterityAdditionPercentage() const;
	int getEvasionProbAddition() const;
	int getStunProbSubtraction() const;

	// Save and load
	void saveToFile(const string&);
	void loadFromFile(const string&);

private:
	int tier;
	int defense;

	ArmourType type;
	int defAddition;
	int strAdditionPerc, dexAdditionPerc;
	int evasionProbAddition, stunProbSubtraction;
};
