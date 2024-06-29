// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Enabling visual styles
// For detailed information see https://learn.microsoft.com/en-us/windows/win32/controls/cookbook-overview#using-manifests-or-directives-to-ensure-that-visual-styles-can-be-applied-to-applications
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// __________ Dependencies __________
#include <array> // `array` for localization
#include <cstdlib> // `rand()`, `srand()`
#include <ctime> // `time()`
#include <fstream> // `ifstream`, `ofstream`
#include <iostream>
#include <string>
#include <vector>
#include <map> // `map` for inventory
#include <exception>
#include <memory> // 'unique_ptr'
#include <filesystem> // for loading methods
#include <regex> // for 'parseDateTime()'
using namespace std;

#include <Windows.h>
// Libraries for MCI (Media Control Interface) - sound effects, music
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include "resource.h"

// __________ User dependencies __________
#include "LocalizationEnums.h"

#include "NewMenuStorage.h"
#include "StartingEquipment.h"
#include "PlayerAttributeStorage.h"
#include "FolderInfo.h"
#include "IMenu.h"
#include "StoryScreen.h"
#include "MainMenu.h"
#include "GameMenu.h"

#include "Item.h"
#include "PairRef.h"
#include "Inventory.h"
#include "CityMenu.h"
#include "MenuManager.h"

#include "Enums.h"
#include "Statistics.h"

#include "Weapon.h"
#include "Armour.h"

#include "Fighter.h"
#include "Leveling.h"
#include "Player.h"
#include "NamedNPC.h"
#include "HarmlessNPC.h"
#include "NPC.h"

#include "Converter.h"
#include "Constants.h"
#include "Fighting.h"

#include "Arena.h"
#include "City.h"
#include "WorldMap.h"
#include "Game.h"

#include "Localization.h"

const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
extern Localization l;
extern Game game;

// __________ Functions __________

// __________ Other __________
void updateWindow(HWND);
//void adjustStaticControlHeight(HWND hStatic_, const string& text_);
string toStringPrecision(double number_, int decimalPlaces_ = 2);
tm parseDateTime(const string& folderName_);
bool hasSubdirectory(const string& directoryPath_);
void getFoldersInDirectory(const string& directoryPath_, vector<string>& folderNames_);
string formatSaveName(const string& input_);
int normalize(int value_, int min_, int max_);
int calculateWeightedAverageFactor(const vector<int>& values_, const vector<pair<int, int>>& ranges_, const vector<int>& weights_);

// __________ Statistics __________
int calculateFameForLevel(int level_);

// __________ NPC __________
unique_ptr<NPC> generateNPC(int aproximateLevel_ = 6);
bool compareNPC(const unique_ptr<NPC>&, const unique_ptr<NPC>&);
unique_ptr<HarmlessNPC> generateTrader(int level_ = MIN_TRADER_LEVEL);

// __________ Weapon and Armour __________
int getWeaponScaleLimit(Weapon::WeaponType, Attribute, Limit);
int getArmourScaleLimit(Armour::ArmourType, Armour::Stat, Limit);
unique_ptr<Weapon> generateWeapon(int tier_ = MIN_WEAPON_TIER, Weapon::WeaponType = Weapon::NUMBER);
unique_ptr<Armour> generateArmour(int tier_ = MIN_ARMOUR_TIER, Armour::ArmourType = Armour::NUMBER);

// __________ Sound __________
void playSound(SoundEnum);

// __________ Game process __________
LRESULT CALLBACK WFunc(HWND, UINT, WPARAM, LPARAM);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
