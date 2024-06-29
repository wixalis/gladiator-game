#include "stdafx.h"
#include "Weapon.h"

Weapon::Weapon() :
	Item(ItemType::WEAPON),
	tier(MIN_WEAPON_TIER),
	damage(MIN_WEAPON_DAMAGE),
	type(BASIC_WEAPON_TYPE),
	damageAddition(0),
	strAdditionPerc(0),
	dexAdditionPerc(0),
	shieldProbAddition(0),
	shieldDefPercentAddition(0),
	name("")
{ }

Weapon::Weapon(
	const Item& rItem,
	int ttier,
	int ddamage,
	WeaponType ttype,
	int ddamageAddition,
	int sstrAdditionPerc,
	int ddexAdditionPerc,
	int sshieldProbAddition,
	int sshieldDefPercentAddition,
	const string& nname
) :
	Item(rItem),
	tier(ttier),
	damage(ddamage),
	type(ttype),
	damageAddition(ddamageAddition),
	strAdditionPerc(sstrAdditionPerc),
	dexAdditionPerc(ddexAdditionPerc),
	shieldProbAddition(sshieldProbAddition),
	shieldDefPercentAddition(sshieldDefPercentAddition),
	name(nname)
{ }

Weapon::Weapon(const Weapon& W) :
	Item(W),
	tier(W.tier),
	damage(W.damage),
	type(W.type),
	damageAddition(W.damageAddition),
	strAdditionPerc(W.strAdditionPerc),
	dexAdditionPerc(W.dexAdditionPerc),
	shieldProbAddition(W.shieldProbAddition),
	shieldDefPercentAddition(W.shieldDefPercentAddition),
	name(W.name)
{ }

Weapon& Weapon::operator=(const Weapon& W)
{
	if (&W == this) return *this;

	Item::operator=(W);
	tier = W.tier;
	damage = W.damage;
	type = W.type;
	damageAddition = W.damageAddition;
	strAdditionPerc = W.strAdditionPerc;
	dexAdditionPerc = W.dexAdditionPerc;
	shieldProbAddition = W.shieldProbAddition;
	shieldDefPercentAddition = W.shieldDefPercentAddition;
	name = W.name;
	return *this;
}

Weapon::~Weapon() { }

unique_ptr<Item> Weapon::clone() const
{
	return make_unique<Weapon>(*this);
}

void Weapon::update(int sstrength, int ddexterity)
{
	if (type != SHIELD)
		damageAddition = sstrength * strAdditionPerc / 100 + ddexterity * dexAdditionPerc / 100;
}

void Weapon::setName(const string& newName) { name = newName; }

int Weapon::getTotalDamage() const { return damage + damageAddition; }

bool Weapon::isCompatibleWith(WeaponType ttype) const
{
	switch (type)
	{
	case Weapon::SWORD:
		if (ttype == Weapon::SPEAR || ttype == Weapon::AXE)
			return false;
		break;
	case Weapon::SPEAR:
		return false;
	case Weapon::DAGGER:
		if (ttype == Weapon::SPEAR || ttype == Weapon::AXE)
			return false;
		break;
	case Weapon::AXE:
		return false;
	case Weapon::MACE:
		if (ttype == Weapon::SPEAR || ttype == Weapon::AXE)
			return false;
		break;
	case Weapon::SHIELD:
		if (ttype == Weapon::SPEAR || ttype == Weapon::AXE || ttype == Weapon::SHIELD)
			return false;
		break;
	default:
		//outputError("Unknown weapon type!\n");
		break;
	}
	return true;
}

int Weapon::getTier() const { return tier; }

int Weapon::getDamage() const { return damage; }

Weapon::WeaponType Weapon::getWeaponType() const { return type; }

int Weapon::getDamageAddition() const { return damageAddition; }

int Weapon::getStrengthAdditionPercentage() const
{
	return strAdditionPerc;
}

int Weapon::getDexterityAdditionPercentage() const
{
	return dexAdditionPerc;
}

int Weapon::getShieldProbAddition() const { return shieldProbAddition; }

int Weapon::getShieldDefPercentAddition() const { return shieldDefPercentAddition; }

const string& Weapon::getName() const { return name; }

void Weapon::saveToFile(const string& path)
{
	const string FILE_NAME = "_name", FORMAT = ".sav";
	// Opening file for save
	ofstream fout(path + FORMAT, ios::binary);

	if (!fout)
		throw new exception("Error: Couldn't open file for weapon's saving");

	// Write weapon data
	fout << id << " " << itemType << " " << value << " " << price << " "
		<< tier << " " << damage << " " << type << " "
		<< damageAddition << " " << strAdditionPerc << " " << dexAdditionPerc << " "
		<< shieldProbAddition << " " << shieldDefPercentAddition;

	fout.close();

	// Write weapon namee
	ofstream foutName(path + FILE_NAME + FORMAT, ios::binary);

	if (!foutName)
		throw new exception("Error: Couldn't open file for weapon's name saving");

	foutName << name;

	foutName.close();
}

void Weapon::loadFromFile(const string& path)
{
	const string FILE_NAME = "_name", FORMAT = ".sav";
	// Opening file for load
	ifstream fin(path + FORMAT, ios::binary);

	if (!fin)
		throw new exception("Error: Couldn't open file for weapon's loading");

	// Read weapon data
	int loadedItemType, loadedWeaponType;
	fin >> id >> loadedItemType >> value >> price
		>> tier >> damage >> loadedWeaponType
		>> damageAddition >> strAdditionPerc >> dexAdditionPerc
		>> shieldProbAddition >> shieldDefPercentAddition;

	itemType = static_cast<Item::ItemType>(loadedItemType);
	type = static_cast<Weapon::WeaponType>(loadedWeaponType);

	// Read weapon name
	ifstream finName(path + FILE_NAME + FORMAT, ios::binary);

	if (!finName)
		throw new exception("Error: Couldn't open file for weapon's name loading");

	finName >> name;

	finName.close();
}
