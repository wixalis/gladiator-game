// stdafx.cpp : source file that includes just the standard includes
// GladiatorGame.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information
/*
 * It is the game about gladiators in the Rome.
 * ...
 */

#include "stdafx.h"

// Buffers
TCHAR str[256] = "";
string buf = "";
string logStr = "";

extern HINSTANCE hInst;

Localization l;
Game game;

// __________ Other __________

void updateWindow(HWND hWnd)
{
	//if (!game.isBackgroundChanged())
	//	return;
	RECT windowRect;
	GetWindowRect(hWnd, &windowRect);
	SendMessage(hWnd, WM_SIZE, SIZE_RESTORED, MAKELPARAM(windowRect.right - windowRect.left, windowRect.bottom - windowRect.top));
	InvalidateRect(hWnd, 0, 1);
	UpdateWindow(hWnd);
	//RedrawWindow(hWnd, 0, 0, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
}

//void adjustStaticControlHeight(HWND hStatic, const string& text)
//{
//	HDC hdc = GetDC(hStatic);
//
//	// Get current size and position for window
//	RECT staticRect;
//	GetWindowRect(hStatic, &staticRect);
//
//	// Calculate new height for text
//	DrawText(hdc, text.c_str(), -1, &staticRect, DT_CALCRECT | DT_WORDBREAK);
//
//	ReleaseDC(hStatic, hdc);
//
//	// Apply padding
//	int padding = 10;
//	staticRect.left -= padding;
//	staticRect.top -= padding;
//	staticRect.right += padding;
//	staticRect.bottom += padding;
//
//	// Set new window height
//	SetWindowPos(hStatic, nullptr, 0, 0, staticRect.right - staticRect.left, staticRect.bottom - staticRect.top, SWP_NOMOVE | SWP_NOZORDER);
//}

// TODO: think about move to Converter
string toStringPrecision(double n, int precision)
{
	string s = to_string(n);
	if (precision > 0)
		return s.substr(0, s.find(".") + 1 + precision);
	else if (precision == 0)
		return s.substr(0, s.find("."));
	return "";
}

tm parseDateTime(const string & folderName)
{
	regex rgx(R"(\d{2}\d{2}\d{2}_\d{2}\d{2}\d{2})");
	smatch match;

	if (regex_search(folderName, match, rgx)) {
		string dateTimeStr = match.str();
		tm dateTime = {};

		// Parsing date and time from string
		dateTime.tm_year = stoi(dateTimeStr.substr(0, 2)) + 100; // years since 1900
		dateTime.tm_mon = std::stoi(dateTimeStr.substr(2, 2)) - 1; // months since January [0-11]
		dateTime.tm_mday = std::stoi(dateTimeStr.substr(4, 2));
		dateTime.tm_hour = std::stoi(dateTimeStr.substr(7, 2));
		dateTime.tm_min = std::stoi(dateTimeStr.substr(9, 2));
		dateTime.tm_sec = std::stoi(dateTimeStr.substr(11, 2));

		return dateTime;
	}
	else
		throw runtime_error("Invalid folder name format");
}

bool hasSubdirectory(const string& directoryPath)
{
	namespace fs = std::experimental::filesystem;

	// Exception if path is incorrect
	if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath))
		return false;

	// Chech every file in directory, if it's subdirectory - return true
	for (const auto& entry : fs::directory_iterator(directoryPath))
		if (fs::is_directory(entry))
			return true;

	return false;
}

void getFoldersInDirectory(const string& directoryPath, vector<string>& folderNames)
{
	string path;
	// Create namespace for easy access to functions
	namespace fs = std::experimental::filesystem;
	if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath))
		throw new exception("Error: Couldn't open folder for loading");

	// Struct holds name and time of creation of folder
	vector<FolderInfo> folderInfos;

	// Get every folder
	for (const auto& entry : fs::directory_iterator(directoryPath))
		if (fs::is_directory(entry))
		{
			string folderName = entry.path().filename().string();
			tm dateTime = parseDateTime(folderName);
			folderInfos.push_back({ folderName, dateTime });
		}

	// Sort in time of creation
	sort(folderInfos.begin(), folderInfos.end());

	// Add sorted names to vector
	for (const auto& folderInfo : folderInfos)
		folderNames.push_back(folderInfo.name);
}

