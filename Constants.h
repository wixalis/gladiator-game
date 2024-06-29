#pragma once

// World's statistics
const int MAX_CITIES = 11;
const int OPPONENTS_NUMBER = 15;
const int BASIC_REGEN = 30;
const int BASIC_ATTRIBUTES = 10;
//const string OUTPUT_DIVIDER = "\n______________________________________________\n\n\n";

// Player's and NPC's statistics
const int MIN_ATTRIBUTE = 1, MAX_ATTRIBUTE = 100;
const int MIN_STRENGTH = 1, MAX_STRENGTH = 100;
const int MIN_CONSTITUTION = 1, MAX_CONSTITUTION = 100;
const int MIN_DEXTERITY = 1, MAX_DEXTERITY = 100;
const int MIN_INTELLIGENCE = 1, MAX_INTELLIGENCE = 100;
const int MIN_WISDOM = 1, MAX_WISDOM = 100;
const int MIN_CHARISMA = 1, MAX_CHARISMA = 100;
const int MIN_AGE = 20, MAX_AGE = 60;
const int MIN_FAME = 0, BASIC_FAME = 100;
// Fighter's statistics
const int BASIC_HP = 100;
const int MAX_ADDITIONAL_HP_STRENGTH = 30;
const int MAX_ADDITIONAL_HP_CONSTITUTION = 70;
// Leveling statistics
const int MIN_LEVEL = 1;
const int MAX_LEVEL = 100;
const int EXPERIENCE_PER_LEVEL = 200; // Experience needed for 1 level up (first level, then its increases by percentage)
const int EXPERIENCE_PER_LEVEL_INCREASE_PERC = 2; // 2.0% more experience needed for every next level from 1
const int ATTRIBUTES_PER_LEVEL = 5;
const double MIN_EXPERIENCE_MULTIPLIER = 0.1;
const double MAX_EXPERIENCE_MULTIPLIER = 2.5;

// Trader's statistics
const int MIN_TRADER_LEVEL = 1, MAX_TRADER_LEVEL = 5; // Trader level affects it's goods (weapon and armour level)

// Inventory's statistics
const int MAX_INVENTORY_SIZE = 14;

// Item's statistics
const int BASIC_ITEM_VALUE = 1;
const int MAX_PRICE_MULTIPLIER = 30; // Charisma of the player influences prices of item. At 0 charisma price is increased by 30%. At 100 charisma price is decrease by 30%.
const int LOW_VALUE_ITEM_LEVEL1 = 100, HIGH_VALUE_ITEM_LEVEL1 = 300, // value for weapon and armour of different levels
		  LOW_VALUE_ITEM_LEVEL2 = 300, HIGH_VALUE_ITEM_LEVEL2 = 500,
	  	  LOW_VALUE_ITEM_LEVEL3 = 500, HIGH_VALUE_ITEM_LEVEL3 = 700,
		  LOW_VALUE_ITEM_LEVEL4 = 700, HIGH_VALUE_ITEM_LEVEL4 = 1000,
		  LOW_VALUE_ITEM_LEVEL5 = 1000, HIGH_VALUE_ITEM_LEVEL5 = 1500;

// Weapon's statistics
const int MIN_WEAPON_TIER = MIN_TRADER_LEVEL,
		  MAX_WEAPON_TIER = MAX_TRADER_LEVEL;
const int MIN_WEAPON_DAMAGE = 20;
const int WEAPON_RAND_DAM_ADDITION = 6;
const Weapon::WeaponType BASIC_WEAPON_TYPE = Weapon::SWORD;
const int MIN_SHIELD_PROB_ADDITION = 30;
const int SHIELD_RAND_PROB_ADDITION = 10;
const int MIN_SHIELD_DEF_PERC_ADDITION = 30;
const int SHIELD_RAND_DEF_PERC_ADDITION = 20;

// Armour's statistics
const int MIN_ARMOUR_TIER = MIN_TRADER_LEVEL,
		  MAX_ARMOUR_TIER = MAX_TRADER_LEVEL;
