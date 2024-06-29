#include "stdafx.h"
#include "Arena.h"

Arena::Arena() : gladiators() { }

Arena::Arena(const vector<unique_ptr<NPC>>& G)
{
	for (const auto& gladiator : G)
		gladiators.push_back(gladiator->clone());
}

Arena::Arena(const Arena& A)
{
	for (const auto& gladiator : A.gladiators)
		gladiators.push_back(gladiator->clone());
}

Arena& Arena::operator=(const Arena& A)
{
	if (&A == this) return *this;

	gladiators.clear();
	for (const auto& gladiator : A.gladiators)
		gladiators.push_back(gladiator->clone());

	return *this;
}

Arena::~Arena() { }

vector<unique_ptr<NPC>>& Arena::getGladiators()
{
	return gladiators;
}

unique_ptr<NPC>& Arena::getGladiator(int n)
{
	return gladiators[n];
}

void Arena::changeGladiator(int index, int level)
{
	gladiators[index] = generateNPC(level);
}

void Arena::saveToFile(const string& directory) const
{
	const string PATH = directory,
		FILE_ARENA = "Arena",
		FOLDER_GLADIATOR = "Gladiator",
		FORMAT = ".sav";

	// Opening file for saving
	ofstream fout(PATH + FILE_ARENA + FORMAT, ios::binary);

	if (!fout)
		throw new exception("Error: Couldn't open file for arena's saving");

	for (int i = 0; i < OPPONENTS_NUMBER; i++)
	{
		// Creating the folder for gladiator if it doesn't exist
		BOOL success = CreateDirectory((PATH + FOLDER_GLADIATOR + to_string(i) + '/').c_str(), NULL);
		if (!success && GetLastError() != ERROR_ALREADY_EXISTS)
			throw new exception("Error: Couldn't create directory for save");

		if (gladiators[i])
		{
			fout << "1 ";
			gladiators[i]->saveToFile(PATH + FOLDER_GLADIATOR + to_string(i) + '/');
		}
		else
			fout << "0 ";
	}
	fout.close();
}

void Arena::loadFromFile(const string& directory)
{
	const string PATH = directory,
		FILE_ARENA = "Arena",
		FOLDER_GLADIATOR = "Gladiator",
		FORMAT = ".sav";

	// Opening file for loading
	ifstream fin(PATH + FILE_ARENA + FORMAT, ios::binary);

	if (!fin)
		throw new exception("Error: Couldn't open file for arena's loading");

	int hasGladiator;
	gladiators.resize(OPPONENTS_NUMBER);
	for (int i = 0; i < OPPONENTS_NUMBER; i++)
	{
		gladiators[i] = make_unique<NPC>();
		fin >> hasGladiator;
		if (hasGladiator)
			gladiators[i]->loadFromFile(PATH + FOLDER_GLADIATOR + to_string(i) + '/');
	}

	fin.close();
}