string formatSaveName(const string& input)
{
	// Function tranforms input string of save folder name to display to user
	// For example string "w_1lvl_100624_002655" transfroms to "w | 1 level | 10.06.24 00:26:55"
	// 1. Separate string into parts
	int lastUnderscore = input.rfind('_');
	int secondLastUnderscore = input.rfind('_', lastUnderscore - 1);
	int thirdLastUnderscore = input.rfind('_', secondLastUnderscore - 1);
	string dateTime = input.substr(secondLastUnderscore + 1);
	string level = input.substr(thirdLastUnderscore + 1, secondLastUnderscore - thirdLastUnderscore - 1);
	string name = input.substr(0, thirdLastUnderscore);

	// 2. Format
	string dateTimeFormatted = dateTime.substr(0, 2) + '.' + dateTime.substr(2, 2) + "." + dateTime.substr(4, 2)
		+ ' ' + dateTime.substr(7, 2) + ':' + dateTime.substr(9, 2) + ':' + dateTime.substr(11, 2);

	string levelFormatted = level;
	int lvlPos = levelFormatted.find("lvl");
	if (lvlPos != string::npos)
		levelFormatted.replace(lvlPos, 3, " " + l.getMessage(Localized::LEVEL));

	// 3. Compose string
	return name + " | " + levelFormatted + " | " + dateTimeFormatted;
}

int normalize(int value, int min, int max)
{
	return ((value - min) * 100 / (max - min + 1));
}

int calculateWeightedAverageFactor(const vector<int>& values, const vector<pair<int, int>>& ranges, const vector<int>& weights)
{
	int weightedSum = 0;
	int totalWeight = 0;
	int unusedValues = 0;
	int size = values.size();
	for (int i = 0; i < size; i++)
	{
		int normalizedValue = normalize(values[i], ranges[i].first, ranges[i].second);
		weightedSum += normalizedValue * weights[i];
		totalWeight += weights[i];
	}

	return weightedSum / totalWeight;
}

int calculateFameForLevel(int level)
{
	return level * FAME_FOR_LEVEL_MULTIPLIER;
}

// __________ NPC __________