const Armour::ArmourType BASIC_ARMOUR_TYPE = Armour::LIGHT;
/*
 * OUTDATED
 * TODO: UPDATE
 * Maximum values of the parameters are assumed.
 *
 * HP - (Damage - Defense).
 * HP == Basic HP + Constitution + Strength:
 * 500 == 100 + 300 + 100.
 *
 * Weapon damage == Basic weapon damage + Strength scale (50~10) + Dexterity scale (50~10).
 * Damage == Strength + Dexterity + Weapon damage:
 * 450 == 100 + 100 + (25 + 50 + 50) * 2.
 *
 * Armour defense compensates weapon damage.
 * Armour defense == Basic armour defense + Strength scale (25~5) + Dexterity scale (75~15).
 * 250 == 50 + 50 + 150.
 *
 * ~Number of hits for one == 4 == 500 / (200 * (1000 - 375) / 1000).
 * [* (1000 - 375) / 1000] is 37.5% damage reduction to prolong a fight.
 * Fight == ~(4 * 2) hits.
 *
 * Gladitor values.
 * Damage: 72 == 10 + 10 + (20 + 3 + 3) * 2.
 * Armour: 40 == 35 + 1 + 4.
 * ~Number of hits for one == 7 == 140 / (32 * (1000 - 375) / 1000).
 *
 * Minimum values.
 * HP: 140 == 100 + 30 + 10.
 * Damage: 64 == 10 + 10 + (20 + 1 + 1) * 2.
 * Armour: 31 == 30 + 0 + 1.
 * ~Number of hits for one == 7 == 140 / (33 * (1000 - 375) / 1000).
 * Fight == ~(7 * 2) hits.
 */
const int MIN_ARMOUR_DEFENSE = 30, MAX_ARMOUR_DEFENSE = 50;
const int ARMOUR_RAND_DEF_ADDITION = 6;

// Fight's statistics
const int ONE_HUNDRED_PERCENT = 1000; // 100.0%
const int DAMAGE_REDUCTION_PERCENT = 375; // 1000 = 100%
const int WEAPON_MAX_RAND_DMG_SPREAD_PERCENT = 10; // 10.0%
const int MAX_DAMAGE_FROM_STATS = 70; // Strength and dexterity
const int VICTORY_LOOT_CHANCE = 30; // 30.0%
const int FAME_FOR_LEVEL_MULTIPLIER = 100; // Used to calculateFameEquilibrium. Level * FAME_FOR_LEVEL_MULTIPLIER = fameEquilibrium
const int FAME_DISPERSION = 200; // rand() % FAME_DISPERSION -- gained fame for fight (amongst other modifiers)
const int FAME_GAIN_FROM_OPPONENT_PERC = 10; // 10.0%
const int MAX_FAME_GAINED_FOR_EQUILIBRIUM = 200;
const int MAX_FAME_GAINED_FROM_OPPONENT = 200;
const int FAME_FOR_PERC_GOLD_MULTIPLIER = 150; // Every x fame increases player's gold reward by 1%

// Game settings
const int FIGHT_SLEEP_TIME = 1000; // Delay between attacks in fight
const int STORY_SLEEP_TIME = 300; // Delay between story screen display
const int STARTING_ADDITIONAL_ATTRIBUTES = 30;
const int ATTRIBUTE_MAX_DIFFERENCE = 20; // How much a player can increase attributes relative to average attributes level
const int MONEY_NEEDED_FOR_FREEDOM = 1500; // How much gold player needs to progress story
const int CHARISMA_FOR_PROMOTION = 30; // How much charisma player needs to gain promotion on arena from lanista
const int CITY_TRAVEL_COST = 100; // Gold needed to travel to another city

// Quest settings
// Perugia
// Shandar and Abhilasha
const int ATTRIBUTES_NEEDED = 20; // How much of a certain attribute a player needs to pass attribute check

const COLORREF COLOR_WHITE = RGB(255, 255, 255),
			   COLOR_STATIC_BROWN = RGB(119, 89, 59),
			   COLOR_ROMAN_RED = RGB(80, 0, 0),
			   COLOR_ROMAN_RED_PUSHED = RGB(50, 0, 0),
			   COLOR_DARK_BLUE = RGB(15, 15, 30),
			   COLOR_ORANGE = RGB(140, 40, 0);

const int ROME_CITY_LEVEL = 80,
		  NAPLES_CITY_LEVEL = 70,
		  METAPONTO_CITY_LEVEL = 20,
		  BOJANO_CITY_LEVEL = 30,
		  ANCONA_CITY_LEVEL = 20,
		  PERUGIA_CITY_LEVEL = 10,
		  FLORENCE_CITY_LEVEL = 40,
		  BOLOGNA_CITY_LEVEL = 30,
		  GENOA_CITY_LEVEL = 40,
		  AQUILEIA_CITY_LEVEL = 50,
		  MILAN_CITY_LEVEL = 60;

const int ROME_TRADER_LEVEL = 5,
		  NAPLES_TRADER_LEVEL = 5,
		  METAPONTO_TRADER_LEVEL = 2,
		  BOJANO_TRADER_LEVEL = 2,
	  	  ANCONA_TRADER_LEVEL = 2,
		  PERUGIA_TRADER_LEVEL = 1,
		  FLORENCE_TRADER_LEVEL = 3,
		  BOLOGNA_TRADER_LEVEL = 2,
		  GENOA_TRADER_LEVEL = 3,
		  AQUILEIA_TRADER_LEVEL = 4,
		  MILAN_TRADER_LEVEL = 4;

