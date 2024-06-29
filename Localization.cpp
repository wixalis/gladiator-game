#include "stdafx.h"
#include "Localization.h"

extern string logStr;

Localization::Localization() :
	language(Language::ENGLISH),
	messages(),
	npcFirstNames(),
	npcLastNames(),
	gladiatorNicknames(),
	itemTypes(),
	weaponTypes(),
	armourTypes(),
	cityNames()
{ }

Localization::Localization(const Localization& L) :
	language(L.language),
	messages(L.messages),
	npcFirstNames(L.npcFirstNames),
	npcLastNames(L.npcLastNames),
	gladiatorNicknames(L.gladiatorNicknames),
	itemTypes(L.itemTypes),
	weaponTypes(L.weaponTypes),
	armourTypes(L.armourTypes),
	cityNames(L.cityNames)
{ }

Localization& Localization::operator=(const Localization& L)
{
	if (&L == this) return *this;
	language = L.language;
	messages = L.messages;
	npcFirstNames = L.npcFirstNames;
	npcLastNames = L.npcLastNames;
	gladiatorNicknames = L.gladiatorNicknames;
	itemTypes = L.itemTypes;
	weaponTypes = L.weaponTypes;
	armourTypes = L.armourTypes;
	cityNames = L.cityNames;
	return *this;
}

Localization::~Localization() { }

const string& Localization::operator[](int i) const { return messages[i]; }

void Localization::setLanguage(Language llanguage)
{
	// Clear previous data
	language = llanguage;
	npcFirstNames.clear();
	npcLastNames.clear();
	gladiatorNicknames.clear();
	itemTypes.clear();
	weaponTypes.clear();
	armourTypes.clear();
	cityNames.clear();
	// Determinating the language file
	string fileName = "", langPrefix = "";
	switch (llanguage)
	{
	default: case Language::ENGLISH:
		fileName = "English";
		langPrefix = "En_";
		break;
	case Language::UKRAINIAN:
		fileName = "Ukrainian";
		langPrefix = "Uk_";
		break;
	case Language::RUSSIAN:
		fileName = "Russian";
		langPrefix = "Ru_";
		break;
	case Language::LATIN:
		fileName = "Latin";
		langPrefix = "En_";
		break;
	}
	// Opening files
	string path = "Data/Language/";
	const string SETTINGS_FILE = "Settings", SETTINGS_FORMAT = ".conf", FORMAT = ".lang";

	// Saving selected language (and other settings)
	ofstream fout("Data/" + SETTINGS_FILE + SETTINGS_FORMAT, ios::binary);
	if (!fout)
		throw new exception("Error: Couldn't open file for game settings");
	fout << llanguage << " " << game.getSoundStatus() << " " << game.getAutoSaveStatus();
	fout.close();

	// Reading the file and filling the localization array
	ifstream fin(path + fileName + FORMAT);
	ifstream finEng(path + "English" + FORMAT);

	string line, lineEng;
	int lineIndex, i = 0;
	for (; i < Localized::MESSAGE_NUMBER; i++)
	{
		getline(fin, line);
		if (!line.empty())
			messages[i] = line;
		else
		{
			lineIndex = 0;
			while (getline(finEng, line))
			{
				if (lineIndex == i)
				{
					messages[i] = line;
					finEng.clear();
					finEng.seekg(0);
					break;
				}
				lineIndex++;
			}
		}
	}

	fin.close();
	finEng.close();

	// Reading the file of NPCs` first names
	path += "Names/";
	fileName = "FirstNames";
	fin.open(path + langPrefix + fileName + FORMAT);
	finEng.open(path + "En_" + fileName + FORMAT);

	while (getline(fin, line))
	{
		getline(finEng, lineEng);
		if (line != "")
			npcFirstNames.push_back(line);
		else
			npcFirstNames.push_back(lineEng);
	}

	fin.close();
	finEng.close();

	// Reading the file of NPCs` last names
	fileName = "LastNames";
	fin.open(path + langPrefix + fileName + FORMAT);
	finEng.open(path + "En_" + fileName + FORMAT);

	while (getline(fin, line))
	{
		getline(finEng, lineEng);
		if (line != "")
			npcLastNames.push_back(line);
		else
			npcLastNames.push_back(lineEng);
	}

	fin.close();
	finEng.close();

	// Reading the file of item types
	path = "Data/Language/World/";
	fileName = "ItemTypes";
	fin.open(path + langPrefix + fileName + FORMAT);
	finEng.open(path + "En_" + fileName + FORMAT);

	while (getline(fin, line))
	{
		getline(finEng, lineEng);
		if (line != "")
			itemTypes.push_back(line);
		else
			itemTypes.push_back(lineEng);
	}

	fin.close();
	finEng.close();

	// Reading the file of weapon types
	fileName = "WeaponTypes";
	fin.open(path + langPrefix + fileName + FORMAT);
	finEng.open(path + "En_" + fileName + FORMAT);

	while (getline(fin, line))
	{
		getline(finEng, lineEng);
		if (line != "")
			weaponTypes.push_back(line);
		else
			weaponTypes.push_back(lineEng);
	}

	fin.close();
	finEng.close();

	// Reading the file of armour types
	fileName = "ArmourTypes";
	fin.open(path + langPrefix + fileName + FORMAT);
	finEng.open(path + "En_" + fileName + FORMAT);

	while (getline(fin, line))
	{
		getline(finEng, lineEng);
		if (line != "")
			armourTypes.push_back(line);
		else
			armourTypes.push_back(lineEng);
	}

	fin.close();
	finEng.close();

	// Reading the file of City names
	fileName = "CityNames";
	fin.open(path + langPrefix + fileName + FORMAT);
	finEng.open(path + "En_" + fileName + FORMAT);

	while (getline(fin, line))
	{
		getline(finEng, lineEng);
		if (line != "")
			cityNames.push_back(line);
		else
			cityNames.push_back(lineEng);
	}

	fin.close();
	finEng.close();
	logStr = l.getMessage(Localized::MESSAGES_LOG) + ":\r\n\r\n";
}