unique_ptr<NPC> generateNPC(int aproximateLevel)
{
	int i;
	// 1. Generating name
	const string PATH_BASE = "Data/Language/Names/", FORMAT = ".lang";
	ifstream fin;
	// Open file containing roman first names
	switch (l.getLanguage())
	{
	default: case Language::ENGLISH: fin.open(PATH_BASE + "En_FirstNames" + FORMAT); break;
	case Language::UKRAINIAN: fin.open(PATH_BASE + "Uk_FirstNames" + FORMAT); break;
	case Language::RUSSIAN: fin.open(PATH_BASE + "Ru_FirstNames" + FORMAT); break;
	}
	//if (!fin)
	//	output(localization[Localized::CREATION_NAME_LOAD_ERROR], 4);

	int count = 0;
	string line;
	while (getline(fin, line)) // Count lines in file
		count++;
	int randomFirstName = rand() % count; // Choose random line
	fin.close();

	// Open file containing roman last names
	switch (l.getLanguage())
	{
	default: case Language::ENGLISH: fin.open(PATH_BASE + "En_LastNames" + FORMAT); break;
	case Language::UKRAINIAN: fin.open(PATH_BASE + "Uk_LastNames" + FORMAT); break;
	case Language::RUSSIAN: fin.open(PATH_BASE + "Ru_LastNames" + FORMAT); break;
	}
	//if (!fin)
		//output(localization[Localized::CREATION_NAME_LOAD_ERROR], 4);

	count = 0;
	while (getline(fin, line)) // Count lines in file
		count++;
	int randomLastName = rand() % count; // Choose random line
	fin.close();

	// 2. Generating stats
	// Generating level in range (aproximate level - 5) to (aproximate level + 5)
	int level = rand() % 11 + aproximateLevel - 5;

	if (level < MIN_LEVEL)
		level = MIN_LEVEL;
	else if (level > MAX_LEVEL)
		level = MAX_LEVEL;

	// Calculating experience for current level
	Leveling leveling(level);
	leveling.setExperience(leveling.calculateExperienceForLevel(leveling.getLevel()));

	// Calculating unnassigned attributes for current level
	int unnassignedAttributes = level * ATTRIBUTES_PER_LEVEL;
	if (level > 9)
		unnassignedAttributes += 10;
	else
		unnassignedAttributes += level;

	// Randomly distributing attribute points
	vector<int> attributes(6, BASIC_ATTRIBUTES);
	int points;
	while (unnassignedAttributes > 0)
	{
		for (i = 0; i < 6; i++)
		{
			if (unnassignedAttributes > MAX_ATTRIBUTE - BASIC_ATTRIBUTES)
				points = rand() % (MAX_ATTRIBUTE - BASIC_ATTRIBUTES + 1);
			else
				points = rand() % (unnassignedAttributes + 1);
			points /= 2;
			points++;
			if (attributes[i] + points < MAX_ATTRIBUTE)
			{
				attributes[i] += points;
				unnassignedAttributes -= points;
			}
			if (unnassignedAttributes == 0)
				break;
		}
	}

	// 3. Generating weapons
	int itemTier = 0, n = aproximateLevel;
	// For every 16 levels of NPC increasing it's equipment tier by 1
	while (n > 0)
	{
		itemTier++;
		n -= 16;
	}

	// Generating type. Fill vector with every type
	vector<Weapon::WeaponType> weaponTypes;
	for (i = 0; i < Weapon::WeaponType::SHIELD; i++)
		weaponTypes.push_back(static_cast<Weapon::WeaponType>(i));

	// Choose possible types based on NPC's stats
	if (attributes[0] >= attributes[2])
	{
		weaponTypes.erase(weaponTypes.begin() + 1);
		weaponTypes.erase(weaponTypes.begin() + 1);
	}
	else
	{
		weaponTypes.erase(weaponTypes.begin() + 3);
		weaponTypes.erase(weaponTypes.begin() + 3);
	}

	unique_ptr<Weapon> rightHand = generateWeapon(itemTier, weaponTypes[rand() % weaponTypes.size()]);
	unique_ptr<Weapon> leftHand;
	if (rightHand->getWeaponType() != Weapon::WeaponType::AXE && rightHand->getWeaponType() != Weapon::WeaponType::SPEAR)
	{
		leftHand = generateWeapon(itemTier, weaponTypes[rand() % weaponTypes.size()]);
		if (!rightHand->isCompatibleWith(leftHand->getWeaponType()))
		{
			if (rand() % 100 < 75)
			{
				if (leftHand)
					leftHand.release();
				leftHand = generateWeapon(itemTier, Weapon::SHIELD);
			}
			else if (rand() % 100 < 75)
				do
				{
					if (leftHand)
						leftHand.release();
					leftHand = generateWeapon(itemTier, weaponTypes[rand() % weaponTypes.size()]);
				} while (!rightHand->isCompatibleWith(leftHand->getWeaponType()));
			else if (leftHand)
			{
				leftHand.release();
				leftHand = nullptr;
			}
		}
	}
	else
		leftHand = nullptr;

	// Updating weapon damage depending on stat scale
	rightHand->update(attributes[0], attributes[2]);
	if (leftHand)
		leftHand->update(attributes[0], attributes[2]);

	// Generate armour
	// Choose armour type based on NPC's stats
	Armour::ArmourType armourType = attributes[0] >= attributes[2] ? Armour::ArmourType::HEAVY : Armour::ArmourType::LIGHT;
	unique_ptr<Armour> armour = generateArmour(itemTier, armourType);

	// Updating armour defense depending on stat scale
	armour->update(attributes[0], attributes[2]);

	NPC npc(
		Fighter(
			Statistics(
				attributes[0], // Strength
				attributes[1], // Constitution
				attributes[2], // Dexterity
				attributes[3], // Intelligence
				attributes[4], // Wisdom
				attributes[5], // Charisma
				rand() % (MAX_AGE - MIN_AGE) + MIN_AGE, // Age
				calculateFameForLevel(leveling.getLevel()) + (rand() % FAME_DISPERSION * leveling.getLevel() / 100) // Fame
			),
			BASIC_HP, // HP
			BASIC_HP, // FullHP
			make_unique<Inventory>(), // Inventory
			move(rightHand), // Weapon (right hand)
			move(leftHand), // Weapon (left hand)
			move(armour) // Armour
		),
		NamedNPC(
			randomFirstName, // First name
			randomLastName // Last name
		),
		leveling, // Levelling (level, experience, unnassigned attributes)
		rand() % 3 + 1 // Portrait index
	);
	npc.updateMaxHP();

	return make_unique<NPC>(npc);
}

bool compareNPC(const unique_ptr<NPC>& a, const unique_ptr<NPC>& b)
{
	if (a->getLevel() == b->getLevel())
		return a->getFame() > b->getFame();
	return a->getLevel() > b->getLevel();
}

unique_ptr<HarmlessNPC> generateTrader(int level)
{
	// 1. Generating name
	const string PATH_BASE = "Data/Language/Names/", FORMAT = ".lang";
	ifstream fin;
	// Open file containing roman first names
	switch (l.getLanguage())
	{
	default: case Language::ENGLISH: fin.open(PATH_BASE + "En_FirstNames" + FORMAT); break;
	case Language::UKRAINIAN: fin.open(PATH_BASE + "Uk_FirstNames" + FORMAT); break;
	case Language::RUSSIAN: fin.open(PATH_BASE + "Ru_FirstNames" + FORMAT); break;
	}
	//if (!fin)
	//	output(localization[Localized::CREATION_NAME_LOAD_ERROR], 4);

	int count = 0;
	string line;
	while (getline(fin, line)) // Count lines in file
		count++;
	int randomFirstName = rand() % count; // Choose random line
	fin.close();

	// Open file containing roman last names
	switch (l.getLanguage())
	{
	default: case Language::ENGLISH: fin.open(PATH_BASE + "En_LastNames" + FORMAT); break;
	case Language::UKRAINIAN: fin.open(PATH_BASE + "Uk_LastNames" + FORMAT); break;
	case Language::RUSSIAN: fin.open(PATH_BASE + "Ru_LastNames" + FORMAT); break;
	}
	//if (!fin)
		//output(localization[Localized::CREATION_NAME_LOAD_ERROR], 4);

	count = 0;
	while (getline(fin, line)) // Count lines in file
		count++;
	int randomLastName = rand() % count; // Choose random line
	fin.close();

	// 2. Generating inventory
	unique_ptr<Inventory> inventory = make_unique<Inventory>();

	// Add gold
	inventory->addItem(make_unique<Item>(Item(Item::ItemType::GOLD)), 2000 * level);
	// Add items
	for (int i = 1; i < MAX_INVENTORY_SIZE; i++)
	{
		if (rand() % 100 < 75) // 75% chance to generate weapon
		{
			if (rand() % 100 < 75) // 75% chance to generate normal weapon
				inventory->addItem(generateWeapon(level));
			else // 25% chance to generate shield
				inventory->addItem(generateWeapon(level, Weapon::WeaponType::SHIELD));
		}
		else // 25% chance to generate armour
			inventory->addItem(generateArmour(level));
	}

	HarmlessNPC trader(
		level,
		Statistics(),
		NamedNPC(
			randomFirstName,
			randomLastName
		),
		move(inventory)
	);

	return make_unique<HarmlessNPC>(trader);
}

