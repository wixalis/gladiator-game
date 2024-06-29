#include "stdafx.h"
#include "MainMenu.h"

extern TCHAR str[256];
extern string buf;

extern HINSTANCE hInst;
extern Localization l;
extern Game game;

MainMenu::MainMenu() :
	hItems(),
	hSubItems(),
	hSubMenuItems(),
	nms(),
	hBackgroundImage(NULL),
	hBackgroundBrush(NULL),
	selectedLanguage(Language::ENGLISH),
	changedSettings(false),
	selected(-1)
{
	// Generate 3 sets of random starting equipment
	se.push_back(make_unique<StartingEquipment>());
	se.push_back(make_unique<StartingEquipment>());
	se.push_back(make_unique<StartingEquipment>());

	hasSavedGames = hasSubdirectory("Saves/") ? true : false;
}

MainMenu::MainMenu(HWND hWnd) :
	hItems(MenuItem::ITEM_NUMBER),
	hSubItems(),
	hSubMenuItems(),
	nms(),
	hBackgroundImage(NULL),
	hBackgroundBrush(NULL),
	selectedLanguage(Language::ENGLISH),
	changedSettings(false),
	selected(-1)
{
	// Create starting windows
	char className[256] = "BUTTON";
	hItems[BUT_CONTINUE] = CreateWindow(className, l.getMessage(Localized::CONTINUE_GAME).c_str(),
		WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);

	hItems[BUT_LOAD_GAME] = CreateWindow(className, l.getMessage(Localized::LOAD_GAME).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_NEW_GAME] = CreateWindow(className, l.getMessage(Localized::NEW_GAME).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_SETTINGS] = CreateWindow(className, l.getMessage(Localized::SETTINGS).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_SPECIALS] = CreateWindow(className, l.getMessage(Localized::SPECIALS).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_EXIT] = CreateWindow(className, l.getMessage(Localized::EXIT).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);

	// Check if at least one save is available
	hasSavedGames = hasSubdirectory("Saves/") ? true : false;
	// If yes - show 'Continue' button
	if (hasSavedGames)
	{
		ShowWindow(hItems[BUT_CONTINUE], SW_SHOW);
		ShowWindow(hItems[BUT_LOAD_GAME], SW_SHOW);
	}
	else
	{
		ShowWindow(hItems[BUT_CONTINUE], SW_HIDE);
		ShowWindow(hItems[BUT_LOAD_GAME], SW_HIDE);
	}

	// Generate 3 sets of random starting equipment
	se.push_back(make_unique<StartingEquipment>());
	se.push_back(make_unique<StartingEquipment>());
	se.push_back(make_unique<StartingEquipment>());
}

MainMenu::MainMenu(const MainMenu& MM) : hSubItems(), hSubMenuItems()
{
	// Resizing items' vector
	int sz = MM.hItems.size();
	hItems.resize(sz);

	HWND hItem;
	RECT rect;
	char className[256], windowText[256];
	for (int i = 0; i < sz; i++)
	{
		// Obtaining the previous parameters
		hItem = MM.hItems[i];
		GetClassName(hItem, className, sizeof(className));
		GetWindowText(hItem, windowText, sizeof(windowText));
		GetWindowRect(hItem, &rect);

		// Creating new window
		hItems[i] = CreateWindow(
			className,
			windowText,
			GetWindowLongPtr(hItem, GWL_STYLE),
			rect.left,
			rect.top,
			rect.right - rect.left,
			rect.bottom - rect.top,
			GetParent(hItem),
			0,
			hInst,
			0
		);
	}

	if (MM.hBackgroundImage != NULL)
	{
		BITMAP bm;
		GetObject(MM.hBackgroundImage, sizeof(BITMAP), &bm);
		hBackgroundImage = CreateBitmapIndirect(&bm);
	}
	else
		hBackgroundImage = NULL;

	if (MM.hBackgroundBrush != NULL)
	{
		LOGBRUSH lb;
		GetObject(MM.hBackgroundBrush, sizeof(LOGBRUSH), &lb);
		hBackgroundBrush = CreateBrushIndirect(&lb);
	}
	else
		hBackgroundBrush = NULL;

	// New menu storage
	nms = make_unique<NewMenuStorage>();

	// Starting equipment
	for (const auto& equipmentSet : MM.se)
		se.push_back(make_unique<StartingEquipment>(*equipmentSet));

	selectedLanguage = MM.selectedLanguage;
	changedSettings = MM.changedSettings;
	selected = MM.selected;
	hasSavedGames = MM.hasSavedGames;
}

MainMenu& MainMenu::operator=(const MainMenu& MM)
{
	if (&MM == this) return *this;

	// Destroying the previous items of the menu
	for (HWND hItem : hItems)
		if (hItem != NULL)
			DestroyWindow(hItem);

	// Resizing items' vector
	int sz = MM.hItems.size();
	hItems.resize(sz);

	HWND hItem;
	RECT rect;
	char className[256], windowText[256];
	for (int i = 0; i < sz; i++)
	{
		// Obtaining the previous parameters
		hItem = MM.hItems[i];
		GetClassName(hItem, className, sizeof(className));
		GetWindowText(hItem, windowText, sizeof(windowText));
		GetWindowRect(hItem, &rect);

		// Creating new window
		hItems[i] = CreateWindow(
			className,
			windowText,
			GetWindowLongPtr(hItem, GWL_STYLE),
			rect.left,
			rect.top,
			rect.right - rect.left,
			rect.bottom - rect.top,
			GetParent(hItem),
			0,
			hInst,
			0
		);
	}

	hSubItems = vector<HWND>();
	hSubMenuItems = vector<HWND>();

	// New menu storage
	if (MM.nms)
	{
		if (!nms)
			nms = make_unique<NewMenuStorage>(*MM.nms);
		else
			*nms = *MM.nms;
	}
	else
		nms = nullptr;

	// Starting equipment
	se.clear();
	for (const auto& equipmentSet : MM.se)
		se.push_back(make_unique<StartingEquipment>(*equipmentSet));

	// Image
	if (hBackgroundImage != NULL)
		DeleteObject(hBackgroundImage);

	if (MM.hBackgroundImage != NULL)
	{
		BITMAP bm;
		GetObject(MM.hBackgroundImage, sizeof(BITMAP), &bm);
		hBackgroundImage = CreateBitmapIndirect(&bm);
	}
	else
		hBackgroundImage = NULL;

	if (hBackgroundBrush != NULL)
		DeleteObject(hBackgroundBrush);

	if (MM.hBackgroundBrush != NULL)
	{
		LOGBRUSH lb;
		GetObject(MM.hBackgroundBrush, sizeof(LOGBRUSH), &lb);
		hBackgroundBrush = CreateBrushIndirect(&lb);
	}
	else
		hBackgroundBrush = NULL;

	selectedLanguage = MM.selectedLanguage;
	changedSettings = MM.changedSettings;
	selected = MM.selected;
	hasSavedGames = MM.hasSavedGames;

	return *this;
}

MainMenu::~MainMenu()
{
	for (HWND hItem : hItems)
		if (hItem != NULL)
			DestroyWindow(hItem);

	for (HWND hItem : hSubItems)
		if (hItem != NULL)
			DestroyWindow(hItem);

	for (HWND hItem : hSubMenuItems)
		if (hItem != NULL)
			DestroyWindow(hItem);

	if (hBackgroundImage != NULL)
		DeleteObject(hBackgroundImage);

	if (hBackgroundBrush != NULL)
		DeleteObject(hBackgroundBrush);
}

