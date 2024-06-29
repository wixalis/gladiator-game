#include "stdafx.h"
#include "Player.h"

extern string buf;

extern Localization l;

Player::Player() :
	Fighter(),
	Leveling(),
	name(l.getMessage(Localized::GLADIATOR)),
	portraitIndex(1),
	isExhausted(false),
	isFree(false)
{ }

Player::Player(
	const Fighter& F,
	const Leveling& L,
	const string& rName
) :
	Fighter(F),
	Leveling(L),
	name(rName),
	portraitIndex(1),
	isExhausted(false),
	isFree(false)
{ }

Player::Player(const Player& P) : Fighter(P), Leveling(P), name(P.name), portraitIndex(P.portraitIndex), isExhausted(P.isExhausted), isFree(P.isFree) { }

Player& Player::operator=(const Player& P)
{
	if (&P == this) return *this;

	Fighter::operator=(P);
	Leveling::operator=(P);

	name = P.name;
	portraitIndex = P.portraitIndex;
	isExhausted = P.isExhausted;
	isFree = P.isFree;

	return *this;
}

Player::~Player() { }

// TODO: think about regex
void Player::setName(const string& rName) { name = rName; }

void Player::setPortaitIndex(int i) { portraitIndex = i; }

void Player::setExhaustion(bool exhaustion)
{
	isExhausted = exhaustion;
}

void Player::setFreedom(bool freedom)
{
	isFree = freedom;
}

const string& Player::getName() const { return name; }

const int Player::getPortaitIndex() const { return portraitIndex; }

const bool Player::getExhaustion() const
{
	return isExhausted;
}

const bool Player::getFreedom() const
{
	return isFree;
}

void Player::saveToFile(const string& directory) const
{
	const string PATH = directory,
		FILE_PLAYER = "Player",
		FOLDER_INVENTORY = "Inventory/",
		FILE_INVENTORY = "_inventory",
		FILE_RIGHT_HAND = "_righthand",
		FILE_LEFT_HAND = "_lefthand",
		FILE_ARMOUR = "_armour",
		FILE_NAME = "_name",
		FORMAT = ".sav";

	// Opening file for saving
	ofstream fout(PATH + FILE_PLAYER + FORMAT, ios::binary);

	if (!fout)
		throw new exception("Error: Couldn't open file for player's saving");

	// Leveling
	fout << level << " " << experience << " " << unnassignedAttributes << " ";

	// Statistics
	fout << strength << " " << constitution << " " << dexterity << " "
		<< intelligence << " " << wisdom << " " << charisma << " "
		<< age << " " << fame << " ";

	// Fighter
	fout << hp << " " << fullHP << " ";

	// Player
	fout << portraitIndex << " " << isExhausted << " " << isFree << " ";

	// Inventory and equipment stored as pointers so they are saved in separate files
	// Main file contains information whether there is equipment

	// Inventory
	if (inventory)
	{
		fout << "1 ";
		// Creating the folder for inventory if it doesn't exist
		BOOL success = CreateDirectory((PATH + FOLDER_INVENTORY).c_str(), NULL);
		if (!success && GetLastError() != ERROR_ALREADY_EXISTS)
			throw new exception("Error: Couldn't create directory for save");

		inventory->saveToFile(PATH + FOLDER_INVENTORY);
	}
	else
		fout << "0 ";

	// Right hand weapon
	if (rightHand)
	{
		fout << "1 ";
		rightHand->saveToFile(PATH + FILE_PLAYER + FILE_RIGHT_HAND);
	}
	else
		fout << "0 ";

	// Left hand weapon
	if (leftHand)
	{
		fout << "1 ";
		leftHand->saveToFile(PATH + FILE_PLAYER + FILE_LEFT_HAND);
	}
	else
		fout << "0 ";

	// Armour
	if (armour)
	{
		fout << "1 ";
		armour->saveToFile(PATH + FILE_PLAYER + FILE_ARMOUR);
	}
	else
		fout << "0 ";

	fout.close();

	// Name
	ofstream foutName(PATH + FILE_PLAYER + FILE_NAME + FORMAT, ios::binary);

	if (!foutName)
		throw new exception("Error: Couldn't open file for player's name saving");

	foutName << name;

	foutName.close();
}

void Player::loadFromFile(const string& directory)
{
	const string PATH = directory,
		FILE_PLAYER = "Player",
		FOLDER_INVENTORY = "Inventory/",
		FILE_INVENTORY = "_inventory",
		FILE_RIGHT_HAND = "_righthand",
		FILE_LEFT_HAND = "_lefthand",
		FILE_ARMOUR = "_armour",
		FILE_NAME = "_name",
		FORMAT = ".sav";

	// Opening file for loading
	ifstream fin(PATH + FILE_PLAYER + FORMAT, ios::binary);

	if (!fin)
		throw new exception("Error: Couldn't open file for player's loading");

	// Leveling
	fin >> level >> experience >> unnassignedAttributes;

	// Statistics
	fin >> strength >> constitution >> dexterity
		>> intelligence >> wisdom >> charisma
		>> age >> fame;

	// Fighter
	fin >> hp >> fullHP;

	//Player
	fin >> portraitIndex >> isExhausted >> isFree;

	int hasInventory, hasRightHand, hasLeftHand, hasArmour;
	// Inventory
	fin >> hasInventory;
	if (hasInventory)
		inventory->loadFromFile(PATH + FOLDER_INVENTORY);
	else
		inventory.reset();

	// Right hand weapon
	fin >> hasRightHand;
	if (hasRightHand)
		rightHand->loadFromFile(PATH + FILE_PLAYER + FILE_RIGHT_HAND);
	else
		rightHand.reset();

	// Left hand weapon
	fin >> hasLeftHand;
	if (hasLeftHand)
		leftHand->loadFromFile(PATH + FILE_PLAYER + FILE_LEFT_HAND);
	else
		leftHand.reset();

	// Armour
	fin >> hasArmour;
	if (hasArmour)
		armour->loadFromFile(PATH + FILE_PLAYER + FILE_ARMOUR);
	else
		armour.reset();

	fin.close();

	// Name
	ifstream finName(PATH + FILE_PLAYER + FILE_NAME + FORMAT, ios::binary);

	if (!fin)
		throw new exception("Error: Couldn't open file for player's name loading");

	getline(finName, name);

	finName.close();
}