// __________ Weapon and Armour __________

int getWeaponScaleLimit(Weapon::WeaponType ttype, Attribute aattribute, Limit llimit)
{
	switch (ttype)
	{
	case Weapon::AXE:
		switch (aattribute)
		{
		case Attribute::STRENGTH:
			switch (llimit)
			{
			case Limit::MIN: return 32;
			case Limit::MAX: return 160;
			}
			break;
		case Attribute::DEXTERITY:
			switch (llimit)
			{
			case Limit::MIN: return 6;
			case Limit::MAX: return 30;
			}
			break;
		}
		break;

	case Weapon::DAGGER:
		switch (aattribute)
		{
		case Attribute::STRENGTH:
			switch (llimit)
			{
			case Limit::MIN: return 3;
			case Limit::MAX: return 15;
			}
			break;
		case Attribute::DEXTERITY:
			switch (llimit)
			{
			case Limit::MIN: return 17;
			case Limit::MAX: return 85;
			}
			break;
		}
		break;

	case Weapon::MACE:
		switch (aattribute)
		{
		case Attribute::STRENGTH:
			switch (llimit)
			{
			case Limit::MIN: return 15;
			case Limit::MAX: return 75;
			}
			break;
		case Attribute::DEXTERITY:
			switch (llimit)
			{
			case Limit::MIN: return 5;
			case Limit::MAX: return 25;
			}
			break;
		}
		break;

	case Weapon::SPEAR:
		switch (aattribute)
		{
		case Attribute::STRENGTH:
			switch (llimit)
			{
			case Limit::MIN: return 10;
			case Limit::MAX: return 50;
			}
			break;
		case Attribute::DEXTERITY:
			switch (llimit)
			{
			case Limit::MIN: return 30;
			case Limit::MAX: return 150;
			}
			break;
		}
		break;

	case Weapon::SWORD:
		switch (aattribute)
		{
		case Attribute::STRENGTH:
			switch (llimit)
			{
			case Limit::MIN: return 10;
			case Limit::MAX: return 50;
			}
			break;
		case Attribute::DEXTERITY:
			switch (llimit)
			{
			case Limit::MIN: return 10;
			case Limit::MAX: return 50;
			}
			break;
		}
		break;

	default: break;
	}
	//outputError("Unknown weapon type when getting its scale limit!\n");
	return -1;
}

int getArmourScaleLimit(Armour::ArmourType ttype, Armour::Stat sstat, Limit llimit)
{
	switch (ttype)
	{
	case Armour::LIGHT:
		switch (sstat)
		{
		case Armour::STR_ADDITION_PERC:
			switch (llimit)
			{
			case Limit::MIN: return 5;
			case Limit::MAX: return 25;
			}
			break;
		case Armour::DEX_ADDITION_PERC:
			switch (llimit)
			{
			case Limit::MIN: return 15;
			case Limit::MAX: return 75;
			}
			break;
		case Armour::EVASION_PROB_ADDITION:
			switch (llimit)
			{
			case Limit::MIN: return 1;
			case Limit::MAX: return 10;
			}
			break;
		case Armour::STUN_PROB_SUBSTRACTION:
			switch (llimit) { case Limit::MIN: case Limit::MAX: return 0; }
			break;
		}
		break;

	case Armour::HEAVY:
		switch (sstat)
		{
		case Armour::STR_ADDITION_PERC:
			switch (llimit)
			{
			case Limit::MIN: return 15;
			case Limit::MAX: return 75;
			}
			break;
		case Armour::DEX_ADDITION_PERC:
			switch (llimit)
			{
			case Limit::MIN: return 5;
			case Limit::MAX: return 25;
			}
			break;
		case Armour::EVASION_PROB_ADDITION:
			switch (llimit) { case Limit::MIN: case Limit::MAX: return 0; }
			break;
		case Armour::STUN_PROB_SUBSTRACTION:
			switch (llimit)
			{
			case Limit::MIN: return 1;
			case Limit::MAX: return 4;
			}
			break;
		}
		break;

	default: break;
	}

	//outputError("Unknown weapon type when getting its scale limit!\n");
	return -1;
}