void MainMenu::drawMenu(HWND hWnd, HDC hdc, int cx, int cy)
{
	// 1. Background
	if (game.isBackgroundChanged())
	{
		const string DIRECTORY = "Data/Image/Background/";
		const string FORMAT = ".bmp";
		string path("");
		RECT rect;

		GetClientRect(hWnd, &rect);

		// Composing path based on current menu
		switch (game.getBackground())
		{
		default:case Game::Background::MAIN_MENU:
			path = DIRECTORY + "menuBackground" + FORMAT;
			for (HWND hItem : hItems)
				ShowWindow(hItem, SW_SHOW);
			if (!hasSavedGames)
			{
				ShowWindow(hItems[BUT_CONTINUE], SW_HIDE);
				ShowWindow(hItems[BUT_LOAD_GAME], SW_HIDE);
			}
			break;
		case Game::Background::MAIN_MENU_LOAD: path = DIRECTORY + "menuBackground" + FORMAT; break; // TODO: background
		case Game::Background::MAIN_MENU_NEW_GAME: path = DIRECTORY + "characterCreationBackground" + FORMAT; break;
		case Game::Background::MAIN_MENU_NEW_GAME_EQUIPMENT: path = DIRECTORY + "startingEquipmentBackground" + FORMAT; break;
		case Game::Background::MAIN_MENU_SETTINGS: path = DIRECTORY + "menuBackground" + FORMAT; break; // TODO: background
		case Game::Background::MAIN_MENU_SPECIALS: path = DIRECTORY + "menuBackground" + FORMAT; break; // TODO: background
		}
		// Loading image
		if (hBackgroundImage != NULL)
			DeleteObject(hBackgroundImage);
		hBackgroundImage = (HBITMAP)LoadImage(0, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		// Filling background with selected image
		if (hBackgroundBrush != NULL)
			DeleteObject(hBackgroundBrush);
		hBackgroundBrush = CreatePatternBrush(hBackgroundImage);
		FillRect(hdc, &rect, hBackgroundBrush);
		game.backgroundChangeCompleted();
	}

	// 2. Text
	switch (game.getBackground())
	{
	default:case Game::Background::MAIN_MENU: break;
	case Game::Background::MAIN_MENU_LOAD: break;
	case Game::Background::MAIN_MENU_NEW_GAME: break;
	case Game::Background::MAIN_MENU_NEW_GAME_EQUIPMENT:
	{
		// General
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_CHOOSE], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::CHOOSE_STARTING_EQUIPMENT).c_str());
		SendMessage(hSubMenuItems[EQUIPMENT_BUT_NEXT], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::NEXT).c_str());
		SendMessage(hSubMenuItems[EQUIPMENT_BUT_BACK], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::BACK).c_str());
		for (int i = EQUIPMENT_BUT_SET1; i <= EQUIPMENT_BUT_SET3; i++)
			SendMessage(hSubMenuItems[i], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::SELECT).c_str());

		// Equipment
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET1_RIGHT_HAND], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::RIGHT_HAND).c_str());
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET2_RIGHT_HAND], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::RIGHT_HAND).c_str());
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET3_RIGHT_HAND], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::RIGHT_HAND).c_str());

		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET1_LEFT_HAND], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::LEFT_HAND).c_str());
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET2_LEFT_HAND], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::LEFT_HAND).c_str());
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET3_LEFT_HAND], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::LEFT_HAND).c_str());

		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET1_ARMOUR], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::ARMOUR).c_str());
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET2_ARMOUR], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::ARMOUR).c_str());
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET3_ARMOUR], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::ARMOUR).c_str());

		// Get equipment
		// Left hand is a unique_ptr since it can be empty thus unable to be received by reference
		Weapon& rRightHand1 = *se[0]->rightHand;
		auto& pLeftHand1 = se[0]->leftHand;
		Armour& rArmour1 = *se[0]->armour;

		Weapon& rRightHand2 = *se[1]->rightHand;
		auto& pLeftHand2 = se[1]->leftHand;
		Armour& rArmour2 = *se[1]->armour;

		Weapon& rRightHand3 = *se[2]->rightHand;
		auto& pLeftHand3 = se[2]->leftHand;
		Armour& rArmour3 = *se[2]->armour;

		// Set 1
		{
		buf = l.getMessage(Localized::SET) + " 1";
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET1], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		// Right hand
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET1_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)l.getWeaponTypeName(rRightHand1).c_str());

		buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rRightHand1.getDamage()) + " (+" + to_string(rRightHand1.getDamageAddition()) + ")";
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET1_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		buf = l.getMessage(Localized::STRENGTH_SCALE) + ": " + to_string(rRightHand1.getStrengthAdditionPercentage()) + "%";
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET1_RIGHT_HAND_STRENGTH_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		buf = l.getMessage(Localized::DEXTERITY_SCALE) + ": " + to_string(rRightHand1.getDexterityAdditionPercentage()) + "%";
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET1_RIGHT_HAND_DEXTERITY_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		// Left hand
		if (pLeftHand1)
		{
			SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET1_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)l.getWeaponTypeName(*pLeftHand1).c_str());

			if (pLeftHand1->getWeaponType() != Weapon::WeaponType::SHIELD)
			{
				buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(pLeftHand1->getDamage()) + " (+" + to_string(pLeftHand1->getDamageAddition()) + ")";
				SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET1_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				buf = l.getMessage(Localized::STRENGTH_SCALE) + ": " + to_string(pLeftHand1->getStrengthAdditionPercentage()) + "%";
				SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET1_LEFT_HAND_STRENGTH_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				buf = l.getMessage(Localized::DEXTERITY_SCALE) + ": " + to_string(pLeftHand1->getDexterityAdditionPercentage()) + "%";
				SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET1_LEFT_HAND_DEXTERITY_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
			}
			else
			{
				buf = l.getMessage(Localized::BLOCK_DEFENSE) + ": " + to_string(pLeftHand1->getShieldDefPercentAddition()) + "%";
				SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET1_LEFT_HAND_SHIELD_DEFENSE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				buf = l.getMessage(Localized::BLOCK_CHANCE) + ": " + to_string(pLeftHand1->getShieldProbAddition()) + "%";
				SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET1_LEFT_HAND_SHIELD_BLOCK_CHANCE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
			}
		}
		else
			SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET1_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::EMPTY_HAND).c_str());

		// Armour
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET1_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)l.getArmourTypeName(rArmour1).c_str());

		buf = l.getMessage(Localized::ARMOUR_DEFENSE) + ": " + to_string(rArmour1.getDefense()) + " (+" + to_string(rArmour1.getDefAddition()) + ")";
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET1_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		buf = l.getMessage(Localized::STRENGTH_SCALE) + ": " + to_string(rArmour1.getStrengthAdditionPercentage()) + "%";
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET1_ARMOUR_STRENGTH_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		buf = l.getMessage(Localized::DEXTERITY_SCALE) + ": " + to_string(rArmour1.getDexterityAdditionPercentage()) + "%";
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET1_ARMOUR_DEXTERITY_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		if (rArmour1.getArmourType() == Armour::ArmourType::LIGHT)
			buf = l.getMessage(Localized::EVASION_CHANCE) + ": " + to_string(rArmour1.getEvasionProbAddition()) + "%";
		else
			buf = l.getMessage(Localized::STUN_RESISTANCE_CHANCE) + ": " + to_string(rArmour1.getStunProbSubtraction()) + "%";
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET1_ARMOUR_ABILITY], WM_SETTEXT, 0, (LPARAM)buf.c_str());
		}

		// Set 2
		{
		buf = l.getMessage(Localized::SET) + " 2";
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET2], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		// Right hand
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET2_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)l.getWeaponTypeName(rRightHand2).c_str());

		buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rRightHand2.getDamage()) + " (+" + to_string(rRightHand2.getDamageAddition()) + ")";
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET2_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		buf = l.getMessage(Localized::STRENGTH_SCALE) + ": " + to_string(rRightHand2.getStrengthAdditionPercentage()) + "%";
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET2_RIGHT_HAND_STRENGTH_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		buf = l.getMessage(Localized::DEXTERITY_SCALE) + ": " + to_string(rRightHand2.getDexterityAdditionPercentage()) + "%";
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET2_RIGHT_HAND_DEXTERITY_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		// Left hand
		if (pLeftHand2)
		{
			SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET2_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)l.getWeaponTypeName(*pLeftHand2).c_str());

			if (pLeftHand2->getWeaponType() != Weapon::WeaponType::SHIELD)
			{
				buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(pLeftHand2->getDamage()) + " (+" + to_string(pLeftHand2->getDamageAddition()) + ")";
				SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET2_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				buf = l.getMessage(Localized::STRENGTH_SCALE) + ": " + to_string(pLeftHand2->getStrengthAdditionPercentage()) + "%";
				SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET2_LEFT_HAND_STRENGTH_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				buf = l.getMessage(Localized::DEXTERITY_SCALE) + ": " + to_string(pLeftHand2->getDexterityAdditionPercentage()) + "%";
				SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET2_LEFT_HAND_DEXTERITY_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
			}
			else
			{
				buf = l.getMessage(Localized::BLOCK_DEFENSE) + ": " + to_string(pLeftHand2->getShieldDefPercentAddition()) + "%";
				SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET2_LEFT_HAND_SHIELD_DEFENSE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				buf = l.getMessage(Localized::BLOCK_CHANCE) + ": " + to_string(pLeftHand2->getShieldProbAddition()) + "%";
				SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET2_LEFT_HAND_SHIELD_BLOCK_CHANCE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
			}
		}
		else
			SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET2_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::EMPTY_HAND).c_str());
		
		// Armour
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET2_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)l.getArmourTypeName(rArmour2).c_str());

		buf = l.getMessage(Localized::ARMOUR_DEFENSE) + ": " + to_string(rArmour2.getDefense()) + " (+" + to_string(rArmour2.getDefAddition()) + ")";
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET2_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		buf = l.getMessage(Localized::STRENGTH_SCALE) + ": " + to_string(rArmour2.getStrengthAdditionPercentage()) + "%";
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET2_ARMOUR_STRENGTH_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		buf = l.getMessage(Localized::DEXTERITY_SCALE) + ": " + to_string(rArmour2.getDexterityAdditionPercentage()) + "%";
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET2_ARMOUR_DEXTERITY_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		if (rArmour2.getArmourType() == Armour::ArmourType::LIGHT)
			buf = l.getMessage(Localized::EVASION_CHANCE) + ": " + to_string(rArmour2.getEvasionProbAddition()) + "%";
		else
			buf = l.getMessage(Localized::STUN_RESISTANCE_CHANCE) + ": " + to_string(rArmour2.getStunProbSubtraction()) + "%";
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET2_ARMOUR_ABILITY], WM_SETTEXT, 0, (LPARAM)buf.c_str());
		}

		// Set 3
		{
		buf = l.getMessage(Localized::SET) + " 3";
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET3], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		// Right hand
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET3_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)l.getWeaponTypeName(rRightHand3).c_str());

		buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rRightHand3.getDamage()) + " (+" + to_string(rRightHand3.getDamageAddition()) + ")";
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET3_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		buf = l.getMessage(Localized::STRENGTH_SCALE) + ": " + to_string(rRightHand3.getStrengthAdditionPercentage()) + "%";
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET3_RIGHT_HAND_STRENGTH_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		buf = l.getMessage(Localized::DEXTERITY_SCALE) + ": " + to_string(rRightHand3.getDexterityAdditionPercentage()) + "%";
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET3_RIGHT_HAND_DEXTERITY_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		// Left hand
		if (pLeftHand3)
		{
			SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET3_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)l.getWeaponTypeName(*pLeftHand3).c_str());

			if (pLeftHand3->getWeaponType() != Weapon::WeaponType::SHIELD)
			{
				buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(pLeftHand3->getDamage()) + " (+" + to_string(pLeftHand3->getDamageAddition()) + ")";
				SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET3_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				buf = l.getMessage(Localized::STRENGTH_SCALE) + ": " + to_string(pLeftHand3->getStrengthAdditionPercentage()) + "%";
				SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET3_LEFT_HAND_STRENGTH_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				buf = l.getMessage(Localized::DEXTERITY_SCALE) + ": " + to_string(pLeftHand3->getDexterityAdditionPercentage()) + "%";
				SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET3_LEFT_HAND_DEXTERITY_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
			}
			else
			{
				buf = l.getMessage(Localized::BLOCK_DEFENSE) + ": " + to_string(pLeftHand3->getShieldDefPercentAddition()) + "%";
				SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET3_LEFT_HAND_SHIELD_DEFENSE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				buf = l.getMessage(Localized::BLOCK_CHANCE) + ": " + to_string(pLeftHand3->getShieldProbAddition()) + "%";
				SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET3_LEFT_HAND_SHIELD_BLOCK_CHANCE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
			}
		}
		else
			SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET3_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::EMPTY_HAND).c_str());

		// Armour
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET3_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)l.getArmourTypeName(rArmour3).c_str());

		buf = l.getMessage(Localized::ARMOUR_DEFENSE) + ": " + to_string(rArmour3.getDefense()) + " (+" + to_string(rArmour3.getDefAddition()) + ")";
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET3_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		buf = l.getMessage(Localized::STRENGTH_SCALE) + ": " + to_string(rArmour3.getStrengthAdditionPercentage()) + "%";
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET3_ARMOUR_STRENGTH_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		buf = l.getMessage(Localized::DEXTERITY_SCALE) + ": " + to_string(rArmour3.getDexterityAdditionPercentage()) + "%";
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET3_ARMOUR_DEXTERITY_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		if (rArmour3.getArmourType() == Armour::ArmourType::LIGHT)
			buf = l.getMessage(Localized::EVASION_CHANCE) + ": " + to_string(rArmour3.getEvasionProbAddition()) + "%";
		else
			buf = l.getMessage(Localized::STUN_RESISTANCE_CHANCE) + ": " + to_string(rArmour3.getStunProbSubtraction()) + "%";
		SendMessage(hSubMenuItems[EQUIPMENT_STAT_SET3_ARMOUR_ABILITY], WM_SETTEXT, 0, (LPARAM)buf.c_str());
		}
	}
	break;

	case Game::Background::MAIN_MENU_SETTINGS:
	{
		if (changedSettings)
			ShowWindow(hSubItems[SETTINGS_BUT_APPLY_CHANGES], SW_SHOW);
		else
			ShowWindow(hSubItems[SETTINGS_BUT_APPLY_CHANGES], SW_HIDE);
	}
	break;

	case Game::Background::MAIN_MENU_SPECIALS: break;
	}
	game.backgroundChangeCompleted();
}

