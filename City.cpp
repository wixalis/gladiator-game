#include "stdafx.h"
#include "City.h"

City::City() : nameIndex(Cities::ROME), arena(), level(0), trader(), isPromotionAchieved(false), isQuestCompleted(false) { }

City::City(int nnameIndex, const Arena& A, int llevel, const HarmlessNPC& ttrader) :
	nameIndex(nnameIndex),
	arena(A),
	level(llevel),
	trader(ttrader),
	isPromotionAchieved(false),
	isQuestCompleted(false)
{ }

City::City(const City& C) :
	nameIndex(C.nameIndex),
	arena(C.arena),
	level(C.level),
	trader(C.trader),
	isPromotionAchieved(C.isPromotionAchieved),
	isQuestCompleted(C.isQuestCompleted)
{ }

City& City::operator=(const City& C)
{
	if (&C == this) return *this;

	nameIndex = C.nameIndex;
	arena = C.arena;
	level = C.level;
	trader = C.trader;
	isPromotionAchieved = C.isPromotionAchieved;
	isQuestCompleted = C.isQuestCompleted;

	return *this;
}

City::~City() { }

void City::setName(int s) { nameIndex = s; }

void City::setLevel(int l) { level = l; }

void City::setPromotion(bool b) { isPromotionAchieved = b; }

void City::setQuestCompletion(bool completion) { isQuestCompleted = completion; }

int City::getNameIndex() const { return nameIndex; }

Arena& City::getArena() { return arena; }

int City::getLevel() const { return level; }

HarmlessNPC & City::getTrader() { return trader; }

bool City::getPromotionStatus() const { return isPromotionAchieved; }

bool City::getQuestCompletion() const { return isQuestCompleted; }

void City::saveToFile(const string& directory) const
{
	const string PATH = directory,
		FILE_CITY = "City",
		FOLDER_ARENA = "Arena/",
		FOLDER_TRADER = "Trader/",
		FORMAT = ".sav";

	// Opening file for saving
	ofstream fout(PATH + FILE_CITY + FORMAT, ios::binary);

	if (!fout)
		throw new exception("Error: Couldn't open file for city's saving");

	fout << nameIndex << " " << level << " " << isPromotionAchieved << " " << isQuestCompleted;

	fout.close();

	// Saving arena
	// Creating the folder for arena if it doesn't exist
	BOOL success = CreateDirectory((PATH + FOLDER_ARENA).c_str(), NULL);
	if (!success && GetLastError() != ERROR_ALREADY_EXISTS)
		throw new exception("Error: Couldn't create directory for save");
	arena.saveToFile(PATH + FOLDER_ARENA);

	// Saving trader
	// Creating the folder for trader if it doesn't exist
	success = CreateDirectory((PATH + FOLDER_TRADER).c_str(), NULL);
	if (!success && GetLastError() != ERROR_ALREADY_EXISTS)
		throw new exception("Error: Couldn't create directory for save");
	trader.saveToFile(PATH + FOLDER_TRADER);
}

void City::loadFromFile(const string& directory)
{
	const string PATH = directory,
		FILE_CITY = "City",
		FOLDER_ARENA = "Arena/",
		FOLDER_TRADER = "Trader/",
		FORMAT = ".sav";

	// Opening file for loading
	ifstream fin(PATH + FILE_CITY + FORMAT, ios::binary);

	if (!fin)
		throw new exception("Error: Couldn't open file for city's saving");

	fin >> nameIndex >> level >> isPromotionAchieved >> isQuestCompleted;

	fin.close();

	// Saving arena
	arena.loadFromFile(PATH + FOLDER_ARENA);

	// Saving trader
	trader.loadFromFile(PATH + FOLDER_TRADER);
}