unique_ptr<Weapon> generateWeapon(int tier, Weapon::WeaponType ttype)
{
	Weapon::WeaponType type;
	if (ttype == Weapon::WeaponType::NUMBER)
		type = Weapon::WeaponType(rand() % (Weapon::NUMBER - 1));
	else
		type = ttype;

	// Calculate aproximate cost for current tier
	int minValue, maxValue;
	switch (tier)
	{
	case 1:
		minValue = LOW_VALUE_ITEM_LEVEL1;
		maxValue = HIGH_VALUE_ITEM_LEVEL1;
		break;
	case 2:
		minValue = LOW_VALUE_ITEM_LEVEL2;
		maxValue = HIGH_VALUE_ITEM_LEVEL2;
		break;
	case 3:
		minValue = LOW_VALUE_ITEM_LEVEL3;
		maxValue = HIGH_VALUE_ITEM_LEVEL3;
		break;
	case 4:
		minValue = LOW_VALUE_ITEM_LEVEL4;
		maxValue = HIGH_VALUE_ITEM_LEVEL4;
		break;
	case 5:
		minValue = LOW_VALUE_ITEM_LEVEL5;
		maxValue = HIGH_VALUE_ITEM_LEVEL5;
		break;
	default:
		throw out_of_range("Invalid weapon level. Must be between 1 and 5");
	}

	// Range of values that weapons's statistics can acquire
	vector<pair<int, int>> weaponStatsRanges = {
		{MIN_WEAPON_DAMAGE, MIN_WEAPON_DAMAGE + WEAPON_RAND_DAM_ADDITION}, // Damage
		{getWeaponScaleLimit(type, Attribute::STRENGTH, Limit::MIN), getWeaponScaleLimit(type, Attribute::STRENGTH, Limit::MAX)}, // Strength damage addition
		{getWeaponScaleLimit(type, Attribute::DEXTERITY, Limit::MIN), getWeaponScaleLimit(type, Attribute::DEXTERITY, Limit::MAX)}, // Dexterity damage addition
		{MIN_SHIELD_PROB_ADDITION, MIN_SHIELD_PROB_ADDITION + SHIELD_RAND_PROB_ADDITION}, // Shield block probability addition
		{MIN_SHIELD_DEF_PERC_ADDITION, MIN_SHIELD_DEF_PERC_ADDITION + SHIELD_RAND_DEF_PERC_ADDITION}, // Shield block defense percent addition
	};

	// Weapon statistics, randomly generated in range
	vector<int> weaponStatsValues;
	int size = weaponStatsRanges.size();
	for (int i = 0; i < size; i++)
		weaponStatsValues.push_back(weaponStatsRanges[i].first + rand() % (weaponStatsRanges[i].second - weaponStatsRanges[i].first + 1));

	// Statistics weight
	// Weapon values damage and scale
	// Shield values defensife stats
	size = weaponStatsValues.size();
	vector<int> weights(size, 1);
	if (type == Weapon::WeaponType::SHIELD)
	{
		weights[0] = 0;
		weights[1] = 0;
		weights[2] = 0;
	}
	else
	{
		weights[3] = 0;
		weights[4] = 0;
	}

	// Average factor of all stats used to calculate value of weapon
	int averageFactor = calculateWeightedAverageFactor(weaponStatsValues, weaponStatsRanges, weights);
	int value;
	// Axe and spear are two handed weapons, they have double the damage and greater value but occupy two weapon slots
	if (type == Weapon::WeaponType::SPEAR || type == Weapon::WeaponType::AXE)
	{
		value = (100 + averageFactor) * maxValue / 100;
		weaponStatsValues[0] *= 2;
	}
	else
		value = (100 + averageFactor) * minValue / 100;

	return make_unique<Weapon>(
		Item(
			Item::ItemType::WEAPON,
			value
		),
		tier,
		weaponStatsValues[0], // Damage
		type,
		0, // Damage addition
		weaponStatsValues[1] * (tier * (100 / MAX_ARMOUR_TIER)) / 100, // Strength damage addition (adjusted for tier)
		weaponStatsValues[2] * (tier * (100 / MAX_ARMOUR_TIER)) / 100, // Dexterity damage addition (adjusted for tier)
		weaponStatsValues[3], // Shield's probability addition
		weaponStatsValues[4] * (tier * (100 / MAX_ARMOUR_TIER)) / 100, // Shield's defense percent addition (adjusted for tier)
		"" // Name
	);
}