void MainMenu::resizeMenu(int cx, int cy)
{
	// If background is the same - no change needed
	if (!game.isBackgroundChanged())
		return;

	int sz, x, y, i;

	switch (game.getBackground())
	{
	default:case Game::Background::MAIN_MENU:
	{
		const int ITEM_HEIGHT = 45, DISTANCE = 10, ITEM_WIDTH = 300;
		sz = hItems.size();
		x = cx - ITEM_WIDTH / 2;
		y = cy - sz / 2 * (ITEM_HEIGHT + DISTANCE);
		for (HWND hItem : hItems)
		{
			y += ITEM_HEIGHT + DISTANCE;
			MoveWindow(hItem, x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		}
	}
	break;

	case Game::Background::MAIN_MENU_LOAD:
	{
		const int ITEM_WIDTH = 300, ITEM_HEIGHT = 30, DISTANCE = 10;

		MoveWindow(hSubItems[LOADING_STAT_LOAD], cx - ITEM_WIDTH / 2, DISTANCE * 2, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hSubItems[LOADING_BUT_BACK], cx - ITEM_WIDTH - DISTANCE, 670, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hSubItems[LOADING_BUT_LOAD], cx + DISTANCE, 670, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hSubItems[LOADING_LISTBOX_SAVES], cx - 250, DISTANCE * 4 + ITEM_HEIGHT, 500, 580, TRUE);
	}
	break;

	case Game::Background::MAIN_MENU_NEW_GAME:
	{
		const int STAT_WIDTH = 125, EDIT_WIDTH = 40, BUT_WIDTH = 35, ITEM_HEIGHT = 25, DISTANCE = 5;
		cy -= 2 * (ITEM_HEIGHT + DISTANCE);
		y = cy;

		// Resizing static windows
		MoveWindow(hSubItems[STAT_CHARACTER_CREATION], cx - 150, 40, 300, 55, TRUE);
		for (i = NewGameItem::STAT_NAME; i <= NewGameItem::STAT_CHARISMA; i++)
		{
			if (i != NewGameItem::STAT_UNNASSIGNED_ATTRIBUTES)
				MoveWindow(hSubItems[i], 557, y, STAT_WIDTH, ITEM_HEIGHT, TRUE);
			else
			{
				MoveWindow(hSubItems[i], 557, y + ITEM_HEIGHT + DISTANCE * 3, STAT_WIDTH + 70, 30, TRUE);
				y += ITEM_HEIGHT * 2 + DISTANCE;
			}
			y += ITEM_HEIGHT + DISTANCE;
		}

		// Resizing edit windows
		MoveWindow(hSubItems[EDIT_NAME], 687, cy, 116, ITEM_HEIGHT, TRUE);
		y = cy;
		for (i = NewGameItem::EDIT_AGE; i <= NewGameItem::EDIT_CHARISMA; i++)
		{
			y += ITEM_HEIGHT + DISTANCE;
			if (i != NewGameItem::EDIT_UNNASSIGNED_ATTRIBUTES)
				MoveWindow(hSubItems[i], 725, y, EDIT_WIDTH, ITEM_HEIGHT, TRUE);
			else
			{
				MoveWindow(hSubItems[i], 755, y + ITEM_HEIGHT + DISTANCE * 4, EDIT_WIDTH, ITEM_HEIGHT, TRUE);
				y += ITEM_HEIGHT * 2 + DISTANCE;
			}
		}

		// Resizing button windows
		y = cy + ITEM_HEIGHT + DISTANCE;
		for (i = NewGameItem::BUT_AGE_MINUS; i <= NewGameItem::BUT_CHARISMA_MINUS; i++)
		{
			if (i == NewGameItem::BUT_STRENGTH_MINUS)
				y += ITEM_HEIGHT * 3 + DISTANCE * 2;
			MoveWindow(hSubItems[i], 687, y, BUT_WIDTH, ITEM_HEIGHT, TRUE);
			MoveWindow(hSubItems[i + 7], 768, y, BUT_WIDTH, ITEM_HEIGHT, TRUE);
			y += ITEM_HEIGHT + DISTANCE;
		}

		// Resizing menu navigation button windows
		y += ITEM_HEIGHT + DISTANCE;
		MoveWindow(hSubItems[NEW_GAME_BUT_BACK], 557, y, 120, ITEM_HEIGHT, TRUE);

		MoveWindow(hSubItems[NEW_GAME_BUT_NEXT], 687, y, STAT_WIDTH, ITEM_HEIGHT, TRUE);
	}
	break;

	case Game::Background::MAIN_MENU_NEW_GAME_EQUIPMENT:
	{
		const int BIG_STAT_WIDTH = 250, BIG_STAT_HEIGHT = 30,
			SMALL_STAT_WIDTH = 180, SMALL_STAT_HEIGHT = 20,
			BIG_DISTANCE = 9, SMALL_DISTANCE = 4,
			BUT_SIZE = 20,
			ITEM_WIDTH = 120, ITEM_HEIGHT = 97;

		MoveWindow(hSubMenuItems[EQUIPMENT_STAT_CHOOSE], cx - 150, BIG_DISTANCE, 300, 40, TRUE);
		// Set 1
		x = cx - (SMALL_STAT_WIDTH + BIG_DISTANCE) * 3 - BIG_DISTANCE, y = BIG_STAT_HEIGHT * 7;
		MoveWindow(hSubMenuItems[EQUIPMENT_STAT_SET1], x + (BIG_DISTANCE + SMALL_STAT_WIDTH) / 2, y - (BIG_DISTANCE + SMALL_STAT_HEIGHT) * 5, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
		// Right hand
		MoveWindow(hSubMenuItems[EQUIPMENT_STAT_SET1_RIGHT_HAND], x, y - (BIG_DISTANCE + SMALL_STAT_HEIGHT) * 4, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
		MoveWindow(hSubMenuItems[EQUIPMENT_BUT_SET1_RIGHT_HAND], x + ITEM_WIDTH / 4, y - (BIG_DISTANCE + SMALL_STAT_HEIGHT) * 3, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		y += BIG_DISTANCE * 2;
		for (i = EQUIPMENT_STAT_SET1_RIGHT_HAND_TYPE; i <= EQUIPMENT_STAT_SET1_RIGHT_HAND_DEXTERITY_SCALE; i++)
		{
			MoveWindow(hSubMenuItems[i], x, y, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
			y += SMALL_STAT_HEIGHT + BIG_DISTANCE;
		}

		// Left hand
		x += SMALL_STAT_WIDTH + BIG_DISTANCE, y = BIG_STAT_HEIGHT * 7;
		MoveWindow(hSubMenuItems[EQUIPMENT_STAT_SET1_LEFT_HAND], x, y - (BIG_DISTANCE + SMALL_STAT_HEIGHT) * 4, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
		MoveWindow(hSubMenuItems[EQUIPMENT_BUT_SET1_LEFT_HAND], x + ITEM_WIDTH / 4, y - (BIG_DISTANCE + SMALL_STAT_HEIGHT) * 3, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		y += BIG_DISTANCE * 2;
		for (i = EQUIPMENT_STAT_SET1_LEFT_HAND_TYPE; i <= EQUIPMENT_STAT_SET1_LEFT_HAND_DEXTERITY_SCALE; i++)
		{
			MoveWindow(hSubMenuItems[i], x, y, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
			// Shield stats
			if (i == EQUIPMENT_STAT_SET1_LEFT_HAND_DAMAGE)
				MoveWindow(hSubMenuItems[EQUIPMENT_STAT_SET1_LEFT_HAND_SHIELD_DEFENSE], x, y, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
			if (i == EQUIPMENT_STAT_SET1_LEFT_HAND_STRENGTH_SCALE)
				MoveWindow(hSubMenuItems[EQUIPMENT_STAT_SET1_LEFT_HAND_SHIELD_BLOCK_CHANCE], x, y, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
			y += SMALL_STAT_HEIGHT + BIG_DISTANCE;
		}

		// Armour
		x -= (SMALL_STAT_WIDTH + BIG_DISTANCE) / 2, y += SMALL_STAT_HEIGHT * 2 + BIG_DISTANCE;
		MoveWindow(hSubMenuItems[EQUIPMENT_STAT_SET1_ARMOUR], x, y - BIG_DISTANCE * 2 - SMALL_STAT_HEIGHT, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
		y += ITEM_HEIGHT;
		MoveWindow(hSubMenuItems[EQUIPMENT_BUT_SET1_ARMOUR], x + ITEM_WIDTH / 4, y - SMALL_DISTANCE * 4 - BIG_STAT_HEIGHT * 3, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		for (i = EQUIPMENT_STAT_SET1_ARMOUR_TYPE; i <= EQUIPMENT_STAT_SET1_ARMOUR_ABILITY; i++)
		{
			MoveWindow(hSubMenuItems[i], x - 10, y, SMALL_STAT_WIDTH + 20, SMALL_STAT_HEIGHT, TRUE);
			y += SMALL_STAT_HEIGHT + BIG_DISTANCE;
		}
		MoveWindow(hSubMenuItems[EQUIPMENT_BUT_SET1], x + SMALL_STAT_WIDTH / 4, y + SMALL_DISTANCE, SMALL_STAT_WIDTH / 2, BIG_STAT_HEIGHT, TRUE);

		// Set 2
		x = cx - SMALL_STAT_WIDTH - BIG_DISTANCE / 2, y = BIG_STAT_HEIGHT * 7;
		MoveWindow(hSubMenuItems[EQUIPMENT_STAT_SET2], x + (BIG_DISTANCE + SMALL_STAT_WIDTH) / 2, y - (BIG_DISTANCE + SMALL_STAT_HEIGHT) * 5, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
		// Right hand
		MoveWindow(hSubMenuItems[EQUIPMENT_STAT_SET2_RIGHT_HAND], x, y - (BIG_DISTANCE + SMALL_STAT_HEIGHT) * 4, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
		MoveWindow(hSubMenuItems[EQUIPMENT_BUT_SET2_RIGHT_HAND], x + ITEM_WIDTH / 4, y - (BIG_DISTANCE + SMALL_STAT_HEIGHT) * 3, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		y += BIG_DISTANCE * 2;
		for (i = EQUIPMENT_STAT_SET2_RIGHT_HAND_TYPE; i <= EQUIPMENT_STAT_SET2_RIGHT_HAND_DEXTERITY_SCALE; i++)
		{
			MoveWindow(hSubMenuItems[i], x, y, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
			y += SMALL_STAT_HEIGHT + BIG_DISTANCE;
		}

		// Left hand
		x += SMALL_STAT_WIDTH + BIG_DISTANCE, y = BIG_STAT_HEIGHT * 7;
		MoveWindow(hSubMenuItems[EQUIPMENT_STAT_SET2_LEFT_HAND], x, y - (BIG_DISTANCE + SMALL_STAT_HEIGHT) * 4, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
		MoveWindow(hSubMenuItems[EQUIPMENT_BUT_SET2_LEFT_HAND], x + ITEM_WIDTH / 4, y - (BIG_DISTANCE + SMALL_STAT_HEIGHT) * 3, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		y += BIG_DISTANCE * 2;
		for (i = EQUIPMENT_STAT_SET2_LEFT_HAND_TYPE; i <= EQUIPMENT_STAT_SET2_LEFT_HAND_DEXTERITY_SCALE; i++)
		{
			MoveWindow(hSubMenuItems[i], x, y, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
			// Shield stats
			if (i == EQUIPMENT_STAT_SET2_LEFT_HAND_DAMAGE)
				MoveWindow(hSubMenuItems[EQUIPMENT_STAT_SET2_LEFT_HAND_SHIELD_DEFENSE], x, y, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
			if (i == EQUIPMENT_STAT_SET2_LEFT_HAND_STRENGTH_SCALE)
				MoveWindow(hSubMenuItems[EQUIPMENT_STAT_SET2_LEFT_HAND_SHIELD_BLOCK_CHANCE], x, y, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
			y += SMALL_STAT_HEIGHT + BIG_DISTANCE;
		}

		// Armour
		x -= (SMALL_STAT_WIDTH + BIG_DISTANCE) / 2, y += SMALL_STAT_HEIGHT * 2 + BIG_DISTANCE;
		MoveWindow(hSubMenuItems[EQUIPMENT_STAT_SET2_ARMOUR], x, y - BIG_DISTANCE * 2 - SMALL_STAT_HEIGHT, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
		y += ITEM_HEIGHT;
		MoveWindow(hSubMenuItems[EQUIPMENT_BUT_SET2_ARMOUR], x + ITEM_WIDTH / 4, y - SMALL_DISTANCE * 4 - BIG_STAT_HEIGHT * 3, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		for (i = EQUIPMENT_STAT_SET2_ARMOUR_TYPE; i <= EQUIPMENT_STAT_SET2_ARMOUR_ABILITY; i++)
		{
			MoveWindow(hSubMenuItems[i], x - 10, y, SMALL_STAT_WIDTH + 20, SMALL_STAT_HEIGHT, TRUE);
			y += SMALL_STAT_HEIGHT + BIG_DISTANCE;
		}
		MoveWindow(hSubMenuItems[EQUIPMENT_BUT_SET2], x + SMALL_STAT_WIDTH / 4, y + SMALL_DISTANCE, SMALL_STAT_WIDTH / 2, BIG_STAT_HEIGHT, TRUE);

		// Set 3
		x = cx + SMALL_STAT_WIDTH + BIG_DISTANCE * 3, y = BIG_STAT_HEIGHT * 7;
		MoveWindow(hSubMenuItems[EQUIPMENT_STAT_SET3], x + (BIG_DISTANCE + SMALL_STAT_WIDTH) / 2, y - (BIG_DISTANCE + SMALL_STAT_HEIGHT) * 5, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
		// Right hand
		MoveWindow(hSubMenuItems[EQUIPMENT_STAT_SET3_RIGHT_HAND], x, y - (BIG_DISTANCE + SMALL_STAT_HEIGHT) * 4, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
		MoveWindow(hSubMenuItems[EQUIPMENT_BUT_SET3_RIGHT_HAND], x + ITEM_WIDTH / 4, y - (BIG_DISTANCE + SMALL_STAT_HEIGHT) * 3, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		y += BIG_DISTANCE * 2;
		for (i = EQUIPMENT_STAT_SET3_RIGHT_HAND_TYPE; i <= EQUIPMENT_STAT_SET3_RIGHT_HAND_DEXTERITY_SCALE; i++)
		{
			MoveWindow(hSubMenuItems[i], x, y, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
			y += SMALL_STAT_HEIGHT + BIG_DISTANCE;
		}

		// Left hand
		x += SMALL_STAT_WIDTH + BIG_DISTANCE, y = BIG_STAT_HEIGHT * 7;
		MoveWindow(hSubMenuItems[EQUIPMENT_STAT_SET3_LEFT_HAND], x, y - (BIG_DISTANCE + SMALL_STAT_HEIGHT) * 4, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
		MoveWindow(hSubMenuItems[EQUIPMENT_BUT_SET3_LEFT_HAND], x + ITEM_WIDTH / 4, y - (BIG_DISTANCE + SMALL_STAT_HEIGHT) * 3, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		y += BIG_DISTANCE * 2;
		for (i = EQUIPMENT_STAT_SET3_LEFT_HAND_TYPE; i <= EQUIPMENT_STAT_SET3_LEFT_HAND_DEXTERITY_SCALE; i++)
		{
			MoveWindow(hSubMenuItems[i], x, y, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
			// Shield stats
			if (i == EQUIPMENT_STAT_SET3_LEFT_HAND_DAMAGE)
				MoveWindow(hSubMenuItems[EQUIPMENT_STAT_SET3_LEFT_HAND_SHIELD_DEFENSE], x, y, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
			if (i == EQUIPMENT_STAT_SET3_LEFT_HAND_STRENGTH_SCALE)
				MoveWindow(hSubMenuItems[EQUIPMENT_STAT_SET3_LEFT_HAND_SHIELD_BLOCK_CHANCE], x, y, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
			y += SMALL_STAT_HEIGHT + BIG_DISTANCE;
		}

		// Armour
		x -= (SMALL_STAT_WIDTH + BIG_DISTANCE) / 2, y += SMALL_STAT_HEIGHT * 2 + BIG_DISTANCE;
		MoveWindow(hSubMenuItems[EQUIPMENT_STAT_SET3_ARMOUR], x, y - BIG_DISTANCE * 2 - SMALL_STAT_HEIGHT, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
		y += ITEM_HEIGHT;
		MoveWindow(hSubMenuItems[EQUIPMENT_BUT_SET3_ARMOUR], x + ITEM_WIDTH / 4, y - SMALL_DISTANCE * 4 - BIG_STAT_HEIGHT * 3, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		for (i = EQUIPMENT_STAT_SET3_ARMOUR_TYPE; i <= EQUIPMENT_STAT_SET3_ARMOUR_ABILITY; i++)
		{
			MoveWindow(hSubMenuItems[i], x - 10, y, SMALL_STAT_WIDTH + 20, SMALL_STAT_HEIGHT, TRUE);
			y += SMALL_STAT_HEIGHT + BIG_DISTANCE;
		}
		MoveWindow(hSubMenuItems[EQUIPMENT_BUT_SET3], x + SMALL_STAT_WIDTH / 4, y + SMALL_DISTANCE, SMALL_STAT_WIDTH / 2, BIG_STAT_HEIGHT, TRUE);

		// Back
		y = 680;
		MoveWindow(hSubMenuItems[EQUIPMENT_BUT_BACK], cx - BIG_STAT_WIDTH - BIG_DISTANCE, y, BIG_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
		// Next
		MoveWindow(hSubMenuItems[EQUIPMENT_BUT_NEXT], cx + BIG_DISTANCE, y, BIG_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
	}
	break;

	case Game::Background::MAIN_MENU_SETTINGS:
	{
		const int ITEM_HEIGHT = 45, DISTANCE = 10, ITEM_WIDTH = 300, BUT_SIZE = 45;
		sz = hItems.size();
		x = cx - ITEM_WIDTH / 2;
		y = cy - sz / 2 * (ITEM_HEIGHT + DISTANCE);

		// Video settings (in progress)
		MoveWindow(hSubItems[SETTINGS_STAT_VIDEO], x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		y += ITEM_HEIGHT + DISTANCE;


		// Sound settings
		MoveWindow(hSubItems[SETTINGS_STAT_SOUND], x, y, ITEM_WIDTH - BUT_SIZE - DISTANCE, ITEM_HEIGHT, TRUE);

		// Sound checkbox
		MoveWindow(hSubItems[SETTINGS_BUT_SOUND], x + ITEM_WIDTH - BUT_SIZE, y, BUT_SIZE, BUT_SIZE, TRUE);
		y += ITEM_HEIGHT + DISTANCE;


		// Autosave settings
		MoveWindow(hSubItems[SETTINGS_STAT_AUTOSAVE], x, y, ITEM_WIDTH - BUT_SIZE - DISTANCE, ITEM_HEIGHT, TRUE);

		// Autosave checkbox
		MoveWindow(hSubItems[SETTINGS_BUT_AUTOSAVE], x + ITEM_WIDTH - BUT_SIZE, y, BUT_SIZE, BUT_SIZE, TRUE);
		y += ITEM_HEIGHT + DISTANCE;
		

		// Language settings
		MoveWindow(hSubItems[SETTINGS_STAT_LANGUAGE], x, y, (ITEM_WIDTH - DISTANCE) / 2, ITEM_HEIGHT, TRUE);

		MoveWindow(hSubItems[SETTINGS_COMBOBOX_LANGUAGE], x + ITEM_WIDTH / 2 + DISTANCE, y, ITEM_WIDTH / 2 - DISTANCE, ITEM_HEIGHT, TRUE);
		y += ITEM_HEIGHT + DISTANCE;


		// Back and apply changes
		MoveWindow(hSubItems[SETTINGS_BUT_BACK], x, y, (ITEM_WIDTH - DISTANCE) / 2, ITEM_HEIGHT, TRUE);

		MoveWindow(hSubItems[SETTINGS_BUT_APPLY_CHANGES], x + ITEM_WIDTH / 2 + DISTANCE, y, ITEM_WIDTH / 2 - DISTANCE, ITEM_HEIGHT, TRUE);
	}
	break;

	case Game::Background::MAIN_MENU_SPECIALS:
	{
		const int DISTANCE = 100, ITEM_WIDTH = 300, ITEM_HEIGHT = 45;

		MoveWindow(hSubItems[SPECIALS_STAT_SPECIALS], cx - ITEM_WIDTH / 2, DISTANCE - (int)(ITEM_HEIGHT * 1.5), ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		MoveWindow(hSubItems[SPECIALS_STAT_TEXT], DISTANCE, DISTANCE, cx * 2 - DISTANCE * 2, cy * 2 - DISTANCE * 3, TRUE);

		MoveWindow(hSubItems[SPECIALS_BUT_BACK], cx - ITEM_WIDTH / 2, cy * 2 - DISTANCE, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
	}
	break;
	}
}

void MainMenu::handleInput(HWND hWnd, UINT m, WPARAM wp, LPARAM lp)
{
	RECT windowRect;
	GetWindowRect(hWnd, &windowRect);
	int i;

	switch (m)
	{
	case WM_COMMAND:
	{
		switch (game.getBackground())
		{
		case Game::Background::MAIN_MENU:
		{
			if ((HWND)lp == hItems[BUT_CONTINUE])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Get every available save
				const string PATH = "Saves/";
				vector<string> folderNames;
				getFoldersInDirectory(PATH, folderNames);

				// Choose last created save
				string path = folderNames[folderNames.size() - 1] + "/";

				game.setWorldMap(WorldMap(hWnd));
				game.setFighting(Fighting(hWnd));
				// Load game
				game.loadFromFile(path);

				// Destroying all buttons
				for (HWND hItem : hSubItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubItems.clear();


				game.getMenuManager().setMenu(new CityMenu(hWnd));
				// Go to city menu
				game.setBackground(Game::Background::CITY_MENU);

				updateWindow(hWnd);
				break;
			}
			if ((HWND)lp == hItems[BUT_LOAD_GAME])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Hiding all buttons
				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_HIDE);

				// Erasing previous sub menu items
				for (HWND hItem : hSubItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubItems.clear();

				// Get every available save for display
				const string PATH = "Saves/";
				vector<string> folderNames;
				getFoldersInDirectory(PATH, folderNames);

				// Creating new sub menu items
				hSubItems.resize(LOADING_ITEM_NUMBER);

				hSubItems[LOADING_STAT_LOAD] = CreateWindow("STATIC", l.getMessage(Localized::CHOOSE_SAVE).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[LOADING_BUT_LOAD] = CreateWindow("BUTTON", l.getMessage(Localized::LOAD_GAME).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[LOADING_BUT_BACK] = CreateWindow("BUTTON", l.getMessage(Localized::BACK).c_str(), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[LOADING_LISTBOX_SAVES] = CreateWindow("LISTBOX", l.getMessage(Localized::BACK).c_str(), WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY | LBS_OWNERDRAWFIXED | LBS_HASSTRINGS, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				// Add strings with saves in reverse order to display chronologicaly from newest to oldest
				for (int i = folderNames.size() - 1; i >= 0; i--)
					SendMessage(hSubItems[LOADING_LISTBOX_SAVES], LB_ADDSTRING, 0, (LPARAM)formatSaveName(folderNames[i]).c_str());

				game.setBackground(Game::Background::MAIN_MENU_LOAD);

				updateWindow(hWnd);
			}
			if ((HWND)lp == hItems[BUT_NEW_GAME])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				nms = make_unique<NewMenuStorage>();
				game.setProgressionStage(Game::Progression::START);

				// Hiding all buttons
				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_HIDE);

				// Erasing previous sub menu items
				for (HWND hItem : hSubItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubItems.clear();

				// Creating new sub menu items
				hSubItems.resize(NewGameItem::NEW_GAME_ITEM_NUMBER);

				hSubItems[STAT_CHARACTER_CREATION] = CreateWindow("STATIC", l.getMessage(Localized::CHARACTER_CREATION).c_str(), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[STAT_NAME] = CreateWindow("STATIC", l.getMessage(Localized::NAME).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[STAT_AGE] = CreateWindow("STATIC", l.getMessage(Localized::AGE).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[STAT_UNNASSIGNED_ATTRIBUTES] = CreateWindow("STATIC", l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[STAT_STRENGTH] = CreateWindow("STATIC", l.getMessage(Localized::STRENGTH).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[STAT_CONSTITUTION] = CreateWindow("STATIC", l.getMessage(Localized::CONSTITUTION).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[STAT_DEXTERITY] = CreateWindow("STATIC", l.getMessage(Localized::DEXTERITY).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[STAT_INTELLIGENCE] = CreateWindow("STATIC", l.getMessage(Localized::INTELLIGENCE).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[STAT_WISDOM] = CreateWindow("STATIC", l.getMessage(Localized::WISDOM).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[STAT_CHARISMA] = CreateWindow("STATIC", l.getMessage(Localized::CHARISMA).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				hSubItems[EDIT_NAME] = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[EDIT_AGE] = CreateWindow("EDIT", to_string(nms->age).c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_CENTER, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[EDIT_UNNASSIGNED_ATTRIBUTES] = CreateWindow("EDIT", to_string(nms->unnassignedAttributes).c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_CENTER, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[EDIT_STRENGTH] = CreateWindow("EDIT", to_string(nms->strength).c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_CENTER, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[EDIT_CONSTITUTION] = CreateWindow("EDIT", to_string(nms->constitution).c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_CENTER, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[EDIT_DEXTERITY] = CreateWindow("EDIT", to_string(nms->dexterity).c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_CENTER, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[EDIT_INTELLIGENCE] = CreateWindow("EDIT", to_string(nms->intelligence).c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_CENTER, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[EDIT_WISDOM] = CreateWindow("EDIT", to_string(nms->wisdom).c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_CENTER, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[EDIT_CHARISMA] = CreateWindow("EDIT", to_string(nms->charisma).c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_CENTER, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				for (i = EDIT_NAME; i <= EDIT_CHARISMA; i++)
					SendMessage(hSubItems[i], WM_SETFONT, (WPARAM)game.getFont(Game::FontSize::MEDIUM), TRUE);

				hSubItems[BUT_AGE_MINUS] = CreateWindow("BUTTON", "-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[BUT_STRENGTH_MINUS] = CreateWindow("BUTTON", "-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[BUT_CONSTITUTION_MINUS] = CreateWindow("BUTTON", "-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[BUT_DEXTERITY_MINUS] = CreateWindow("BUTTON", "-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[BUT_INTELLIGENCE_MINUS] = CreateWindow("BUTTON", "-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[BUT_WISDOM_MINUS] = CreateWindow("BUTTON", "-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[BUT_CHARISMA_MINUS] = CreateWindow("BUTTON", "-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				hSubItems[BUT_AGE_PLUS] = CreateWindow("BUTTON", "+", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[BUT_STRENGTH_PLUS] = CreateWindow("BUTTON", "+", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[BUT_CONSTITUTION_PLUS] = CreateWindow("BUTTON", "+", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[BUT_DEXTERITY_PLUS] = CreateWindow("BUTTON", "+", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[BUT_INTELLIGENCE_PLUS] = CreateWindow("BUTTON", "+", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[BUT_WISDOM_PLUS] = CreateWindow("BUTTON", "+", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[BUT_CHARISMA_PLUS] = CreateWindow("BUTTON", "+", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				hSubItems[NEW_GAME_BUT_BACK] = CreateWindow("BUTTON", l.getMessage(Localized::BACK).c_str(), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[NEW_GAME_BUT_NEXT] = CreateWindow("BUTTON", l.getMessage(Localized::NEXT).c_str(), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				game.setBackground(Game::Background::MAIN_MENU_NEW_GAME);

				SetFocus(hSubItems[EDIT_NAME]);

				// Display story
				game.getStoryScreen().displayScreen(hWnd, StoryScreen::Screen::WELCOME_SCREEN);

				updateWindow(hWnd);
			}
			if ((HWND)lp == hItems[BUT_SETTINGS])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Hiding all buttons
				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_HIDE);

				// Erasing previous sub menu items
				for (HWND hItem : hSubItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubItems.clear();

				// Creating new sub menu items
				hSubItems.resize(SettingsItem::SETTINGS_ITEM_NUMBER);

				hSubItems[SETTINGS_STAT_VIDEO] = CreateWindow("STATIC", l.getMessage(Localized::VIDEO_SETTINGS).c_str(), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[SETTINGS_STAT_SOUND] = CreateWindow("STATIC", l.getMessage(Localized::AUDIO_SETTINGS).c_str(), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[SETTINGS_STAT_AUTOSAVE] = CreateWindow("STATIC", l.getMessage(Localized::AUTOSAVE_SETTINGS).c_str(), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[SETTINGS_STAT_LANGUAGE] = CreateWindow("STATIC", l.getMessage(Localized::LANGUAGE_SETTINGS).c_str(), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[SETTINGS_COMBOBOX_LANGUAGE] = CreateWindow("COMBOBOX", "", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED | CBS_HASSTRINGS, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[SETTINGS_BUT_SOUND] = CreateWindow("BUTTON", "", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[SETTINGS_BUT_AUTOSAVE] = CreateWindow("BUTTON", "", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[SETTINGS_BUT_APPLY_CHANGES] = CreateWindow("BUTTON", l.getMessage(Localized::APPLY_CHANGES).c_str(), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[SETTINGS_BUT_BACK] = CreateWindow("BUTTON", l.getMessage(Localized::BACK).c_str(), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				// Fill combobox with languages
				SendMessage(hSubItems[SETTINGS_COMBOBOX_LANGUAGE], CB_ADDSTRING, 0, (LPARAM)"English");
				SendMessage(hSubItems[SETTINGS_COMBOBOX_LANGUAGE], CB_ADDSTRING, 0, (LPARAM)"Українська");
				SendMessage(hSubItems[SETTINGS_COMBOBOX_LANGUAGE], CB_ADDSTRING, 0, (LPARAM)"Русский");
				SendMessage(hSubItems[SETTINGS_COMBOBOX_LANGUAGE], CB_ADDSTRING, 0, (LPARAM)"Latinus");
				SendMessage(hSubItems[SETTINGS_COMBOBOX_LANGUAGE], CB_SETCURSEL, l.getLanguage(), 0);
				ShowWindow(hSubItems[SETTINGS_BUT_APPLY_CHANGES], SW_HIDE);

				// Check buttons according to settings
				SendMessage(hSubItems[SETTINGS_BUT_SOUND], BM_SETCHECK, 0, game.getSoundStatus());
				SendMessage(hSubItems[SETTINGS_BUT_AUTOSAVE], BM_SETCHECK, 0, game.getAutoSaveStatus());

				game.setBackground(Game::Background::MAIN_MENU_SETTINGS);

				updateWindow(hWnd);
			}
			if ((HWND)lp == hItems[BUT_SPECIALS])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Hiding all buttons
				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_HIDE);

				// Erasing previous sub menu items
				for (HWND hItem : hSubItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubItems.clear();

				// Creating new sub menu items
				hSubItems.resize(SettingsItem::SETTINGS_ITEM_NUMBER);

				hSubItems[SPECIALS_STAT_SPECIALS] = CreateWindow("STATIC", l.getMessage(Localized::SPECIALS).c_str(), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[SPECIALS_STAT_TEXT] = CreateWindow("STATIC", l.getMessage(Localized::SPECIALS_TEXT).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0); // TODO
				hSubItems[SPECIALS_BUT_BACK] = CreateWindow("BUTTON", l.getMessage(Localized::BACK).c_str(), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				game.setBackground(Game::Background::MAIN_MENU_SPECIALS);

				updateWindow(hWnd);
			}
			if ((HWND)lp == hItems[BUT_EXIT])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				DestroyWindow(hWnd);
			}
		}
		break;

		case Game::Background::MAIN_MENU_LOAD:
		{
			if (HIWORD(wp) == LBN_SELCHANGE)
			{
				if (selected != SendMessage(hSubItems[LOADING_LISTBOX_SAVES], LB_GETCURSEL, 0, 0))
				{
					selected = SendMessage(hSubItems[LOADING_LISTBOX_SAVES], LB_GETCURSEL, 0, 0);
					playSound(SoundEnum::SOUND_BUTTON_CLICK);
					ShowWindow(hSubItems[LOADING_BUT_LOAD], SW_SHOW);
				}
			}

			if ((HWND)lp == hSubItems[LOADING_BUT_BACK])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Destroying all buttons
				for (HWND hItem : hSubItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubItems.clear();

				// Showing main menu buttons
				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_SHOW);

				game.setBackground(Game::Background::MAIN_MENU);

				updateWindow(hWnd);
				break;
			}

			if ((HWND)lp == hSubItems[LOADING_BUT_LOAD])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Get selected save
				int index = SendMessage(hSubItems[LOADING_LISTBOX_SAVES], LB_GETCURSEL, 0, 0);

				// If save is unselected
				if (index == -1)
				{
					MessageBox(hWnd, l.getMessage(Localized::CHOOSE_SAVE).c_str(), l.getMessage(Localized::SAVE_UNSELECTED).c_str(), MB_OK);
					break;
				}

				// Get selected folder name
				const string PATH = "Saves/";
				vector<string> folderNames;
				getFoldersInDirectory(PATH, folderNames);
				string path = folderNames[folderNames.size() - 1 - index];
				path += '/';

				game.setWorldMap(WorldMap(hWnd));
				game.setFighting(Fighting(hWnd));
				// Load game
				game.loadFromFile(path);

				// Go to city menu
				// Destroying all buttons
				for (HWND hItem : hSubItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubItems.clear();

				game.setFighting(Fighting(hWnd));

				game.getMenuManager().setMenu(new CityMenu(hWnd));
				game.setBackground(Game::Background::CITY_MENU);

				updateWindow(hWnd);
				break;
			}
		}
		break;

		case Game::Background::MAIN_MENU_NEW_GAME:
		{
			// Changing attributes
			{
			// Age
			if ((HWND)lp == hSubItems[BUT_AGE_MINUS])
			{
				if (nms->age > MIN_AGE)
				{
					playSound(SoundEnum::SOUND_BUTTON_CLICK);
					nms->age--;
					SetWindowText(hSubItems[EDIT_AGE], to_string(nms->age).c_str());
				}
			}
			if ((HWND)lp == hSubItems[BUT_AGE_PLUS])
			{
				if (nms->age < MAX_AGE)
				{
					playSound(SoundEnum::SOUND_BUTTON_CLICK);
					nms->age++;
					SetWindowText(hSubItems[EDIT_AGE], to_string(nms->age).c_str());
				}
			}
			// Strength
			if ((HWND)lp == hSubItems[BUT_STRENGTH_MINUS])
			{
				if (nms->strength > MIN_STRENGTH)
				{
					playSound(SoundEnum::SOUND_BUTTON_CLICK);
					nms->strength--;
					nms->unnassignedAttributes++;
					SetWindowText(hSubItems[EDIT_STRENGTH], to_string(nms->strength).c_str());
					SetWindowText(hSubItems[EDIT_UNNASSIGNED_ATTRIBUTES], to_string(nms->unnassignedAttributes).c_str());
				}
			}
			if ((HWND)lp == hSubItems[BUT_STRENGTH_PLUS])
			{
				if (nms->unnassignedAttributes > 0)
				{
					playSound(SoundEnum::SOUND_BUTTON_CLICK);
					nms->strength++;
					nms->unnassignedAttributes--;
					SetWindowText(hSubItems[EDIT_STRENGTH], to_string(nms->strength).c_str());
					SetWindowText(hSubItems[EDIT_UNNASSIGNED_ATTRIBUTES], to_string(nms->unnassignedAttributes).c_str());
				}
			}
			// Constitution
			if ((HWND)lp == hSubItems[BUT_CONSTITUTION_MINUS])
			{
				if (nms->constitution > MIN_CONSTITUTION)
				{
					playSound(SoundEnum::SOUND_BUTTON_CLICK);
					nms->constitution--;
					nms->unnassignedAttributes++;
					SetWindowText(hSubItems[EDIT_CONSTITUTION], to_string(nms->constitution).c_str());
					SetWindowText(hSubItems[EDIT_UNNASSIGNED_ATTRIBUTES], to_string(nms->unnassignedAttributes).c_str());
				}
			}
			if ((HWND)lp == hSubItems[BUT_CONSTITUTION_PLUS])
			{
				if (nms->unnassignedAttributes > 0)
				{
					playSound(SoundEnum::SOUND_BUTTON_CLICK);
					nms->constitution++;
					nms->unnassignedAttributes--;
					SetWindowText(hSubItems[EDIT_CONSTITUTION], to_string(nms->constitution).c_str());
					SetWindowText(hSubItems[EDIT_UNNASSIGNED_ATTRIBUTES], to_string(nms->unnassignedAttributes).c_str());
				}
			}
			// Dexterity
			if ((HWND)lp == hSubItems[BUT_DEXTERITY_MINUS])
			{
				if (nms->dexterity > MIN_DEXTERITY)
				{
					playSound(SoundEnum::SOUND_BUTTON_CLICK);
					nms->dexterity--;
					nms->unnassignedAttributes++;
					SetWindowText(hSubItems[EDIT_DEXTERITY], to_string(nms->dexterity).c_str());
					SetWindowText(hSubItems[EDIT_UNNASSIGNED_ATTRIBUTES], to_string(nms->unnassignedAttributes).c_str());
				}
			}
			if ((HWND)lp == hSubItems[BUT_DEXTERITY_PLUS])
			{
				if (nms->unnassignedAttributes > 0)
				{
					playSound(SoundEnum::SOUND_BUTTON_CLICK);
					nms->dexterity++;
					nms->unnassignedAttributes--;
					SetWindowText(hSubItems[EDIT_DEXTERITY], to_string(nms->dexterity).c_str());
					SetWindowText(hSubItems[EDIT_UNNASSIGNED_ATTRIBUTES], to_string(nms->unnassignedAttributes).c_str());
				}
			}
			// Intelligence
			if ((HWND)lp == hSubItems[BUT_INTELLIGENCE_MINUS])
			{
				if (nms->intelligence > MIN_INTELLIGENCE)
				{
					playSound(SoundEnum::SOUND_BUTTON_CLICK);
					nms->intelligence--;
					nms->unnassignedAttributes++;
					SetWindowText(hSubItems[EDIT_INTELLIGENCE], to_string(nms->intelligence).c_str());
					SetWindowText(hSubItems[EDIT_UNNASSIGNED_ATTRIBUTES], to_string(nms->unnassignedAttributes).c_str());
				}
			}
			if ((HWND)lp == hSubItems[BUT_INTELLIGENCE_PLUS])
			{
				if (nms->unnassignedAttributes > 0)
				{
					playSound(SoundEnum::SOUND_BUTTON_CLICK);
					nms->intelligence++;
					nms->unnassignedAttributes--;
					SetWindowText(hSubItems[EDIT_INTELLIGENCE], to_string(nms->intelligence).c_str());
					SetWindowText(hSubItems[EDIT_UNNASSIGNED_ATTRIBUTES], to_string(nms->unnassignedAttributes).c_str());
				}
			}
			// Wisdom
			if ((HWND)lp == hSubItems[BUT_WISDOM_MINUS])
			{
				if (nms->wisdom > MIN_WISDOM)
				{
					playSound(SoundEnum::SOUND_BUTTON_CLICK);
					nms->wisdom--;
					nms->unnassignedAttributes++;
					SetWindowText(hSubItems[EDIT_WISDOM], to_string(nms->wisdom).c_str());
					SetWindowText(hSubItems[EDIT_UNNASSIGNED_ATTRIBUTES], to_string(nms->unnassignedAttributes).c_str());
				}
			}
			if ((HWND)lp == hSubItems[BUT_WISDOM_PLUS])
			{
				if (nms->unnassignedAttributes > 0)
				{
					playSound(SoundEnum::SOUND_BUTTON_CLICK);
					nms->wisdom++;
					nms->unnassignedAttributes--;
					SetWindowText(hSubItems[EDIT_WISDOM], to_string(nms->wisdom).c_str());
					SetWindowText(hSubItems[EDIT_UNNASSIGNED_ATTRIBUTES], to_string(nms->unnassignedAttributes).c_str());
				}
			}
			// Charisma
			if ((HWND)lp == hSubItems[BUT_CHARISMA_MINUS])
			{
				if (nms->charisma > MIN_CHARISMA)
				{
					playSound(SoundEnum::SOUND_BUTTON_CLICK);
					nms->charisma--;
					nms->unnassignedAttributes++;
					SetWindowText(hSubItems[EDIT_CHARISMA], to_string(nms->charisma).c_str());
					SetWindowText(hSubItems[EDIT_UNNASSIGNED_ATTRIBUTES], to_string(nms->unnassignedAttributes).c_str());
				}
			}
			if ((HWND)lp == hSubItems[BUT_CHARISMA_PLUS])
			{
				if (nms->unnassignedAttributes > 0)
				{
					playSound(SoundEnum::SOUND_BUTTON_CLICK);
					nms->charisma++;
					nms->unnassignedAttributes--;
					SetWindowText(hSubItems[EDIT_CHARISMA], to_string(nms->charisma).c_str());
					SetWindowText(hSubItems[EDIT_UNNASSIGNED_ATTRIBUTES], to_string(nms->unnassignedAttributes).c_str());
				}
			}
			}

			// Continue - choosing equipment
			if ((HWND)lp == hSubItems[NEW_GAME_BUT_NEXT])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Hiding previous windows
				for (HWND hItem : hSubItems)
					ShowWindow(hItem, SW_HIDE);

				// Erasing previous sub menu items
				hSubMenuItems.clear();

				// Creating new sub menu items
				hSubMenuItems.resize(EQUIPMENT_ITEM_NUMBER);

				// Creating static windows
				for (i = EQUIPMENT_STAT_CHOOSE; i <= EQUIPMENT_STAT_SET3_ARMOUR_ABILITY; i++)
					hSubMenuItems[i] = CreateWindow("STATIC", "", WS_VISIBLE | WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				// Creating button windows
				for (i = EQUIPMENT_BUT_SET1_RIGHT_HAND; i <= EQUIPMENT_BUT_NEXT; i++)
					hSubMenuItems[i] = CreateWindow("BUTTON", "", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				for (i = EQUIPMENT_BUT_SET1; i <= EQUIPMENT_BUT_SET3; i++)
					hSubMenuItems[i] = CreateWindow("BUTTON", "", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW | BS_AUTORADIOBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				auto& pLeftHand1 = se[0]->leftHand;
				auto& pLeftHand2 = se[1]->leftHand;
				auto& pLeftHand3 = se[2]->leftHand;

				// Set 1
				if (pLeftHand1)
				{
					if (pLeftHand1->getWeaponType() != Weapon::WeaponType::SHIELD)
					{
						ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET1_LEFT_HAND_SHIELD_DEFENSE], SW_HIDE);
						ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET1_LEFT_HAND_SHIELD_BLOCK_CHANCE], SW_HIDE);
					}
					else
					{
						ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET1_LEFT_HAND_DAMAGE], SW_HIDE);
						ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET1_LEFT_HAND_STRENGTH_SCALE], SW_HIDE);
						ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET1_LEFT_HAND_DEXTERITY_SCALE], SW_HIDE);
					}
				}
				else
				{
					ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET1_LEFT_HAND_DAMAGE], SW_HIDE);
					ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET1_LEFT_HAND_STRENGTH_SCALE], SW_HIDE);
					ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET1_LEFT_HAND_DEXTERITY_SCALE], SW_HIDE);
					ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET1_LEFT_HAND_SHIELD_DEFENSE], SW_HIDE);
					ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET1_LEFT_HAND_SHIELD_BLOCK_CHANCE], SW_HIDE);
				}
				// Set 2
				if (pLeftHand2)
				{
					if (pLeftHand2->getWeaponType() != Weapon::WeaponType::SHIELD)
					{
						ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET2_LEFT_HAND_SHIELD_DEFENSE], SW_HIDE);
						ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET2_LEFT_HAND_SHIELD_BLOCK_CHANCE], SW_HIDE);
					}
					else
					{
						ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET2_LEFT_HAND_DAMAGE], SW_HIDE);
						ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET2_LEFT_HAND_STRENGTH_SCALE], SW_HIDE);
						ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET2_LEFT_HAND_DEXTERITY_SCALE], SW_HIDE);
					}
				}
				else
				{
					ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET2_LEFT_HAND_DAMAGE], SW_HIDE);
					ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET2_LEFT_HAND_STRENGTH_SCALE], SW_HIDE);
					ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET2_LEFT_HAND_DEXTERITY_SCALE], SW_HIDE);
					ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET2_LEFT_HAND_SHIELD_DEFENSE], SW_HIDE);
					ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET2_LEFT_HAND_SHIELD_BLOCK_CHANCE], SW_HIDE);
				}
				// Set 3
				if (pLeftHand3)
				{
					if (pLeftHand3->getWeaponType() != Weapon::WeaponType::SHIELD)
					{
						ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET3_LEFT_HAND_SHIELD_DEFENSE], SW_HIDE);
						ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET3_LEFT_HAND_SHIELD_BLOCK_CHANCE], SW_HIDE);
					}
					else
					{
						ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET3_LEFT_HAND_DAMAGE], SW_HIDE);
						ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET3_LEFT_HAND_STRENGTH_SCALE], SW_HIDE);
						ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET3_LEFT_HAND_DEXTERITY_SCALE], SW_HIDE);
					}
				}
				else
				{
					ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET3_LEFT_HAND_DAMAGE], SW_HIDE);
					ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET3_LEFT_HAND_STRENGTH_SCALE], SW_HIDE);
					ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET3_LEFT_HAND_DEXTERITY_SCALE], SW_HIDE);
					ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET3_LEFT_HAND_SHIELD_DEFENSE], SW_HIDE);
					ShowWindow(hSubMenuItems[EQUIPMENT_STAT_SET3_LEFT_HAND_SHIELD_BLOCK_CHANCE], SW_HIDE);
				}

				// Update generated weapons' stats based on player's stats
				for (i = 0; i < 3; i++)
				{
					if (se[i]->rightHand)
					{
						se[i]->rightHand->update(nms->strength, nms->dexterity);
						se[i]->rightHand->calculatePrice(nms->charisma, true);
					}

					if (se[i]->leftHand)
					{
						se[i]->leftHand->update(nms->strength, nms->dexterity);
						se[i]->leftHand->calculatePrice(nms->charisma, true);
					}

					if (se[i]->armour)
					{
						se[i]->armour->update(nms->strength, nms->dexterity);
						se[i]->armour->calculatePrice(nms->charisma, true);
					}
				}

				game.setBackground(Game::Background::MAIN_MENU_NEW_GAME_EQUIPMENT);

				updateWindow(hWnd);
			}

			// Return to main menu
			if ((HWND)lp == hSubItems[NEW_GAME_BUT_BACK] || LOWORD(wp) == IDCANCEL)
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				nms.reset();

				// Destroying all buttons
				for (HWND hItem : hSubItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubItems.clear();

				// Showing main menu buttons
				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_SHOW);

				game.setBackground(Game::Background::MAIN_MENU);

				updateWindow(hWnd);
				break;
			}
		}
		break;

		case Game::Background::MAIN_MENU_NEW_GAME_EQUIPMENT:
		{
			// Set 1
			if ((HWND)lp == hSubMenuItems[EQUIPMENT_BUT_SET1])
			{
				int n = selected;
				selected = EQUIPMENT_BUT_SET1;
				if (n != selected)
					playSound(SoundEnum::SOUND_BUTTON_CLICK);
				if (n != -1)
				{
					SetWindowText(hSubMenuItems[n], l.getMessage(Localized::SELECT).c_str());
					updateWindow(hSubMenuItems[n]);
				}
				SetWindowText(hSubMenuItems[selected], l.getMessage(Localized::SELECTED).c_str());
				UpdateWindow(hSubMenuItems[selected]);
			}

			// Set 2
			if ((HWND)lp == hSubMenuItems[EQUIPMENT_BUT_SET2])
			{
				int n = selected;
				selected = EQUIPMENT_BUT_SET2;
				if (n != selected)
					playSound(SoundEnum::SOUND_BUTTON_CLICK);
				if (n != -1)
				{
					SetWindowText(hSubMenuItems[n], l.getMessage(Localized::SELECT).c_str());
					updateWindow(hSubMenuItems[n]);
				}
				SetWindowText(hSubMenuItems[selected], l.getMessage(Localized::SELECTED).c_str());
				UpdateWindow(hSubMenuItems[selected]);
			}

			// Set 3
			if ((HWND)lp == hSubMenuItems[EQUIPMENT_BUT_SET3])
			{
				int n = selected;
				selected = EQUIPMENT_BUT_SET3;
				if (n != selected)
					playSound(SoundEnum::SOUND_BUTTON_CLICK);
				if (n != -1)
				{
					SetWindowText(hSubMenuItems[n], l.getMessage(Localized::SELECT).c_str());
					updateWindow(hSubMenuItems[n]);
				}
				SetWindowText(hSubMenuItems[selected], l.getMessage(Localized::SELECTED).c_str());
				UpdateWindow(hSubMenuItems[selected]);
			}

			// Entering game
			if ((HWND)lp == hSubMenuItems[EQUIPMENT_BUT_NEXT] && selected != -1)
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Player creation
				if (SendMessage(hSubItems[EDIT_NAME], WM_GETTEXTLENGTH, 0, 0) > 0)
					SendMessage(hSubItems[EDIT_NAME], WM_GETTEXT, 256, (LPARAM)str);
				else
					strcpy_s(str, l.getMessage(Localized::GLADIATOR).c_str());

				game.setPlayer(
					Player(
						Fighter(
							Statistics(
								nms->strength,
								nms->constitution,
								nms->dexterity,
								nms->intelligence,
								nms->wisdom,
								nms->charisma,
								nms->age,
								MIN_FAME
							),
							BASIC_HP,
							BASIC_HP,
							make_unique<Inventory>(),
							move(se[selected - EQUIPMENT_BUT_SET1]->rightHand),
							move(se[selected - EQUIPMENT_BUT_SET1]->leftHand),
							move(se[selected - EQUIPMENT_BUT_SET1]->armour)
						),
						Leveling(
							MIN_LEVEL,
							EXPERIENCE_PER_LEVEL,
							nms->unnassignedAttributes
						),
						string(str)
					)
				);
				game.getPlayer().updateMaxHP();

				// Give player starting gold
				game.getPlayer().getInventory()->addItem(make_unique<Item>(Item(Item::ItemType::GOLD)), 20);

				nms.reset();

				// World creation
				unique_ptr<City> pCity;
				vector<City> cities;

				vector<unique_ptr<NPC>> gladiators;

				for (int i = 0; i < MAX_CITIES; i++)
				{
					int cityLevel, traderLevel;
					// Use predetermined parameters for different cities
					switch (i)
					{
					case 0: // Rome
						cityLevel = ROME_CITY_LEVEL;
						traderLevel = ROME_TRADER_LEVEL;
						break;
					case 1: // Naples
						cityLevel = NAPLES_CITY_LEVEL;
						traderLevel = NAPLES_TRADER_LEVEL;
						break;
					case 2: // Metaponto
						cityLevel = METAPONTO_CITY_LEVEL;
						traderLevel = METAPONTO_TRADER_LEVEL;
						break;
					case 3: // Bojano
						cityLevel = BOJANO_CITY_LEVEL;
						traderLevel = BOJANO_TRADER_LEVEL;
						break;
					case 4: // Ancona
						cityLevel = ANCONA_CITY_LEVEL;
						traderLevel = ANCONA_TRADER_LEVEL;
						break;
					case 5: // Perugia
						cityLevel = PERUGIA_CITY_LEVEL;
						traderLevel = PERUGIA_TRADER_LEVEL;
						break;
					case 6: // Florence
						cityLevel = FLORENCE_CITY_LEVEL;
						traderLevel = FLORENCE_TRADER_LEVEL;
						break;
					case 7: // Bologna
						cityLevel = BOLOGNA_CITY_LEVEL;
						traderLevel = BOLOGNA_TRADER_LEVEL;
						break;
					case 8: // Genoa
						cityLevel = GENOA_CITY_LEVEL;
						traderLevel = GENOA_TRADER_LEVEL;
						break;
					case 9: // Aquileia
						cityLevel = AQUILEIA_CITY_LEVEL;
						traderLevel = AQUILEIA_TRADER_LEVEL;
						break;
					case 10: // Milan
						cityLevel = MILAN_CITY_LEVEL;
						traderLevel = MILAN_TRADER_LEVEL;
						break;
					}

					// Creating opponents for arenas of different levels for different cities
					for (int j = 0; j < OPPONENTS_NUMBER; j++)
						gladiators.push_back(generateNPC(cityLevel));
					// Sort NPCs by level and fame
					sort(gladiators.begin(), gladiators.end(), compareNPC);

					// Creating cities
					pCity = make_unique<City>(Cities::ROME + i, Arena(gladiators), cityLevel, *generateTrader(traderLevel));
					cities.push_back(*pCity);
					gladiators.clear();
				}

				game.setWorldMap(WorldMap(hWnd, cities, Cities::PERUGIA));
				game.setFighting(Fighting(hWnd));

				game.getMenuManager().setMenu(new CityMenu(hWnd));

				// Display story
				game.getStoryScreen().displayScreen(hWnd, StoryScreen::Screen::START_GAME_SCREEN);

				// Display city
				game.setBackground(Game::Background::CITY_MENU);

				updateWindow(hWnd);
				break;
			}
			else if ((HWND)lp == hSubMenuItems[EQUIPMENT_BUT_NEXT] && selected == -1)
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				MessageBox(hWnd, l.getMessage(Localized::SELECT_EQUIPMENT).c_str(), l.getMessage(Localized::EQUIPMENT_UNSELECTED).c_str(), MB_OK);
			}

			// Back to character creation
			if ((HWND)lp == hSubMenuItems[EQUIPMENT_BUT_BACK] || LOWORD(wp) == IDCANCEL)
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Destroying all buttons
				for (HWND hItem : hSubMenuItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubMenuItems.clear();

				// Showing main menu buttons
				for (HWND hItem : hSubItems)
					ShowWindow(hItem, SW_SHOW);

				selected = -1;

				game.setBackground(Game::Background::MAIN_MENU_NEW_GAME);

				updateWindow(hWnd);
				break;
			}
		}
		break;

		case Game::Background::MAIN_MENU_SETTINGS:
		{
			if ((HWND)lp == hSubItems[SETTINGS_BUT_SOUND])
			{
				game.setSoundStatus(!game.getSoundStatus());
				playSound(SoundEnum::SOUND_BUTTON_CLICK);

				// Write data in file
				// Opening file
				string path = "Data/Settings.conf";
				// Saving settings
				ofstream fout(path, ios::binary);
				if (!fout)
					throw new exception("Error: Couldn't open file for game settings");
				fout << l.getLanguage() << " " << game.getSoundStatus() << " " << game.getAutoSaveStatus();
				fout.close();
			}

			if ((HWND)lp == hSubItems[SETTINGS_BUT_AUTOSAVE])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				game.setAutoSaveStatus(!game.getAutoSaveStatus());

				// Write data in file
				// Opening file
				string path = "Data/Settings.conf";
				// Saving settings
				ofstream fout(path, ios::binary);
				if (!fout)
					throw new exception("Error: Couldn't open file for game settings");
				fout << l.getLanguage() << " " << game.getSoundStatus() << " " << game.getAutoSaveStatus();
				fout.close();
			}

			if (HIWORD(wp) == CBN_SELCHANGE)
			{
				if (selectedLanguage != (Language)SendMessage(hSubItems[SETTINGS_COMBOBOX_LANGUAGE], CB_GETCURSEL, 0, 0))
				{
					selectedLanguage = (Language)SendMessage(hSubItems[SETTINGS_COMBOBOX_LANGUAGE], CB_GETCURSEL, 0, 0);
					changedSettings = true;
					playSound(SoundEnum::SOUND_BUTTON_CLICK);
					ShowWindow(hSubItems[SETTINGS_BUT_APPLY_CHANGES], SW_SHOW);
				}
			}

			if ((HWND)lp == hSubItems[SETTINGS_BUT_APPLY_CHANGES])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Update language
				l.setLanguage(selectedLanguage);
				SetWindowText(hItems[BUT_CONTINUE], l.getMessage(Localized::CONTINUE_GAME).c_str());
				SetWindowText(hItems[BUT_LOAD_GAME], l.getMessage(Localized::LOAD_GAME).c_str());
				SetWindowText(hItems[BUT_NEW_GAME], l.getMessage(Localized::NEW_GAME).c_str());
				SetWindowText(hItems[BUT_SETTINGS], l.getMessage(Localized::SETTINGS).c_str());
				SetWindowText(hItems[BUT_SPECIALS], l.getMessage(Localized::SPECIALS).c_str());
				SetWindowText(hItems[BUT_EXIT], l.getMessage(Localized::EXIT).c_str());

				SetWindowText(hSubItems[SETTINGS_STAT_VIDEO], l.getMessage(Localized::VIDEO_SETTINGS).c_str());
				SetWindowText(hSubItems[SETTINGS_STAT_SOUND], l.getMessage(Localized::AUDIO_SETTINGS).c_str());
				SetWindowText(hSubItems[SETTINGS_STAT_LANGUAGE], l.getMessage(Localized::LANGUAGE_SETTINGS).c_str());
				SetWindowText(hSubItems[SETTINGS_STAT_AUTOSAVE], l.getMessage(Localized::AUTOSAVE_SETTINGS).c_str());
				SetWindowText(hSubItems[SETTINGS_BUT_APPLY_CHANGES], l.getMessage(Localized::APPLY_CHANGES).c_str());
				SetWindowText(hSubItems[SETTINGS_BUT_BACK], l.getMessage(Localized::BACK).c_str());

				// Save changes
				changedSettings = false;
				ShowWindow(hSubItems[SETTINGS_BUT_APPLY_CHANGES], SW_HIDE);
				game.updateBackground();
			}

			if ((HWND)lp == hSubItems[SETTINGS_BUT_BACK] || LOWORD(wp) == IDCANCEL)
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				changedSettings = false;
				// Destroying all buttons
				for (HWND hItem : hSubItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubItems.clear();

				// Showing main menu buttons
				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_SHOW);

				game.setBackground(Game::Background::MAIN_MENU);

				updateWindow(hWnd);
				break;
			}
		}
		break;

		case Game::Background::MAIN_MENU_SPECIALS:
		{
			if ((HWND)lp == hSubItems[SPECIALS_BUT_BACK] || LOWORD(wp) == IDCANCEL)
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Destroying all buttons
				for (HWND hItem : hSubItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubItems.clear();

				// Showing main menu buttons
				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_SHOW);

				game.setBackground(Game::Background::MAIN_MENU);

				updateWindow(hWnd);
				break;
			}
		}
		break;
		}
	}
	break;

	//case WM_MOUSEMOVE:
	//{
	//	static int i;
	//	// First method
	//	RECT rect;
	//	POINT pt;
	//	// Get cursor position
	//	GetCursorPos(&pt);
	//	ScreenToClient(hWnd, &pt);

	//	// Test
	//	HDC hdc;
	//	PAINTSTRUCT ps;
	//	buf = to_string(pt.x) + " " + to_string(pt.y);
	//	for (HWND hItem : hItems)
	//		if ((HWND)lp == hItem)
	//			SetWindowText(hWnd, buf.c_str());

	//	// Second method
	//	// Get cursor position
	//	TRACKMOUSEEVENT tme;
	//	tme.cbSize = sizeof(TRACKMOUSEEVENT);
	//	tme.dwFlags = TME_HOVER | TME_LEAVE;
	//	tme.dwHoverTime = HOVER_DEFAULT;

	//	switch (game.getBackground())
	//	{
	//	case Game::Background::MAIN_MENU:
	//	{
	//		for (HWND hItem : hItems)
	//		{
	//			// Get button position
	//			GetWindowRect(hItem, &rect);
	//			MapWindowPoints(HWND_DESKTOP, hWnd, (LPPOINT)&rect, 2);
	//			//InvalidateRect(hWnd, &rect, 1);

	//			if (PtInRect(&rect, pt))
	//			{
	//				HBRUSH hBrush = CreateSolidBrush(COLOR_ROMAN_RED);
	//				SetWindowText(hItem, to_string(i).c_str());
	//				i++;
	//				hdc = BeginPaint(hWnd, &ps);
	//				DrawText(hdc, "Test", -1, &rect, DT_LEFT);
	//				FillRect(hdc, &rect, hBrush);
	//				EndPaint(hWnd, &ps);
	//				DeleteObject(hBrush);
	//			}
	//		}
	//	}
	//	break;

	//	case Game::Background::MAIN_MENU_LOAD:
	//	{
	//		// TODO
	//	}
	//	break;

	//	case Game::Background::MAIN_MENU_NEW_GAME:
	//	{
	//		// TODO
	//	}
	//	break;

	//	case Game::Background::MAIN_MENU_SETTINGS:
	//	{
	//		// TODO
	//	}
	//	break;

	//	case Game::Background::MAIN_MENU_SPECIALS:
	//	{
	//		// TODO
	//	}
	//	break;
	//	}
	//}
	//break;

	//case WM_MOUSEHOVER:
	//{
	//	SetWindowText(hWnd, "First stage succesful");
	//}
	//break;

	//case WM_MOUSELEAVE:
	//{
	//	SetWindowText(hWnd, "Second stage succesful");
	//}
	//break;
	}
}

bool MainMenu::stylizeWindow(HWND hWnd, UINT m, WPARAM wp, LPARAM lp, LRESULT& result)
{
	switch (m)
	{
		case WM_DRAWITEM:
		{
			LPDRAWITEMSTRUCT item = (LPDRAWITEMSTRUCT)lp;
			HDC hdc = item->hDC;

			GetClassName(item->hwndItem, str, sizeof(str) / sizeof(str[0]));

			SelectObject(hdc, game.getFont(Game::FontSize::MEDIUM));
			SetBkMode(hdc, TRANSPARENT);

			// Get text
			int len = GetWindowTextLength(item->hwndItem);
			buf.resize(len + 1); // Resize buffer to contain button text
			GetWindowTextA(item->hwndItem, &buf[0], len + 1); // Write text into buffer

			SetTextColor(hdc, COLOR_WHITE); // Set basic text color

			// Checking window type to draw it using correct styles
			if (game.getBackground() == Game::Background::MAIN_MENU_NEW_GAME && item->hwndItem == hSubItems[STAT_CHARACTER_CREATION])
			{
				SelectObject(hdc, game.getFont(Game::FontSize::LARGE));
				SetBkMode(hdc, TRANSPARENT);

				FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_DARK_BLUE)); // Fill background
				DrawTextA(item->hDC, buf.c_str(), len, &item->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER); // Display text
				DrawEdge(hdc, &item->rcItem, EDGE_SUNKEN, BF_RECT); // Draw edge
				return true;
			}
			if (game.getBackground() == Game::Background::MAIN_MENU_NEW_GAME_EQUIPMENT)
			{
				if (item->CtlType == ODT_STATIC)
				{
					// Select font size
					if (item->hwndItem == hSubMenuItems[EQUIPMENT_STAT_CHOOSE])
						SelectObject(hdc, game.getFont(Game::FontSize::LARGE));
					else
						SelectObject(hdc, game.getFont(Game::FontSize::SMALL));
					SetBkMode(hdc, TRANSPARENT);

					FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_DARK_BLUE)); // Fill background
					DrawTextA(item->hDC, buf.c_str(), len, &item->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER); // Display text
					DrawEdge(hdc, &item->rcItem, EDGE_SUNKEN, BF_RECT); // Draw edge
					return true;
				}

				// Check every item icon
				for (int i = EQUIPMENT_BUT_SET1_RIGHT_HAND; i <= EQUIPMENT_BUT_SET3_ARMOUR; i++)
				{
					if (item->hwndItem == hSubMenuItems[i])
					{
						if (item->itemState & ODS_SELECTED) // Pushed
							drawItem(hWnd, item, i, true);
						else // Not pushed
							drawItem(hWnd, item, i, false);
						return true;
					}
				}

				if (item->hwndItem == hSubMenuItems[EQUIPMENT_BUT_SET1] && selected != -1 && selected == EQUIPMENT_BUT_SET1)
				{
					FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_ROMAN_RED_PUSHED)); // Fill background
					DrawTextA(item->hDC, buf.c_str(), len, &item->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER); // Display text
					DrawEdge(hdc, &item->rcItem, EDGE_RAISED, BF_RECT); // Draw edge
					return true;
				}

				if (item->hwndItem == hSubMenuItems[EQUIPMENT_BUT_SET2] && selected != -1 && selected == EQUIPMENT_BUT_SET2)
				{
					FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_ROMAN_RED_PUSHED)); // Fill background
					DrawTextA(item->hDC, buf.c_str(), len, &item->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER); // Display text
					DrawEdge(hdc, &item->rcItem, EDGE_RAISED, BF_RECT); // Draw edge
					return true;
				}

				if (item->hwndItem == hSubMenuItems[EQUIPMENT_BUT_SET3] && selected != -1 && selected == EQUIPMENT_BUT_SET3)
				{
					FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_ROMAN_RED_PUSHED)); // Fill background
					DrawTextA(item->hDC, buf.c_str(), len, &item->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER); // Display text
					DrawEdge(hdc, &item->rcItem, EDGE_RAISED, BF_RECT); // Draw edge
					return true;
				}
			}
			if (game.getBackground() == Game::Background::MAIN_MENU_SETTINGS)
			{
				if (item->hwndItem == hSubItems[SETTINGS_BUT_SOUND])
				{
					if (game.getSoundStatus())
						FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_ROMAN_RED_PUSHED));
					else
						FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_ROMAN_RED));

					DrawEdge(hdc, &item->rcItem, EDGE_RAISED, BF_RECT);
					return true;
				}
				if (item->hwndItem == hSubItems[SETTINGS_BUT_AUTOSAVE])
				{
					if (game.getAutoSaveStatus())
						FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_ROMAN_RED_PUSHED));
					else
						FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_ROMAN_RED));

					DrawEdge(hdc, &item->rcItem, EDGE_RAISED, BF_RECT);
					return true;
				}
			}
			if (game.getBackground() == Game::Background::MAIN_MENU_SPECIALS)
			{
				if (item->hwndItem == hSubItems[SPECIALS_STAT_TEXT])
				{
					SelectObject(hdc, game.getFont(Game::FontSize::LARGE));
					SetBkMode(hdc, TRANSPARENT);

					FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_DARK_BLUE)); // Fill background

					// Text padding
					RECT rect = item->rcItem;
					int padding = 10;
					rect.left += padding;
					rect.top += padding;
					rect.right -= padding;
					rect.bottom -= padding;

					DrawTextA(item->hDC, buf.c_str(), len, &rect, DT_VCENTER | DT_LEFT | DT_WORDBREAK); // Display text
					DrawEdge(hdc, &item->rcItem, EDGE_SUNKEN, BF_RECT); // Draw edge

					return true;
				}
			}
			return false;
		}
		break;

		case WM_CTLCOLORSTATIC:
		{
			HDC hdc = (HDC)wp;
			SetTextColor(hdc, COLOR_WHITE);
			SetBkMode(hdc, TRANSPARENT);

			if (hBackgroundBrush != NULL)
				DeleteObject(hBackgroundBrush);
			hBackgroundBrush = CreateSolidBrush(COLOR_DARK_BLUE);

			result = (LRESULT)hBackgroundBrush;
			return true;
		}
		break;

		case WM_CTLCOLOREDIT:
		{
			HDC hdc = (HDC)wp;
			SetTextColor(hdc, COLOR_WHITE);
			SetBkMode(hdc, TRANSPARENT);

			if (hBackgroundBrush != NULL)
				DeleteObject(hBackgroundBrush);
			hBackgroundBrush = CreateSolidBrush(COLOR_DARK_BLUE);

			result = (LRESULT)hBackgroundBrush;
			return true;
		}
		break;

		case WM_CTLCOLORLISTBOX:
		{
			HDC hdc = (HDC)wp;
			SetTextColor(hdc, COLOR_WHITE);
			SetBkMode(hdc, TRANSPARENT);

			if (hBackgroundBrush != NULL)
				DeleteObject(hBackgroundBrush);
			hBackgroundBrush = CreateSolidBrush(COLOR_DARK_BLUE);

			result = (LRESULT)hBackgroundBrush;
			return true;
		}
		break;
	}
	return false;
}

