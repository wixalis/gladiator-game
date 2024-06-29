#include "stdafx.h"
#include "NPC.h"

NPC::NPC() : Fighter(), NamedNPC(), Leveling(), portraitIndex((rand() % 3) + 1) { } // TODO: change generation of portrait index to support more portraits

NPC::NPC(const Fighter& F, const NamedNPC& C, const Leveling& L, int pportraitIndex) : Fighter(F), NamedNPC(C), Leveling(L), portraitIndex(pportraitIndex) { }

NPC::NPC(const NPC& C) : Fighter(C), NamedNPC(C), Leveling(C), portraitIndex(C.portraitIndex) { }

NPC& NPC::operator=(const NPC& C)
{
	if (&C == this) return *this;

	Fighter::operator=(C);
	NamedNPC::operator=(C);
	Leveling::operator=(C);
	portraitIndex = C.portraitIndex;

	return *this;
}

NPC::~NPC() { }

unique_ptr<NPC> NPC::clone() const
{
	return make_unique<NPC>(*this);
}

void NPC::setPortraitIndex(int i) { portraitIndex = i; }

const int NPC::getPortraitIndex() const { return portraitIndex; }

void NPC::saveToFile(const string& directory) const
{
	const string PATH = directory,
		FILE_GLADIATOR = "Gladiator",
		FOLDER_INVENTORY = "Inventory/",
		FILE_INVENTORY = "_inventory",
		FILE_RIGHT_HAND = "_righthand",
		FILE_LEFT_HAND = "_lefthand",
		FILE_ARMOUR = "_armour",
		FILE_NAME = "_name",
		FORMAT = ".sav";

	// Opening file for saving
	ofstream fout(PATH + FILE_GLADIATOR + FORMAT, ios::binary);

	if (!fout)
		throw new exception("Error: Couldn't open file for gladiator's saving");

	// Leveling
	fout << level << " " << experience << " " << unnassignedAttributes << " ";

	// Statistics
	fout << strength << " " << constitution << " " << dexterity << " "
		<< intelligence << " " << wisdom << " " << charisma << " "
		<< age << " " << fame << " ";

	// Fighter
	fout << hp << " " << fullHP << " ";

	// NamedNPC
	fout << firstNameIndex << " " << lastNameIndex << " ";

	// Gladiator
	fout << portraitIndex << " ";

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
		rightHand->saveToFile(PATH + FILE_GLADIATOR + FILE_RIGHT_HAND);
	}
	else
		fout << "0 ";

	// Left hand weapon
	if (leftHand)
	{
		fout << "1 ";
		leftHand->saveToFile(PATH + FILE_GLADIATOR + FILE_LEFT_HAND);
	}
	else
		fout << "0 ";

	// Armour
	if (armour)
	{
		fout << "1 ";
		armour->saveToFile(PATH + FILE_GLADIATOR + FILE_ARMOUR);
	}
	else
		fout << "0 ";

	fout.close();
}

void NPC::loadFromFile(const string& directory)
{
	const string PATH = directory,
		FILE_GLADIATOR = "Gladiator",
		FOLDER_INVENTORY = "Inventory/",
		FILE_INVENTORY = "_inventory",
		FILE_RIGHT_HAND = "_righthand",
		FILE_LEFT_HAND = "_lefthand",
		FILE_ARMOUR = "_armour",
		FILE_NAME = "_name",
		FORMAT = ".sav";

	// Opening file for loading
	ifstream fin(PATH + FILE_GLADIATOR + FORMAT, ios::binary);

	if (!fin)
		throw new exception("Error: Couldn't open file for gladiator's loading");

	// Leveling
	fin >> level >> experience >> unnassignedAttributes;

	// Statistics
	fin >> strength >> constitution >> dexterity
		>> intelligence >> wisdom >> charisma
		>> age >> fame;

	// Fighter
	fin >> hp >> fullHP;

	// NamedNPC
	fin >> firstNameIndex >> lastNameIndex;

	// Gladiator
	fin >> portraitIndex;

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
		rightHand->loadFromFile(PATH + FILE_GLADIATOR + FILE_RIGHT_HAND);
	else
		rightHand.reset();

	// Left hand weapon
	fin >> hasLeftHand;
	if (hasLeftHand)
		leftHand->loadFromFile(PATH + FILE_GLADIATOR + FILE_LEFT_HAND);
	else
		leftHand.reset();

	// Armour
	fin >> hasArmour;
	if (hasArmour)
		armour->loadFromFile(PATH + FILE_GLADIATOR + FILE_ARMOUR);
	else
		armour.reset();

	fin.close();
}