unique_ptr<Armour> generateArmour(int tier, Armour::ArmourType ttype)
{
	// Determining the type
	Armour::ArmourType type = ttype != Armour::NUMBER ? ttype : Armour::ArmourType(rand() % Armour::NUMBER);

	// Calculating the armour statistics based on its type
	int maxStrDefAddition = getArmourScaleLimit(type, Armour::STR_ADDITION_PERC, Limit::MAX),
		maxDexDefAddition = getArmourScaleLimit(type, Armour::DEX_ADDITION_PERC, Limit::MAX);
	int evasionProbAddition, stunProbSubstraction;
	switch (type)
	{
	case Armour::LIGHT:
		evasionProbAddition = getArmourScaleLimit(type, Armour::EVASION_PROB_ADDITION, Limit::MAX) * 2 / 5 + rand() % 3;
		stunProbSubstraction = 0;
		break;
	case Armour::HEAVY:
		evasionProbAddition = 0;
		stunProbSubstraction = getArmourScaleLimit(type, Armour::STUN_PROB_SUBSTRACTION, Limit::MAX) / 2;
		break;
	}

	// Calculating value
	int minValue, maxValue;
	switch (tier)
	{
	case 1:
		minValue = LOW_VALUE_ITEM_LEVEL1;
		maxValue = HIGH_VALUE_ITEM_LEVEL1;
		break;
	case 2:
		minValue = LOW_VALUE_ITEM_LEVEL2;
		maxValue = HIGH_VALUE_ITEM_LEVEL2;
		break;
	case 3:
		minValue = LOW_VALUE_ITEM_LEVEL3;
		maxValue = HIGH_VALUE_ITEM_LEVEL3;
		break;
	case 4:
		minValue = LOW_VALUE_ITEM_LEVEL4;
		maxValue = HIGH_VALUE_ITEM_LEVEL4;
		break;
	case 5:
		minValue = LOW_VALUE_ITEM_LEVEL5;
		maxValue = HIGH_VALUE_ITEM_LEVEL5;
		break;
	default:
		throw out_of_range("Invalid armour level. Must be between 1 and 5");
	}
	int medianValue = (minValue + maxValue) / 2;

	// Range of values that armour's statistics can acquire
	vector<pair<int, int>> armourStatsRanges = {
		{MIN_ARMOUR_DEFENSE, MIN_ARMOUR_DEFENSE + ARMOUR_RAND_DEF_ADDITION}, // Defense
		{getArmourScaleLimit(type, Armour::STR_ADDITION_PERC, Limit::MIN), getArmourScaleLimit(type, Armour::STR_ADDITION_PERC, Limit::MAX)}, // Strength defense addition
		{getArmourScaleLimit(type, Armour::DEX_ADDITION_PERC, Limit::MIN), getArmourScaleLimit(type, Armour::DEX_ADDITION_PERC, Limit::MAX)}, // Dexterity defense addition
		{getArmourScaleLimit(type, Armour::EVASION_PROB_ADDITION, Limit::MIN), getArmourScaleLimit(type, Armour::EVASION_PROB_ADDITION, Limit::MAX)}, // Evasion probability addition
		{getArmourScaleLimit(type, Armour::STUN_PROB_SUBSTRACTION, Limit::MIN), getArmourScaleLimit(type, Armour::STUN_PROB_SUBSTRACTION, Limit::MAX)}, // Stun probability subtraction
	};

	// Armour statistics, randomly generated in range
	vector<int> armourStatsValues;
	int size = armourStatsRanges.size();
	for (int i = 0; i < size; i++)
		armourStatsValues.push_back(armourStatsRanges[i].first + rand() % (armourStatsRanges[i].second - armourStatsRanges[i].first + 1));

	// Statistics weight
	// Light armour values dexterity scale and evasion chance more
	// Heavy armour values strength scale and stun resistance more
	size = armourStatsValues.size();
	vector<int> weights(size, 1);
	if (type == Armour::ArmourType::LIGHT)
	{
		weights[2] = 5;
		weights[3] = 5;
		weights[4] = 0;
	}
	else
	{
		weights[1] = 5;
		weights[3] = 0;
	}

	// Average factor of all stats used to calculate value of armour
	int averageFactor = calculateWeightedAverageFactor(armourStatsValues, armourStatsRanges, weights);
	int value = (100 + averageFactor) * medianValue / 100;

	return make_unique<Armour>(
		Item(
			Item::ItemType::ARMOUR,
			value
		),
		tier,
		armourStatsValues[0], // Defense
		type,
		0, // Defense addition
		armourStatsValues[1] * (tier * (100 / MAX_ARMOUR_TIER)) / 100, // Strength defense addition (adjusted for tier)
		armourStatsValues[2] * (tier * (100 / MAX_ARMOUR_TIER)) / 100, // Dexterity defense addition (adjusted for tier)
		armourStatsValues[3], // Evasion probability addition
		armourStatsValues[4] // Stun probability subtraction
	);
}