void MainMenu::drawItem(HWND hWnd, LPDRAWITEMSTRUCT item, int buttonIndex, bool isPushed)
{
	// Selecting image for button based on item type
	const string DIRECTORY = "Data/Image/Items/",
		FORMAT = ".bmp";
	string path = "";

	// Getting item represented by button
	unique_ptr<Item> pItem;
	switch (buttonIndex)
	{
	case EQUIPMENT_BUT_SET1_RIGHT_HAND:
		pItem = make_unique<Weapon>(*se[0]->rightHand);
		break;
	case EQUIPMENT_BUT_SET1_LEFT_HAND:
		if (se[0]->leftHand)
			pItem = make_unique<Weapon>(*se[0]->leftHand);
		else
			pItem = nullptr;
		break;
	case EQUIPMENT_BUT_SET1_ARMOUR:
		pItem = make_unique<Armour>(*se[0]->armour);
		break;
	case EQUIPMENT_BUT_SET2_RIGHT_HAND:
		pItem = make_unique<Weapon>(*se[1]->rightHand);
		break;
	case EQUIPMENT_BUT_SET2_LEFT_HAND:
		if (se[1]->leftHand)
			pItem = make_unique<Weapon>(*se[1]->leftHand);
		else
			pItem = nullptr;
		break;
	case EQUIPMENT_BUT_SET2_ARMOUR:
		pItem = make_unique<Armour>(*se[1]->armour);
		break;
	case EQUIPMENT_BUT_SET3_RIGHT_HAND:
		pItem = make_unique<Weapon>(*se[2]->rightHand);
		break;
	case EQUIPMENT_BUT_SET3_LEFT_HAND:
		if (se[2]->leftHand)
			pItem = make_unique<Weapon>(*se[2]->leftHand);
		else
			pItem = nullptr;
		break;
	case EQUIPMENT_BUT_SET3_ARMOUR:
		pItem = make_unique<Armour>(*se[2]->armour);
		break;
	}

	if (pItem)
	{
		switch (pItem->getItemType())
		{
		case Item::ItemType::WEAPON:
			if (auto weapon = dynamic_cast<Weapon*>(pItem.get()))
			{
				switch (weapon->getWeaponType())
				{
				case Weapon::WeaponType::SWORD: path = DIRECTORY + "sword"; break;
				case Weapon::WeaponType::SPEAR: path = DIRECTORY + "spear"; break;
				case Weapon::WeaponType::DAGGER: path = DIRECTORY + "dagger"; break;
				case Weapon::WeaponType::AXE: path = DIRECTORY + "axe"; break;
				case Weapon::WeaponType::MACE: path = DIRECTORY + "mace"; break;
				case Weapon::WeaponType::SHIELD: path = DIRECTORY + "shield"; break;
				default: path = DIRECTORY + "error"; break;
				}
			}
			else
				path = DIRECTORY + "error";
			break;

		case Item::ItemType::ARMOUR:
			if (auto armour = dynamic_cast<Armour*>(pItem.get()))
			{
				switch (armour->getArmourType())
				{
				case Armour::ArmourType::LIGHT: path = DIRECTORY + "lightArmour"; break;
				case Armour::ArmourType::HEAVY: path = DIRECTORY + "heavyArmour"; break;
				default: path = DIRECTORY + "error"; break;
				}
			}
			else
				path = DIRECTORY + "error";
			break;

		case Item::ItemType::GOLD: path = DIRECTORY + "gold"; break;
		default: path = DIRECTORY + "error"; break;
		}
	}
	else
		path = DIRECTORY + "emptyHand";
	

	if (item->itemState & ODS_SELECTED || isPushed) // Pushed button
		path += "Pushed" + FORMAT;
	else // Unpushed button
		path += FORMAT;

	// Select image
	hBackgroundImage = (HBITMAP)LoadImage(0, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	// Filling background with selected image
	hBackgroundBrush = CreatePatternBrush(hBackgroundImage);
	FillRect(item->hDC, &item->rcItem, hBackgroundBrush);
	// Drawing edge
	DrawEdge(item->hDC, &item->rcItem, EDGE_RAISED, BF_RECT);
}