Language Localization::getLanguage() const { return language; }

string Localization::getMessage(Localized::Message i) const
{
	return messages[i];
}

string Localization::getNPCName(const NamedNPC& NPC) const
{
	return npcFirstNames[NPC.getFirstNameIndex()] + ' ' + npcLastNames[NPC.getLastNameIndex()];
}

const string& Localization::getNPCFirstName(const NamedNPC& NPC) const
{
	return npcFirstNames[NPC.getFirstNameIndex()];
}

const string& Localization::getNPCLastName(const NamedNPC& NPC) const
{
	return npcLastNames[NPC.getLastNameIndex()];
}

const string& Localization::getNPCNickname(const NamedNPC& NPC) const
{
	return npcLastNames[NPC.getLastNameIndex()];
}

const string& Localization::getItemTypeName(const Item& I) const
{
	switch (I.getItemType())
	{
	case Item::ItemType::WEAPON:
		if (dynamic_cast<const Weapon*>(&I))
			return getWeaponTypeName(*dynamic_cast<const Weapon*>(&I));
		return itemTypes[I.getItemType()];

	case Item::ItemType::ARMOUR:
		if (dynamic_cast<const Armour*>(&I))
			return getArmourTypeName(*dynamic_cast<const Armour*>(&I));
		return itemTypes[I.getItemType()];

	default: return itemTypes[I.getItemType()];
	}
}

const string& Localization::getWeaponTypeName(const Weapon& W) const
{
	return weaponTypes[W.getWeaponType()];
}

const string& Localization::getArmourTypeName(const Armour& A) const
{
	return armourTypes[A.getArmourType()];
}

const string& Localization::getCityName(const City& C) const
{
	return cityNames[C.getNameIndex()];
}