// __________ Sound __________
void playSound(SoundEnum soundEnum)
{
	if (!game.getSoundStatus())
		return;
	// 1. Open file
	// Compose path to sound based on enum
	const string DIRECTORY = "Data/Sound/";
	const string FORMAT = ".wav";
	string path("");

	switch (soundEnum)
	{
	default:case SoundEnum::SOUND_BUTTON_CLICK: path = DIRECTORY + "buttonClick" + FORMAT; break;
	case SoundEnum::SOUND_FIGHT_HIT: path = DIRECTORY + "fightHit" + FORMAT; break;
	case SoundEnum::SOUND_FIGHT_DODGED: path = DIRECTORY + "fightDodge" + FORMAT; break;
	case SoundEnum::SOUND_FIGHT_BLOCKED: path = DIRECTORY + "fightBlock" + FORMAT; break;
	case SoundEnum::SOUND_FIGHT_CRIT: path = DIRECTORY + "fightCrit" + FORMAT; break;
	case SoundEnum::SOUND_FIGHT_STUNNED: path = DIRECTORY + "fightBlock" + FORMAT; break; // TODO: unique sound
	case SoundEnum::SOUND_FIGHT_COUNTERATTACKED: path = DIRECTORY + "fightCounterattack" + FORMAT; break;
	case SoundEnum::SOUND_GOLD: path = DIRECTORY + "gold" + FORMAT; break;
	case SoundEnum::SOUND_LEVEL_UP: path = DIRECTORY + "levelUp" + FORMAT; break;
	}

	// 2. Play selected sound
	PlaySound(path.c_str(), 0, SND_ASYNC | SND_NOSTOP);

	// TODO: remake function using MCI
	// 2. Compose full command string
	//buf = "open \"" + path + "\" type mpegvideo alias sound";
	//// Create command string
	//const char* command = buf.c_str();
	//MCIERROR error = mciSendString(command, NULL, 0, NULL);

	//// TODO: handle error
	//if (error)
	//{
	//	char errorText[128];
	//	mciGetErrorString(error, errorText, sizeof(errorText));
	//	return;
	//}

	//// 3. Play selected sound
	//command = "play sound";

	//error = mciSendString(command, NULL, 0, NULL);

	//// TODO: jandle error
	//if (error)
	//{
	//	char errorText[128];
	//	mciGetErrorString(error, errorText, sizeof(errorText));
	//	return;
	//}
}

// __________ Game process __________

LRESULT CALLBACK WFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int sx, sy;

	switch (message)
	{
	case WM_CREATE:
		// Setting the random seed
		srand((unsigned)time(0));

		// Setting language (default = English) and other settings
		buf = "Data/Settings.conf";
		if (std::experimental::filesystem::exists(buf))
		{
			ifstream fin(buf, ios::binary);
			int savedLanguage, soundStatus, autoSaveStatus;
			fin >> savedLanguage >> soundStatus >> autoSaveStatus;
			fin.close();

			l.setLanguage(static_cast<Language>(savedLanguage));
			game.setSoundStatus(soundStatus);
			game.setAutoSaveStatus(autoSaveStatus);
		}
		else
			l.setLanguage();
		// Setting menu
		game.getMenuManager().setMenu(new MainMenu(hwnd));
		break;

	case WM_COMMAND:
		// Processing user input with desired function accordingly to selected menu
		game.handleInput(hwnd, message, wParam, lParam);
		break;

	case WM_LBUTTONDOWN:
		// Processing user input with desired function accordingly to selected menu
		// Used to hide additional windows when user clicks on an empty space
		game.handleInput(hwnd, message, wParam, lParam);
		break;

	case WM_SIZE:
		// Resizing and rearranging windows on screen accordingly to selected menu
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		game.resizeWindow(sx / 2, sy / 2);
		break;

	case WM_PAINT:
		// Drawing background and changing text on windows accordingly to selected menu
		hdc = BeginPaint(hwnd, &ps);
		game.drawWindow(hwnd, hdc, sx / 2, sy / 2);
		EndPaint(hwnd, &ps);
		break;

	case WM_DRAWITEM:
	{
		// Drawing windows style accordingly to selected menu
		// If item has a custom style it is drawn using stylizeWindow()
		// Otherwise it is drawn using generic style
		LRESULT result = 0;
		if (!game.stylizeWindow(hwnd, message, wParam, lParam, result))
		{
			LPDRAWITEMSTRUCT item = (LPDRAWITEMSTRUCT)lParam;
			HDC hdc = item->hDC;
			RECT rect = item->rcItem;

			GetClassName(item->hwndItem, str, sizeof(str) / sizeof(str[0]));

			// Set text font and background
			SelectObject(hdc, game.getFont(Game::FontSize::MEDIUM));
			SetBkMode(hdc, TRANSPARENT);

			// Get text
			int len = GetWindowTextLength(item->hwndItem);
			buf.resize(len + 1); // Resize buffer to contain button text
			GetWindowTextA(item->hwndItem, &buf[0], len + 1); // Write text into buffer

			SetTextColor(hdc, COLOR_WHITE); // Set basic text color

			if (item->CtlType == ODT_STATIC) // Static windows
			{
				//SetTextColor(hdc, RGB(0,0,0)); // Set basic text color
				FillRect(hdc, &rect, CreateSolidBrush(COLOR_DARK_BLUE)); // Fill background
				DrawTextA(item->hDC, buf.c_str(), len, &rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER); // Display text
				DrawEdge(hdc, &rect, EDGE_SUNKEN, BF_RECT); // Draw edge
			}
			else if (strcmp(str, ("Edit")) == 0) // Edit windows
			{
				DrawEdge(hdc, &rect, EDGE_SUNKEN, BF_RECT);
			}
			else if (item->CtlType == ODT_BUTTON) // Button windows
			{
				// Fill background
				if (item->itemState & ODS_SELECTED)
					FillRect(hdc, &rect, CreateSolidBrush(COLOR_ROMAN_RED_PUSHED));
				else
					FillRect(hdc, &rect, CreateSolidBrush(COLOR_ROMAN_RED));

				DrawTextA(item->hDC, buf.c_str(), len, &rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER); // Display text
				DrawEdge(hdc, &rect, EDGE_RAISED, BF_RECT); // Draw edge
			}
			else if (item->CtlType == ODT_LISTBOX) // Listbox windows
			{
				// Get text
				SendMessage(item->hwndItem, LB_GETTEXT, item->itemID, (LPARAM)str);

				// Fill background
				if (item->itemState & ODS_FOCUS)
					SetBkColor(hdc, COLOR_ROMAN_RED_PUSHED);
				else
					SetBkColor(hdc, COLOR_ROMAN_RED);

				// Text padding
				RECT rectPadding = item->rcItem;
				int padding = 5;
				rectPadding.left += padding;
				// Write text
				ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
				DrawText(hdc, str, -1, &rectPadding, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				// Draw edge
				DrawEdge(hdc, &rect, EDGE_RAISED, BF_RECT); 
			}
			else if (item->CtlType == ODT_COMBOBOX)
			{
				// Get text
				SendMessage(item->hwndItem, CB_GETLBTEXT, item->itemID, (LPARAM)str);

				// Fill background
				if (item->itemState & ODS_FOCUS)
					SetBkColor(hdc, COLOR_ROMAN_RED_PUSHED);
				else
					SetBkColor(hdc, COLOR_ROMAN_RED);

				// Text padding
				RECT rectPadding = item->rcItem;
				int padding = 5;
				rectPadding.left += padding;
				// Write text
				ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
				DrawText(hdc, str, -1, &rectPadding, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				// Draw edge
				DrawEdge(hdc, &rect, EDGE_RAISED, BF_RECT);
			}
		}
		return true;
	}
	break;

	case WM_MEASUREITEM:
	{
		LPMEASUREITEMSTRUCT item = (LPMEASUREITEMSTRUCT)lParam;
		if (item->CtlType == ODT_LISTBOX)
		{
			item->itemHeight = 30;
		}
		if (item->CtlType == ODT_COMBOBOX)
		{
			item->itemHeight = 35;
		}
	}
	break;

	case WM_CTLCOLORSTATIC:
	{
		LRESULT result = 0;
		if (game.stylizeWindow(hwnd, message, wParam, lParam, result))
			return result;
	}
	break;

	case WM_CTLCOLOREDIT:
	{
		LRESULT result = 0;
		if (game.stylizeWindow(hwnd, message, wParam, lParam, result))
			return result;
	}
	break;

	case WM_CTLCOLORLISTBOX:
	{
		LRESULT result = 0;
		if (game.stylizeWindow(hwnd, message, wParam, lParam, result))
			return result;
	}
	break;

	case WM_MOUSEMOVE:
		// TODO: improve | delete ?
		// An attempt to handle user placing cursor over button without clicking. Currently only used in MainMenu
		game.handleInput(hwnd, message, wParam, lParam);
		break;

	case WM_MOUSEHOVER:
		// TODO: improve | delete ?
		// An attempt to handle user placing cursor over button without clicking. Currently only used in MainMenu
		game.handleInput(hwnd, message, wParam, lParam);
		break;

	case WM_MOUSELEAVE:
		// TODO: improve | delete ?
		// An attempt to handle user placing cursor over button without clicking. Currently only used in MainMenu
		game.handleInput(hwnd, message, wParam, lParam);
		break;

	case WM_DESTROY:

		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0L;
}
