#include "stdafx.h"
#include "CityMenu.h"

extern TCHAR str[256];
extern string buf;
extern string logStr;

extern HINSTANCE hInst;
extern Localization l;
extern Game game;

CityMenu::CityMenu() :
	hItems(MenuItem::ITEM_NUMBER),
	hSubItems(),
	hSubMenuItems(),
	selectedOpponent(-1),
	selectedItem(-1),
	hBackgroundImage(NULL),
	hBackgroundBrush(NULL),
	pas()
{ }

CityMenu::CityMenu(HWND hWnd) :
	hItems(MenuItem::ITEM_NUMBER),
	hSubItems(),
	selectedOpponent(-1),
	selectedItem(-1),
	hBackgroundImage(NULL),
	hBackgroundBrush(NULL),
	pas()
{
	// Getting the city name
	buf = l.getMessage(Localized::CITY) + " " + l.getCityName(game.getWorldMap().getCurrentCity());
	hItems[STAT_CITY_NAME] = CreateWindow("STATIC", buf.c_str(),
		WS_CHILD | WS_VISIBLE | SS_OWNERDRAW | SS_CENTER,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);

	hItems[EDIT_MESSAGES_LOG] = CreateWindow("EDIT", logStr.c_str(),
		WS_CHILD | WS_VISIBLE | ES_READONLY | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN | WS_BORDER,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);

	SendMessage(hItems[EDIT_MESSAGES_LOG], WM_SETFONT, (WPARAM)game.getFont(Game::FontSize::MEDIUM), TRUE);

	char className[256] = "BUTTON";
	hItems[BUT_ARENA] = CreateWindow(className, l.getMessage(Localized::ARENA).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);

	hItems[BUT_QUEST] = CreateWindow(className, l.getMessage(Localized::QUEST).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_MARKET] = CreateWindow(className, l.getMessage(Localized::MARKET).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_CHARACTER] = CreateWindow(className, l.getMessage(Localized::VIEW_CHARACTER).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_REST] = CreateWindow(className, l.getMessage(Localized::REST).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_MAP] = CreateWindow(className, l.getMessage(Localized::MAP).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_MENU] = CreateWindow(className, l.getMessage(Localized::MENU).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
}

CityMenu::CityMenu(const CityMenu& CM) :
	hSubItems(),
	hSubMenuItems(),
	selectedOpponent(-1),
	selectedItem(-1),
	pas()
{
	// Resizing items' vector
	int sz = CM.hItems.size();
	hItems.resize(sz);

	HWND hItem;
	RECT rect;
	char className[256], windowText[256];
	for (int i = 0; i < sz; i++)
	{
		// Obtaining the previous parameters
		hItem = CM.hItems[i];
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

	if (CM.hBackgroundImage != NULL)
	{
		BITMAP bm;
		GetObject(CM.hBackgroundImage, sizeof(BITMAP), &bm);
		hBackgroundImage = CreateBitmapIndirect(&bm);
	}
	else
		hBackgroundImage = NULL;

	if (CM.hBackgroundBrush != NULL)
	{
		LOGBRUSH lb;
		GetObject(CM.hBackgroundBrush, sizeof(LOGBRUSH), &lb);
		hBackgroundBrush = CreateBrushIndirect(&lb);
	}
	else
		hBackgroundBrush = NULL;
}

CityMenu& CityMenu::operator=(const CityMenu& CM)
{
	if (&CM == this) return *this;

	// Destroying the previous items of the menu
	for (HWND hItem : hItems)
		if (hItem != NULL)
			DestroyWindow(hItem);

	// Resizing items' vector
	int sz = CM.hItems.size();
	hItems.resize(sz);

	HWND hItem;
	RECT rect;
	char className[256], windowText[256];
	for (int i = 0; i < sz; i++)
	{
		// Obtaining the previous parameters
		hItem = CM.hItems[i];
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

	selectedOpponent = CM.selectedOpponent;
	selectedItem = CM.selectedItem;

	if (hBackgroundImage != NULL)
		DeleteObject(hBackgroundImage);

	if (CM.hBackgroundImage != NULL)
	{
		BITMAP bm;
		GetObject(CM.hBackgroundImage, sizeof(BITMAP), &bm);
		hBackgroundImage = CreateBitmapIndirect(&bm);
	}
	else
		hBackgroundImage = NULL;

	if (hBackgroundBrush != NULL)
		DeleteObject(hBackgroundBrush);

	if (CM.hBackgroundBrush != NULL)
	{
		LOGBRUSH lb;
		GetObject(CM.hBackgroundBrush, sizeof(LOGBRUSH), &lb);
		hBackgroundBrush = CreateBrushIndirect(&lb);
	}
	else
		hBackgroundBrush = NULL;

	pas = CM.pas;

	return *this;
}

CityMenu::~CityMenu()
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

void CityMenu::drawMenu(HWND hWnd, HDC hdc, int cx, int cy)
{
	const string DIRECTORY = "Data/Image/Background/";
	const string FORMAT = ".bmp";
	string path("");
	int i;

	RECT rect;
	GetClientRect(hWnd, &rect);

	// 1. Background
	if (game.isBackgroundChanged())
	{
		// Composing path based on current menu
		switch (game.getBackground())
		{
		default:case Game::Background::CITY_MENU:
			switch (game.getWorldMap().getCurrentCity().getNameIndex())
			{
			case ROME: path = DIRECTORY + "Cities/RomeBackground" + FORMAT; break;
			case NAPLES: path = DIRECTORY + "Cities/NaplesBackground" + FORMAT; break;
			case METAPONTO: path = DIRECTORY + "Cities/MetapontoBackground" + FORMAT; break;
			case BOJANO: path = DIRECTORY + "Cities/BojanoBackground" + FORMAT; break;
			case ANCONA: path = DIRECTORY + "Cities/AnconaBackground" + FORMAT; break;
			case PERUGIA: path = DIRECTORY + "Cities/PerugiaBackground" + FORMAT; break;
			case FLORENCE: path = DIRECTORY + "Cities/FlorenceBackground" + FORMAT; break;
			case BOLOGNA: path = DIRECTORY + "Cities/BolognaBackground" + FORMAT; break;
			case GENOA: path = DIRECTORY + "Cities/GenoaBackground" + FORMAT; break;
			case AQUILEIA: path = DIRECTORY + "Cities/AquileiaBackground" + FORMAT; break;
			case MILAN: path = DIRECTORY + "Cities/MilanBackground" + FORMAT; break;
			}
			break;
		case Game::Background::CITY_MENU_ARENA: path = DIRECTORY + "arenaBackground" + FORMAT; break;
		case Game::Background::CITY_MENU_ARENA_FIGHT: path = DIRECTORY + "arenaFightBackground" + FORMAT; break;
		case Game::Background::CITY_MENU_ARENA_BET: path = DIRECTORY + "arenaBetBackground" + FORMAT; break;
		case Game::Background::CITY_MENU_QUEST: path = DIRECTORY + "questBackground" + FORMAT; break;
		case Game::Background::CITY_MENU_LANISTA: path = DIRECTORY + "lanistaBackground" + FORMAT; break;
		case Game::Background::CITY_MENU_MARKET: path = DIRECTORY + "marketBackground" + FORMAT; break;
		case Game::Background::CITY_MENU_CHARACTER: path = DIRECTORY + "characterInventoryBackground" + FORMAT; break;
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
	case Game::Background::CITY_MENU:
	{
		for (HWND hItem : hItems)
					ShowWindow(hItem, SW_SHOW);
		buf = l.getMessage(Localized::CITY) + " " + l.getCityName(game.getWorldMap().getCurrentCity());
		SendMessage(hItems[STAT_CITY_NAME], WM_SETTEXT, 0, (LPARAM)buf.c_str());
		// Updating log
		SendMessage(hItems[EDIT_MESSAGES_LOG], WM_SETTEXT, 0, (LPARAM)logStr.c_str());
		SendMessage(hItems[EDIT_MESSAGES_LOG], EM_SCROLL, SB_BOTTOM, 0);
	}
	break;

	case Game::Background::CITY_MENU_ARENA:
	{
		// Updating log
		SendMessage(hItems[EDIT_MESSAGES_LOG], WM_SETTEXT, 0, (LPARAM)logStr.c_str());
		SendMessage(hItems[EDIT_MESSAGES_LOG], EM_SCROLL, SB_BOTTOM, 0);
	}
	break;

	case Game::Background::CITY_MENU_ARENA_FIGHT:
	{
		if (game.isBackgroundChanged())
		{
			// # 1. Player
			{
				Player& rPlayer = game.getPlayer();
				// Name
				buf = rPlayer.getName();
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_NAME], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				// Level
				buf = l.getMessage(Localized::LEVEL) + ": " + to_string(rPlayer.getLevel());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEVEL], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				// Right hand
				if (rPlayer.getRightHand())
				{
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)l.getWeaponTypeName(*rPlayer.getRightHand()).c_str());

					buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getRightHand()->getDamage()) + " (+" + to_string(rPlayer.getRightHand()->getDamageAddition()) + ")";
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				}
				else
				{
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::EMPTY_HAND).c_str());

					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)"");
				}

				// Left hand
				if (rPlayer.getLeftHand())
				{
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)l.getWeaponTypeName(*rPlayer.getLeftHand()).c_str());

					if (rPlayer.getLeftHand()->getWeaponType() != Weapon::WeaponType::SHIELD)
					{
						buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getLeftHand()->getDamage()) + " (+" + to_string(rPlayer.getLeftHand()->getDamageAddition()) + ")";
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
					}
					else
					{
						buf = l.getMessage(Localized::BLOCK_DEFENSE) + ": " + to_string(rPlayer.getLeftHand()->getShieldDefPercentAddition()) + "%";
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_SHIELD_DEFENSE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
						buf = l.getMessage(Localized::BLOCK_CHANCE) + ": " + to_string(rPlayer.getLeftHand()->getShieldProbAddition()) + "%";
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_SHIELD_BLOCK_CHANCE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
					}
				}
				else
				{
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::EMPTY_HAND).c_str());

					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)"");
				}

				// Health
				buf = l.getMessage(Localized::HEALTH) + ": " + to_string(rPlayer.getHP());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_HEALTH], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				// Armour
				if (rPlayer.getArmour())
				{
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)l.getArmourTypeName(*rPlayer.getArmour()).c_str());

					buf = l.getMessage(Localized::ARMOUR_DEFENSE) + ": " + to_string(rPlayer.getArmour()->getDefense()) + " (+" + to_string(rPlayer.getArmour()->getDefAddition()) + ")";
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				}

				// Stats
				buf = l.getMessage(Localized::STRENGTH) + ": " + to_string(rPlayer.getStrength());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_STRENGTH], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				buf = l.getMessage(Localized::CONSTITUTION) + ": " + to_string(rPlayer.getConstitution());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				buf = l.getMessage(Localized::DEXTERITY) + ": " + to_string(rPlayer.getDexterity());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_DEXTERITY], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				buf = l.getMessage(Localized::INTELLIGENCE) + ": " + to_string(rPlayer.getIntelligence());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				buf = l.getMessage(Localized::WISDOM) + ": " + to_string(rPlayer.getWisdom());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_WISDOM], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				buf = l.getMessage(Localized::CHARISMA) + ": " + to_string(rPlayer.getCharisma());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_CHARISMA], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				buf = l.getMessage(Localized::AGE) + ": " + to_string(rPlayer.getAge());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_AGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				buf = l.getMessage(Localized::FAME) + ": " + to_string(rPlayer.getFame());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_FAME], WM_SETTEXT, 0, (LPARAM)buf.c_str());
			}

			// # 2. Opponent
			if (selectedOpponent != -1)
			{
				NPC& rOpponent = *game.getWorldMap().getCurrentCity().getArena().getGladiator(selectedOpponent);

				for (i = ARENA_FIGHT_STATIC_NAME; i <= ARENA_FIGHT_STATIC_LEFT_HAND_TYPE; i++)
					ShowWindow(hSubMenuItems[i], SW_SHOW);
				if (rOpponent.getLeftHand())
				{
					if (rOpponent.getLeftHand()->getWeaponType() != Weapon::WeaponType::SHIELD)
						ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], SW_SHOW);
					else
					{
						ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_SHIELD_DEFENSE], SW_SHOW);
						ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_SHIELD_BLOCK_CHANCE], SW_SHOW);
					}
				}
				for (i = ARENA_FIGHT_STATIC_HEALTH; i <= ARENA_FIGHT_STATIC_FAME; i++)
					ShowWindow(hSubMenuItems[i], SW_SHOW);
			}
		}
	}
	break;

	case Game::Background::CITY_MENU_MARKET:
	{
		// 1. Player inventory
		Player& rPlayer = game.getPlayer();
		Inventory& rPlayerInventory = *rPlayer.getInventory();
		int inventorySize = rPlayerInventory.size();
		buf = l.getMessage(Localized::INVENTORY) + " " + rPlayer.getName() + " (" + to_string(inventorySize) + " / " + to_string(MAX_INVENTORY_SIZE) + ")";
		SendMessage(hSubItems[MARKET_STAT_INVENTORY], WM_SETTEXT, 0, (LPARAM)buf.c_str());
		// Hide unused slots with no items
		for (i = inventorySize; i < MAX_INVENTORY_SIZE; i++)
		{
			ShowWindow(hSubItems[MARKET_BUT_INVENTORY_ITEM1 + i], SW_HIDE);
			ShowWindow(hSubItems[MARKET_STAT_INVENTORY_ITEM1 + i], SW_HIDE);
		}
		for (i = 0; i < inventorySize; i++)
		{
			ShowWindow(hSubItems[MARKET_BUT_INVENTORY_ITEM1 + i], SW_SHOW);
			ShowWindow(hSubItems[MARKET_STAT_INVENTORY_ITEM1 + i], SW_SHOW);
			Item& item = *rPlayerInventory[i].first;
			int quantity = rPlayerInventory[i].second;
			buf = l.getItemTypeName(item);
			if (quantity > 1)
				buf += " (" + to_string(quantity) + ")";
			SendMessage(hSubItems[MARKET_STAT_INVENTORY_ITEM1 + i], WM_SETTEXT, 0, (LPARAM)buf.c_str());
		}

		// 2. Player equipment
		SendMessage(hSubItems[MARKET_STAT_EQUIPMENT], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::EQUIPMENT).c_str());
		// Right hand
		if (rPlayer.getRightHand())
			buf = l.getWeaponTypeName(*rPlayer.getRightHand());
		else
			buf = l.getMessage(Localized::RIGHT_HAND);
		SendMessage(hSubItems[MARKET_STAT_RIGHT_HAND], WM_SETTEXT, 0, (LPARAM)buf.c_str());
		// Left hand
		if (rPlayer.getLeftHand())
			buf = l.getWeaponTypeName(*rPlayer.getLeftHand());
		else
			buf = l.getMessage(Localized::LEFT_HAND);
		SendMessage(hSubItems[MARKET_STAT_LEFT_HAND], WM_SETTEXT, 0, (LPARAM)buf.c_str());
		// Armour
		if (rPlayer.getArmour())
			buf = l.getArmourTypeName(*rPlayer.getArmour());
		else
			buf = l.getMessage(Localized::ARMOUR);
		SendMessage(hSubItems[MARKET_STAT_ARMOUR], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		// 3. Trader inventory
		HarmlessNPC& rTrader = game.getWorldMap().getCurrentCity().getTrader();
		Inventory& rTraderInventory = *rTrader.getInventory();
		inventorySize = rTraderInventory.size();
		buf = l.getMessage(Localized::INVENTORY) + " " + l.getNPCName(rTrader) + " (" + to_string(inventorySize) + " / " + to_string(MAX_INVENTORY_SIZE) + ")";
		SendMessage(hSubItems[MARKET_STAT_TRADER_INVENTORY], WM_SETTEXT, 0, (LPARAM)buf.c_str());
		// Hide unused slots with no items
		for (i = inventorySize; i < MAX_INVENTORY_SIZE; i++)
		{
			ShowWindow(hSubItems[MARKET_BUT_TRADER_ITEM1 + i], SW_HIDE);
			ShowWindow(hSubItems[MARKET_STAT_TRADER_ITEM1 + i], SW_HIDE);
		}
		for (i = 0; i < inventorySize; i++)
		{
			ShowWindow(hSubItems[MARKET_BUT_TRADER_ITEM1 + i], SW_SHOW);
			ShowWindow(hSubItems[MARKET_STAT_TRADER_ITEM1 + i], SW_SHOW);
			Item& item = *rTraderInventory[i].first;
			int quantity = rTraderInventory[i].second;
			buf = l.getItemTypeName(item);
			if (quantity > 1)
				buf += " (" + to_string(quantity) + ")";
			SendMessage(hSubItems[MARKET_STAT_TRADER_ITEM1 + i], WM_SETTEXT, 0, (LPARAM)buf.c_str());
		}

		// Item interaction buttons
		SendMessage(hSubItems[MARKET_BUT_UNEQUIP_ITEM], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::UNEQUIP_ITEM).c_str());
		SendMessage(hSubItems[MARKET_BUT_EQUIP_ITEM], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::EQUIP_ITEM).c_str());
		SendMessage(hSubItems[MARKET_BUT_DESTROY_ITEM], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::DESTROY_ITEM).c_str());

		// 3. Other windows
		SendMessage(hSubItems[MARKET_STAT_MARKET], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::MARKET).c_str());

		SendMessage(hSubItems[MARKET_BUT_SELL], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::SELL).c_str());
		SendMessage(hSubItems[MARKET_BUT_BUY], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::BUY).c_str());

		SendMessage(hSubItems[MARKET_BUT_BACK], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::BACK).c_str());
	}
	break;

	case Game::Background::CITY_MENU_CHARACTER:
	{
		// 1. Player stats
		Player& rPlayer = game.getPlayer();

		SendMessage(hSubItems[CHARACTER_STAT_INFO], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::CHARACTER).c_str());
		// Name
		buf = rPlayer.getName();
		SendMessage(hSubItems[CHARACTER_STAT_NAME], WM_SETTEXT, 0, (LPARAM)buf.c_str());
		// Level
		buf = l.getMessage(Localized::LEVEL) + ": " + to_string(rPlayer.getLevel());
		SendMessage(hSubItems[CHARACTER_STAT_LEVEL], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		// Experience
		buf = l.getMessage(Localized::EXPERIENCE) + ": " + to_string(rPlayer.getExperience()) + " / " + to_string(rPlayer.calculateExperienceForOneLevel(rPlayer.getLevel() + 1));
		SendMessage(hSubItems[CHARACTER_STAT_EXPERIENCE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		// Age
		buf = l.getMessage(Localized::AGE) + ": " + to_string(rPlayer.getAge());
		SendMessage(hSubItems[CHARACTER_STAT_AGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		// Fame
		buf = l.getMessage(Localized::FAME) + ": " + to_string(rPlayer.getFame());
		SendMessage(hSubItems[CHARACTER_STAT_FAME], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		// Health
		buf = l.getMessage(Localized::HEALTH) + ": (" + to_string(pas.hp) + " / " + to_string(pas.fullHP) + " )";
		SendMessage(hSubItems[CHARACTER_STAT_HEALTH], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		// Stats
		buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
		SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		buf = l.getMessage(Localized::STRENGTH) + ": " + to_string(pas.strength);
		SendMessage(hSubItems[CHARACTER_STAT_STRENGTH], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		buf = l.getMessage(Localized::CONSTITUTION) + ": " + to_string(pas.constitution);
		SendMessage(hSubItems[CHARACTER_STAT_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		buf = l.getMessage(Localized::DEXTERITY) + ": " + to_string(pas.dexterity);
		SendMessage(hSubItems[CHARACTER_STAT_DEXTERITY], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		buf = l.getMessage(Localized::INTELLIGENCE) + ": " + to_string(pas.intelligence);
		SendMessage(hSubItems[CHARACTER_STAT_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		buf = l.getMessage(Localized::WISDOM) + ": " + to_string(pas.wisdom);
		SendMessage(hSubItems[CHARACTER_STAT_WISDOM], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		buf = l.getMessage(Localized::CHARISMA) + ": " + to_string(pas.charisma);
		SendMessage(hSubItems[CHARACTER_STAT_CHARISMA], WM_SETTEXT, 0, (LPARAM)buf.c_str());

		// Buttons
		for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_CHARISMA_PLUS; i++)
			SendMessage(hSubItems[i], WM_SETTEXT, 0, (LPARAM)"+");

		for (i = CHARACTER_BUT_STRENGTH_MINUS; i <= CHARACTER_BUT_CHARISMA_MINUS; i++)
			SendMessage(hSubItems[i], WM_SETTEXT, 0, (LPARAM)"-");

		SendMessage(hSubItems[CHARACTER_BUT_RESET_CHANGES], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::RESET_CHANGES).c_str());

		SendMessage(hSubItems[CHARACTER_BUT_APPLY_CHANGES], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::APPLY_CHANGES).c_str());

		// Buttons display
		// Increasing attributes
		if (pas.unnassignedAttributes > 0)
		{
			double average = pas.calculateAverage() + ATTRIBUTE_MAX_DIFFERENCE;
			// Strength
			if (pas.strength > average)
				ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_HIDE);
			else
				ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_SHOW);
			// Constitution
			if (pas.constitution > average)
				ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_HIDE);
			else
				ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_SHOW);
			// Dexterity
			if (pas.dexterity > average)
				ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_HIDE);
			else
				ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_SHOW);
			// Intelligence
			if (pas.intelligence > average)
				ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_HIDE);
			else
				ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_SHOW);
			// Wisdom
			if (pas.wisdom > average)
				ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_HIDE);
			else
				ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_SHOW);
			// Charisma
			if (pas.charisma > average)
				ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_HIDE);
			else
				ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_SHOW);
		}
		else
		{
			for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_CHARISMA_PLUS; i++)
				ShowWindow(hSubItems[i], SW_HIDE);
		}
		// Decreasing attributes
		// Strength
		if (pas.strength > rPlayer.getStrength())
			ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_MINUS], SW_SHOW);
		else
			ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_MINUS], SW_HIDE);
		// Constitution
		if (pas.constitution > rPlayer.getConstitution())
			ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_MINUS], SW_SHOW);
		else
			ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_MINUS], SW_HIDE);
		// Dexterity
		if (pas.dexterity > rPlayer.getDexterity())
			ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_MINUS], SW_SHOW);
		else
			ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_MINUS], SW_HIDE);
		// Intelligence
		if (pas.intelligence > rPlayer.getIntelligence())
			ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_MINUS], SW_SHOW);
		else
			ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_MINUS], SW_HIDE);
		// Wisdom
		if (pas.wisdom > rPlayer.getWisdom())
			ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_MINUS], SW_SHOW);
		else
			ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_MINUS], SW_HIDE);
		// Charisma
		if (pas.charisma > rPlayer.getCharisma())
			ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_MINUS], SW_SHOW);
		else
			ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_MINUS], SW_HIDE);

		// Apply changes, reset changes
		if (pas.strength > rPlayer.getStrength() ||
			pas.constitution > rPlayer.getConstitution() ||
			pas.dexterity > rPlayer.getDexterity() ||
			pas.intelligence > rPlayer.getIntelligence() ||
			pas.wisdom > rPlayer.getWisdom() ||
			pas.charisma > rPlayer.getCharisma()
			)
		{
			ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_SHOW);
			ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_SHOW);
		}
		else
		{
			ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_HIDE);
			ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_HIDE);
		}

		// 2. Equipment
		SendMessage(hSubItems[CHARACTER_STAT_EQUIPMENT], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::EQUIPMENT).c_str());
		// Right hand
		SendMessage(hSubItems[CHARACTER_STAT_RIGHT_HAND], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::RIGHT_HAND).c_str());
		if (rPlayer.getRightHand())
		{
			ShowWindow(hSubItems[CHARACTER_STAT_RIGHT_HAND_DAMAGE], SW_SHOW);
			ShowWindow(hSubItems[CHARACTER_STAT_RIGHT_HAND_STRENGTH_SCALE], SW_SHOW);
			ShowWindow(hSubItems[CHARACTER_STAT_RIGHT_HAND_DEXTERITY_SCALE], SW_SHOW);
			Weapon& rRightHand = *rPlayer.getRightHand();
			buf = l.getWeaponTypeName(rRightHand) + " (" + l.getMessage(Localized::ITEM_TIER) + " " + to_string(rRightHand.getTier()) + ")";
			SendMessage(hSubItems[CHARACTER_STAT_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

			buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rRightHand.getDamage()) + " (+" + to_string(rRightHand.getDamageAddition()) + ")";
			SendMessage(hSubItems[CHARACTER_STAT_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

			buf = l.getMessage(Localized::STRENGTH_SCALE) + ": " + to_string(rRightHand.getStrengthAdditionPercentage()) + "%";
			SendMessage(hSubItems[CHARACTER_STAT_RIGHT_HAND_STRENGTH_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

			buf = l.getMessage(Localized::DEXTERITY_SCALE) + ": " + to_string(rRightHand.getDexterityAdditionPercentage()) + "%";
			SendMessage(hSubItems[CHARACTER_STAT_RIGHT_HAND_DEXTERITY_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
		}
		else
		{
			SendMessage(hSubItems[CHARACTER_STAT_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::EMPTY_HAND).c_str());
			ShowWindow(hSubItems[CHARACTER_STAT_RIGHT_HAND_DAMAGE], SW_HIDE);
			ShowWindow(hSubItems[CHARACTER_STAT_RIGHT_HAND_STRENGTH_SCALE], SW_HIDE);
			ShowWindow(hSubItems[CHARACTER_STAT_RIGHT_HAND_DEXTERITY_SCALE], SW_HIDE);
		}

		// Left hand
		SendMessage(hSubItems[CHARACTER_STAT_LEFT_HAND], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::LEFT_HAND).c_str());
		if (rPlayer.getLeftHand())
		{
			Weapon& rLeftHand = *rPlayer.getLeftHand();
			buf = l.getWeaponTypeName(rLeftHand) + " (" + l.getMessage(Localized::ITEM_TIER) + " " + to_string(rLeftHand.getTier()) + ")";
			SendMessage(hSubItems[CHARACTER_STAT_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

			if (rLeftHand.getWeaponType() != Weapon::WeaponType::SHIELD)
			{
				ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_DAMAGE], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_STRENGTH_SCALE], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_DEXTERITY_SCALE], SW_SHOW);

				buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rLeftHand.getDamage()) + " (+" + to_string(rLeftHand.getDamageAddition()) + ")";
				SendMessage(hSubItems[CHARACTER_STAT_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				buf = l.getMessage(Localized::STRENGTH_SCALE) + ": " + to_string(rLeftHand.getStrengthAdditionPercentage()) + "%";
				SendMessage(hSubItems[CHARACTER_STAT_LEFT_HAND_STRENGTH_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				buf = l.getMessage(Localized::DEXTERITY_SCALE) + ": " + to_string(rLeftHand.getDexterityAdditionPercentage()) + "%";
				SendMessage(hSubItems[CHARACTER_STAT_LEFT_HAND_DEXTERITY_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				ShowWindow(hSubItems[CHARACTER_STAT_SHIELD_DEFENSE], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_STAT_SHIELD_BLOCK_CHANCE], SW_HIDE);
			}
			else
			{
				ShowWindow(hSubItems[CHARACTER_STAT_SHIELD_DEFENSE], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_STAT_SHIELD_BLOCK_CHANCE], SW_SHOW);

				buf = l.getMessage(Localized::BLOCK_DEFENSE) + ": " + to_string(rLeftHand.getShieldDefPercentAddition()) + "%";
				SendMessage(hSubItems[CHARACTER_STAT_SHIELD_DEFENSE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				buf = l.getMessage(Localized::BLOCK_CHANCE) + ": " + to_string(rLeftHand.getShieldProbAddition()) + "%";
				SendMessage(hSubItems[CHARACTER_STAT_SHIELD_BLOCK_CHANCE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_DAMAGE], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_STRENGTH_SCALE], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_DEXTERITY_SCALE], SW_HIDE);
			}
		}
		else
		{
			SendMessage(hSubItems[CHARACTER_STAT_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::EMPTY_HAND).c_str());
			ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_DAMAGE], SW_HIDE);
			ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_STRENGTH_SCALE], SW_HIDE);
			ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_DEXTERITY_SCALE], SW_HIDE);
			ShowWindow(hSubItems[CHARACTER_STAT_SHIELD_DEFENSE], SW_HIDE);
			ShowWindow(hSubItems[CHARACTER_STAT_SHIELD_BLOCK_CHANCE], SW_HIDE);
		}

		// Armour
		SendMessage(hSubItems[CHARACTER_STAT_ARMOUR], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::ARMOUR).c_str());
		if (rPlayer.getArmour())
		{
			Armour& rArmour = *rPlayer.getArmour();

			ShowWindow(hSubItems[CHARACTER_STAT_ARMOUR_DEFENSE], SW_SHOW);
			ShowWindow(hSubItems[CHARACTER_STAT_ARMOUR_STRENGTH_SCALE], SW_SHOW);
			ShowWindow(hSubItems[CHARACTER_STAT_ARMOUR_DEXTERITY_SCALE], SW_SHOW);
			ShowWindow(hSubItems[CHARACTER_STAT_ARMOUR_ABILITY], SW_SHOW);

			buf = l.getArmourTypeName(rArmour) + " (" + l.getMessage(Localized::ITEM_TIER) + " " + to_string(rArmour.getTier()) + ")";
			SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

			buf = l.getMessage(Localized::ARMOUR_DEFENSE) + ": " + to_string(rArmour.getDefense()) + " (+" + to_string(rArmour.getDefAddition()) + ")";
			SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

			buf = l.getMessage(Localized::STRENGTH_SCALE) + ": " + to_string(rArmour.getStrengthAdditionPercentage()) + "%";
			SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_STRENGTH_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

			buf = l.getMessage(Localized::DEXTERITY_SCALE) + ": " + to_string(rArmour.getDexterityAdditionPercentage()) + "%";
			SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_DEXTERITY_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

			if (rArmour.getArmourType() == Armour::ArmourType::LIGHT)
				buf = l.getMessage(Localized::EVASION_CHANCE) + ": " + to_string(rArmour.getEvasionProbAddition()) + "%";
			else
				buf = l.getMessage(Localized::STUN_RESISTANCE_CHANCE) + ": " + to_string(rArmour.getStunProbSubtraction()) + "%";
			SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_ABILITY], WM_SETTEXT, 0, (LPARAM)buf.c_str());
		}
		else
		{
			SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::NO_ARMOUR).c_str());
			ShowWindow(hSubItems[CHARACTER_STAT_ARMOUR_DEFENSE], SW_HIDE);
			ShowWindow(hSubItems[CHARACTER_STAT_ARMOUR_STRENGTH_SCALE], SW_HIDE);
			ShowWindow(hSubItems[CHARACTER_STAT_ARMOUR_DEXTERITY_SCALE], SW_HIDE);
			ShowWindow(hSubItems[CHARACTER_STAT_ARMOUR_ABILITY], SW_HIDE);
		}

		// 3. Inventory
		Inventory& inventory = *rPlayer.getInventory();
		int size = inventory.size();
		buf = l.getMessage(Localized::INVENTORY) + " " + rPlayer.getName() + " (" + to_string(size) + " / " + to_string(MAX_INVENTORY_SIZE) + ")";
		SendMessage(hSubItems[CHARACTER_STAT_INVENTORY], WM_SETTEXT, 0, (LPARAM)buf.c_str());
		// Hide unused slots with no items
		for (i = size; i < MAX_INVENTORY_SIZE; i++)
		{
			ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_ITEM1 + i], SW_HIDE);
			ShowWindow(hSubItems[CHARACTER_STAT_INVENTORY_ITEM1_NAME + i], SW_HIDE);
		}
		for (i = 0; i < size; i++)
		{
			ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_ITEM1 + i], SW_SHOW);
			ShowWindow(hSubItems[CHARACTER_STAT_INVENTORY_ITEM1_NAME + i], SW_SHOW);
			Item& item = *inventory[i].first;
			int quantity = inventory[i].second;
			buf = l.getItemTypeName(item);
			if (quantity > 1)
				buf += " (" + to_string(quantity) + ")";
			SendMessage(hSubItems[CHARACTER_STAT_INVENTORY_ITEM1_NAME + i], WM_SETTEXT, 0, (LPARAM)buf.c_str());
		}

		// Back button
		SendMessage(hSubItems[CHARACTER_BUT_BACK], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::BACK).c_str());

		// Item interaction buttons
		SendMessage(hSubItems[CHARACTER_BUT_INVENTORY_UNEQUIP_ITEM], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::UNEQUIP_ITEM).c_str());
		SendMessage(hSubItems[CHARACTER_BUT_INVENTORY_EQUIP_ITEM], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::EQUIP_ITEM).c_str());
		SendMessage(hSubItems[CHARACTER_BUT_INVENTORY_INSPECT_ITEM], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::INSPECT_ITEM).c_str());
		SendMessage(hSubItems[CHARACTER_BUT_INVENTORY_DESTROY_ITEM], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::DESTROY_ITEM).c_str());
	}
	break;
	}
	game.backgroundChangeCompleted();
}

void CityMenu::resizeMenu(int cx, int cy)
{
	// If background is the same - no change needed
	if (!game.isBackgroundChanged())
		return;

	int sz, x, y, i;
	switch (game.getBackground())
	{
	case Game::Background::CITY_MENU:
	{
		const int ITEM_HEIGHT = 60, DISTANCE = 15, ITEM_WIDTH = 300;
		sz = hItems.size();
		x = cx - ITEM_WIDTH * 2;
		y = cy - (int)(sz / 1.7 * (ITEM_HEIGHT + DISTANCE));
		y += (ITEM_HEIGHT + DISTANCE) / 2;
		for (HWND hItem : hItems)
		{
			if (hItem == hItems[BUT_MAP])
				y += (ITEM_HEIGHT + DISTANCE) / 2;
			MoveWindow(hItem, x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
			y += ITEM_HEIGHT + DISTANCE;
		}
		MoveWindow(hItems[EDIT_MESSAGES_LOG], 830, 510, 500, 200, TRUE);
	}
	break;

	case Game::Background::CITY_MENU_ARENA:
	{
		const int STAT_WIDTH = 125, EDIT_WIDTH = 40, BUT_WIDTH = 300, ITEM_HEIGHT = 60, DISTANCE = 15;
		cy -= 3 * (ITEM_HEIGHT + DISTANCE);
		y = cy;
		x = cx - BUT_WIDTH * 2;

		for (i = ARENA_BUT_FIGHT; i < ARENA_ITEM_NUMBER; i++)
		{
			y += ITEM_HEIGHT + DISTANCE;
			MoveWindow(hSubItems[i], x, y, BUT_WIDTH, ITEM_HEIGHT, TRUE);
		}
		MoveWindow(hItems[EDIT_MESSAGES_LOG], 830, 510, 500, 200, TRUE);
	}
	break;

	case Game::Background::CITY_MENU_QUEST:
	{
		const int STAT_WIDTH = 125, EDIT_WIDTH = 40, BUT_WIDTH = 300, ITEM_HEIGHT = 60, DISTANCE = 15;
		cy -= 3 * (ITEM_HEIGHT + DISTANCE);
		y = cy;
		x = cx - BUT_WIDTH * 2;

		for (i = QUEST_BUT_LANISTA; i < QUEST_ITEM_NUMBER; i++)
		{
			y += ITEM_HEIGHT + DISTANCE;
			MoveWindow(hSubItems[i], x, y, BUT_WIDTH, ITEM_HEIGHT, TRUE);
		}
		MoveWindow(hItems[EDIT_MESSAGES_LOG], 830, 510, 500, 200, TRUE);
	}
	break;

	case Game::Background::CITY_MENU_LANISTA:
	{
		const int STAT_WIDTH = 125, EDIT_WIDTH = 40, BUT_WIDTH = 300, ITEM_HEIGHT = 60, DISTANCE = 15;
		cy -= 3 * (ITEM_HEIGHT + DISTANCE);
		y = cy;
		x = cx - BUT_WIDTH * 2;

		for (i = LANISTA_BUT_BUY_FREEDOM; i < LANISTA_ITEM_NUMBER; i++)
		{
			y += ITEM_HEIGHT + DISTANCE;
			MoveWindow(hSubMenuItems[i], x, y, BUT_WIDTH, ITEM_HEIGHT, TRUE);
		}
		MoveWindow(hItems[EDIT_MESSAGES_LOG], 830, 510, 500, 200, TRUE);
	}
	break;

	case Game::Background::CITY_MENU_ARENA_FIGHT:
	{
		const int STAT_WIDTH = 170, EDIT_WIDTH = 40, BUT_WIDTH = 300, ITEM_HEIGHT = 30, DISTANCE = 9;
		//cy -= 3 * (ITEM_HEIGHT + DISTANCE);
		y = DISTANCE * 2 + ITEM_HEIGHT * 2;
		x = cx - BUT_WIDTH * 2 - DISTANCE * 2;

		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_OPPONENTS], x, ITEM_HEIGHT, BUT_WIDTH, ITEM_HEIGHT + DISTANCE, TRUE);
		// Opponents list
		for (i = ARENA_FIGHT_BUT_OPPONENT1; i <= ARENA_FIGHT_BUT_OPPONENT15; i++)
		{
			MoveWindow(hSubMenuItems[i], x, y, BUT_WIDTH, ITEM_HEIGHT, TRUE);
			y += ITEM_HEIGHT + DISTANCE;
		}

		// Player's stats
		y = DISTANCE * 2 + ITEM_HEIGHT * 2;
		x = cx + (int)(BUT_WIDTH * 0.5) + DISTANCE * 12;

		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_NAME], x, y - ITEM_HEIGHT - DISTANCE, STAT_WIDTH, ITEM_HEIGHT, TRUE);

		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEVEL], x, y, STAT_WIDTH, ITEM_HEIGHT, TRUE);

		// Portrait

		y += DISTANCE + ITEM_HEIGHT;
		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_PORTRAIT], x, y, 170, 143, TRUE);

		// RightHand stats
		y = cy - (ITEM_HEIGHT + DISTANCE) * 3;
		x -= 120;
		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_RIGHT_HAND_TYPE], x, y, 200, ITEM_HEIGHT, TRUE);
		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_RIGHT_HAND_DAMAGE], x, y + ITEM_HEIGHT + DISTANCE, 200, ITEM_HEIGHT, TRUE);

		// LeftHand stats
		x += 40 + STAT_WIDTH;
		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_TYPE], x, y, 200, ITEM_HEIGHT, TRUE);
		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_DAMAGE], x, y + ITEM_HEIGHT + DISTANCE, 200, ITEM_HEIGHT, TRUE);

		// Shield stats
		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_SHIELD_DEFENSE], x, y + ITEM_HEIGHT + DISTANCE, 200, ITEM_HEIGHT, TRUE);
		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_SHIELD_BLOCK_CHANCE], x, y + 2 * (ITEM_HEIGHT + DISTANCE), 200, ITEM_HEIGHT, TRUE);

		// Other stats
		x -= STAT_WIDTH - 80;
		y = cy;
		for (i = ARENA_FIGHT_STATIC_PLAYER_HEALTH; i <= ARENA_FIGHT_STATIC_PLAYER_FAME; i++)
		{
			MoveWindow(hSubMenuItems[i], x, y, STAT_WIDTH, ITEM_HEIGHT - 10, TRUE);
			y += ITEM_HEIGHT - 10 + DISTANCE / 2;
			if (i == ARENA_FIGHT_STATIC_PLAYER_ARMOUR_DEFENSE || i == ARENA_FIGHT_STATIC_PLAYER_CHARISMA)
				y += DISTANCE;
		}

		// Opponent's stats
		y = DISTANCE * 2 + ITEM_HEIGHT * 2;
		x = cx + (int)(BUT_WIDTH * 0.5) + DISTANCE - 340;
		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_NAME], x, y - ITEM_HEIGHT - DISTANCE, STAT_WIDTH, ITEM_HEIGHT, TRUE);

		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_LEVEL], x, y, STAT_WIDTH, ITEM_HEIGHT, TRUE);

		// Portrait
		y += DISTANCE + ITEM_HEIGHT;
		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PORTRAIT], x, y, 170, 143, TRUE);

		// RightHand stats
		y = cy - (ITEM_HEIGHT + DISTANCE) * 3;
		x -= 120;
		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_TYPE], x, y, 200, ITEM_HEIGHT, TRUE);
		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_DAMAGE], x, y + ITEM_HEIGHT + DISTANCE, 200, ITEM_HEIGHT, TRUE);

		// LeftHand stats
		x += 40 + STAT_WIDTH;
		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], x, y, 200, ITEM_HEIGHT, TRUE);
		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], x, y + ITEM_HEIGHT + DISTANCE, 200, ITEM_HEIGHT, TRUE);

		// Shield stats
		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_SHIELD_DEFENSE], x, y + ITEM_HEIGHT + DISTANCE, 200, ITEM_HEIGHT, TRUE);
		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_SHIELD_BLOCK_CHANCE], x, y + 2 * (ITEM_HEIGHT + DISTANCE), 200, ITEM_HEIGHT, TRUE);

		// Other stats
		x -= STAT_WIDTH - 80;
		y = cy;
		for (i = ARENA_FIGHT_STATIC_HEALTH; i <= ARENA_FIGHT_STATIC_FAME; i++)
		{
			MoveWindow(hSubMenuItems[i], x, y, STAT_WIDTH, ITEM_HEIGHT - 10, TRUE);
			y += ITEM_HEIGHT - 10 + DISTANCE / 2;
			if (i == ARENA_FIGHT_STATIC_ARMOUR_DEFENSE || i == ARENA_FIGHT_STATIC_CHARISMA)
				y += DISTANCE;
		}

		// Back and Fight buttons
		x = 374;
		y = 672;

		MoveWindow(hSubMenuItems[ARENA_FIGHT_BUT_BACK], x, y, BUT_WIDTH, ITEM_HEIGHT, TRUE);

		MoveWindow(hSubMenuItems[ARENA_FIGHT_BUT_FIGHT], x + BUT_WIDTH + DISTANCE, y, BUT_WIDTH, ITEM_HEIGHT, TRUE);
	}
	break;

	case Game::Background::CITY_MENU_ARENA_BET:
	{
		// TODO
	}
	break;

	case Game::Background::CITY_MENU_MARKET:
	{
		const int BIG_STAT_WIDTH = 478, STAT_HEIGHT = 30,
				  STAT_WIDTH = 278,
				  DISTANCE = 9,
				  ITEMS_DISTANCE = 0,
				  ITEM_BUT_WIDTH = 120, ITEM_BUT_HEIGHT = 97,
				  ITEM_STAT_WIDTH = 120, ITEM_STAT_HEIGHT = 20;

		MoveWindow(hSubItems[MARKET_STAT_MARKET], cx - BIG_STAT_WIDTH / 2, DISTANCE, BIG_STAT_WIDTH, STAT_HEIGHT, TRUE);

		// Player's inventory
		x = DISTANCE * 2, y = ITEM_BUT_HEIGHT;
		MoveWindow(hSubItems[MARKET_STAT_INVENTORY], x, y - STAT_HEIGHT - DISTANCE, BIG_STAT_WIDTH, STAT_HEIGHT, TRUE);
		Inventory& playerInventory = *game.getPlayer().getInventory();
		int inventorySize = playerInventory.size();
		for (i = 0; i < inventorySize; i++)
		{
			MoveWindow(hSubItems[MARKET_BUT_INVENTORY_ITEM1 + i], x, y, ITEM_BUT_WIDTH, ITEM_BUT_HEIGHT, TRUE);
			MoveWindow(hSubItems[MARKET_STAT_INVENTORY_ITEM1 + i], x, y + ITEM_BUT_HEIGHT, ITEM_STAT_WIDTH, ITEM_STAT_HEIGHT, TRUE);

			x += ITEM_BUT_WIDTH + ITEMS_DISTANCE;
			if (i % 4 == 3)
			{
				x -= (ITEM_BUT_WIDTH + ITEMS_DISTANCE) * 4;
				y += ITEM_BUT_WIDTH + ITEMS_DISTANCE;
			}
		}

		// Player's equipment
		x = 18, y = 576;
		MoveWindow(hSubItems[MARKET_STAT_EQUIPMENT], x, y, 378, STAT_HEIGHT, TRUE);
		y += STAT_HEIGHT + 2;
		MoveWindow(hSubItems[MARKET_BUT_RIGHT_HAND], x, y, ITEM_BUT_WIDTH, ITEM_BUT_HEIGHT, TRUE);
		MoveWindow(hSubItems[MARKET_STAT_RIGHT_HAND], x, y + ITEM_BUT_HEIGHT, ITEM_STAT_WIDTH, ITEM_STAT_HEIGHT, TRUE);
		x += ITEM_BUT_WIDTH + DISTANCE;
		MoveWindow(hSubItems[MARKET_BUT_LEFT_HAND], x, y, ITEM_BUT_WIDTH, ITEM_BUT_HEIGHT, TRUE);
		MoveWindow(hSubItems[MARKET_STAT_LEFT_HAND], x, y + ITEM_BUT_HEIGHT, ITEM_STAT_WIDTH, ITEM_STAT_HEIGHT, TRUE);
		x += ITEM_BUT_WIDTH + DISTANCE;
		MoveWindow(hSubItems[MARKET_BUT_ARMOUR], x, y, ITEM_BUT_WIDTH, ITEM_BUT_HEIGHT, TRUE);
		MoveWindow(hSubItems[MARKET_STAT_ARMOUR], x, y + ITEM_BUT_HEIGHT, ITEM_STAT_WIDTH, ITEM_STAT_HEIGHT, TRUE);

		// Trader's inventory
		x = cx + ITEM_BUT_WIDTH + DISTANCE * 6, y = ITEM_BUT_HEIGHT;
		MoveWindow(hSubItems[MARKET_STAT_TRADER_INVENTORY], x, y - STAT_HEIGHT - DISTANCE, BIG_STAT_WIDTH, STAT_HEIGHT, TRUE);
		Inventory& traderInventory = *game.getWorldMap().getCurrentCity().getTrader().getInventory();
		inventorySize = traderInventory.size();
		for (i = 0; i < inventorySize; i++)
		{
			MoveWindow(hSubItems[MARKET_BUT_TRADER_ITEM1 + i], x, y, ITEM_BUT_WIDTH, ITEM_BUT_HEIGHT, TRUE);
			MoveWindow(hSubItems[MARKET_STAT_TRADER_ITEM1 + i], x, y + ITEM_BUT_HEIGHT, ITEM_STAT_WIDTH, ITEM_STAT_HEIGHT, TRUE);

			x += ITEM_BUT_WIDTH + ITEMS_DISTANCE;
			if (i % 4 == 3)
			{
				x -= (ITEM_BUT_WIDTH + ITEMS_DISTANCE) * 4;
				y += ITEM_BUT_WIDTH + ITEMS_DISTANCE;
			}
		}

		// Item stats
		x = 534, y = 115;
		MoveWindow(hSubItems[MARKET_STAT_ITEM_TYPE], x, y, STAT_WIDTH, STAT_HEIGHT, TRUE);
		y += STAT_HEIGHT + DISTANCE;

		MoveWindow(hSubItems[MARKET_STAT_ITEM_TIER], x, y, STAT_WIDTH, STAT_HEIGHT, TRUE);
		y += STAT_HEIGHT + DISTANCE;

		MoveWindow(hSubItems[MARKET_STAT_ITEM_DAMAGE], x, y, STAT_WIDTH, STAT_HEIGHT, TRUE);
		MoveWindow(hSubItems[MARKET_STAT_ITEM_DEFENSE], x, y, STAT_WIDTH, STAT_HEIGHT, TRUE);
		MoveWindow(hSubItems[MARKET_STAT_ITEM_SHIELD_CHANCE], x, y, STAT_WIDTH, STAT_HEIGHT, TRUE);
		y += STAT_HEIGHT + DISTANCE;

		MoveWindow(hSubItems[MARKET_STAT_ITEM_STRENGTH_SCALE], x, y, STAT_WIDTH, STAT_HEIGHT, TRUE);
		MoveWindow(hSubItems[MARKET_STAT_ITEM_SHIELD_BLOCK_DEFENSE], x, y, STAT_WIDTH, STAT_HEIGHT, TRUE);
		y += STAT_HEIGHT + DISTANCE;

		MoveWindow(hSubItems[MARKET_STAT_ITEM_DEXTERITY_SCALE], x, y, STAT_WIDTH, STAT_HEIGHT, TRUE);
		y += STAT_HEIGHT + DISTANCE;
		
		MoveWindow(hSubItems[MARKET_STAT_ITEM_ARMOUR_ABILITY], x, y, STAT_WIDTH, STAT_HEIGHT, TRUE);
		y += STAT_HEIGHT + DISTANCE;

		MoveWindow(hSubItems[MARKET_STAT_ITEM_VALUE], x, y, STAT_WIDTH, STAT_HEIGHT, TRUE);
		y += STAT_HEIGHT + DISTANCE;

		MoveWindow(hSubItems[MARKET_STAT_ITEM_QUANTITY], x, y, STAT_WIDTH, STAT_HEIGHT, TRUE);
		y += STAT_HEIGHT + DISTANCE;

		MoveWindow(hSubItems[MARKET_STAT_ITEM_TOTAL_VALUE], x, y, STAT_WIDTH, STAT_HEIGHT, TRUE);
		y += STAT_HEIGHT + DISTANCE;

		// Interaction buttons
		MoveWindow(hSubItems[MARKET_BUT_UNEQUIP_ITEM], 0, 0, 100, 30, TRUE);
		MoveWindow(hSubItems[MARKET_BUT_EQUIP_ITEM], 0, 0, 100, 30, TRUE);
		MoveWindow(hSubItems[MARKET_BUT_DESTROY_ITEM], 0, 0, 100, 30, TRUE);

		// Other buttons
		MoveWindow(hSubItems[MARKET_BUT_SELL], cx - 100, 600, 200, 30, TRUE);
		MoveWindow(hSubItems[MARKET_BUT_BUY], cx - 100, 600, 200, 30, TRUE);
		MoveWindow(hSubItems[MARKET_BUT_BACK], cx - 150, 660, 300, 30, TRUE);
	}
	break;

	case Game::Background::CITY_MENU_CHARACTER:
	{
		const int BIG_STAT_WIDTH = 250, BIG_STAT_HEIGHT = 30,
				SMALL_STAT_WIDTH = 180, SMALL_STAT_HEIGHT = 20,
				BIG_DISTANCE = 9, SMALL_DISTANCE = 4,
				BUT_SIZE = 20;

		// 1. Player's stats
		x = 87, y = 68;
		MoveWindow(hSubItems[CHARACTER_STAT_INFO], x, y - BIG_STAT_HEIGHT - (BIG_DISTANCE * 3), SMALL_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
		// Name, level, experience
		for (i = CHARACTER_STAT_NAME; i <= CHARACTER_STAT_EXPERIENCE; i++)
		{
			MoveWindow(hSubItems[i], x, y, SMALL_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
			y += BIG_STAT_HEIGHT + BIG_DISTANCE;
		}

		// Portrait
		y -= BIG_STAT_HEIGHT;
		y += BIG_DISTANCE * 5;

		MoveWindow(hSubItems[CHARACTER_STAT_PORTRAIT], x, y, 180, 196, TRUE);

		y += BIG_STAT_HEIGHT;
		// Age, Fame, health
		y += 205;
		for (i = CHARACTER_STAT_AGE; i <= CHARACTER_STAT_HEALTH; i++)
		{
			MoveWindow(hSubItems[i], x, y, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
			y += SMALL_STAT_HEIGHT + SMALL_DISTANCE;
		}

		// Attributes
		y += BIG_DISTANCE - SMALL_DISTANCE;
		int y2 = y + SMALL_STAT_HEIGHT + SMALL_DISTANCE;

		MoveWindow(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], x, y, SMALL_STAT_WIDTH + (SMALL_DISTANCE + BUT_SIZE) * 2, SMALL_STAT_HEIGHT, TRUE);
		y += SMALL_STAT_HEIGHT + SMALL_DISTANCE;

		for (i = CHARACTER_STAT_STRENGTH; i <= CHARACTER_STAT_CHARISMA; i++)
		{
			MoveWindow(hSubItems[i], x, y, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
			y += SMALL_STAT_HEIGHT + SMALL_DISTANCE;
		}

		// Minus and plus buttons
		x += SMALL_STAT_WIDTH + SMALL_DISTANCE;
		for (i = CHARACTER_BUT_STRENGTH_MINUS; i <= CHARACTER_BUT_CHARISMA_MINUS; i++)
		{
			// Minus button
			MoveWindow(hSubItems[i], x, y2, BUT_SIZE, BUT_SIZE, TRUE);
			// Plus button
			MoveWindow(hSubItems[i - 6], x + BUT_SIZE + SMALL_DISTANCE, y2, BUT_SIZE, BUT_SIZE, TRUE);
			y2 += SMALL_STAT_HEIGHT + SMALL_DISTANCE;
		}

		// Reset changes button
		x -= SMALL_STAT_WIDTH + SMALL_DISTANCE;
		y += SMALL_DISTANCE * 3;
		MoveWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], x, y, SMALL_STAT_WIDTH * 2 / 3, BIG_STAT_HEIGHT, TRUE);

		// Apply changes button
		MoveWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], x + (SMALL_STAT_WIDTH * 2 / 3) + BIG_DISTANCE, y, SMALL_STAT_WIDTH * 2 / 3, BIG_STAT_HEIGHT, TRUE);

		// 2. Equipment
		const int INVENTORY_BUT_WIDTH = 120, INVENTORY_BUT_HEIGHT = 97;
		x = 364, y = 195;
		MoveWindow(hSubItems[CHARACTER_STAT_EQUIPMENT], x, y - INVENTORY_BUT_HEIGHT - BIG_STAT_HEIGHT * 2 - BIG_DISTANCE * 3, BIG_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
		// Right hand
		MoveWindow(hSubItems[CHARACTER_STAT_RIGHT_HAND], x, y - INVENTORY_BUT_HEIGHT - BIG_STAT_HEIGHT - BIG_DISTANCE * 2, BIG_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
		MoveWindow(hSubItems[CHARACTER_BUT_RIGHT_HAND], x + 65, y - INVENTORY_BUT_HEIGHT - BIG_DISTANCE, INVENTORY_BUT_WIDTH, INVENTORY_BUT_HEIGHT, TRUE);
		for (i = CHARACTER_STAT_RIGHT_HAND_TYPE; i <= CHARACTER_STAT_RIGHT_HAND_DEXTERITY_SCALE; i++)
		{
			MoveWindow(hSubItems[i], x, y, BIG_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
			y += BIG_STAT_HEIGHT + BIG_DISTANCE;
		}

		// Left hand
		x += BIG_STAT_WIDTH + 10, y = 195;
		MoveWindow(hSubItems[CHARACTER_STAT_LEFT_HAND], x, y - INVENTORY_BUT_HEIGHT - BIG_STAT_HEIGHT - BIG_DISTANCE * 2, BIG_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
		MoveWindow(hSubItems[CHARACTER_BUT_LEFT_HAND], x + 65, y - INVENTORY_BUT_HEIGHT - BIG_DISTANCE, INVENTORY_BUT_WIDTH, INVENTORY_BUT_HEIGHT, TRUE);
		for (i = CHARACTER_STAT_LEFT_HAND_TYPE; i <= CHARACTER_STAT_LEFT_HAND_DEXTERITY_SCALE; i++)
		{
			MoveWindow(hSubItems[i], x, y, BIG_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
			// Shield stats
			if (i == CHARACTER_STAT_LEFT_HAND_DAMAGE)
				MoveWindow(hSubItems[CHARACTER_STAT_SHIELD_DEFENSE], x, y, BIG_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
			if (i == CHARACTER_STAT_LEFT_HAND_STRENGTH_SCALE)
				MoveWindow(hSubItems[CHARACTER_STAT_SHIELD_BLOCK_CHANCE], x, y, BIG_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
			y += BIG_STAT_HEIGHT + BIG_DISTANCE;
		}

		// Armour
		x += BIG_STAT_WIDTH + 10, y = 195;
		MoveWindow(hSubItems[CHARACTER_STAT_ARMOUR], x, y - INVENTORY_BUT_HEIGHT - BIG_STAT_HEIGHT - BIG_DISTANCE * 2, BIG_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
		MoveWindow(hSubItems[CHARACTER_BUT_ARMOUR], x + 65, y - INVENTORY_BUT_HEIGHT - BIG_DISTANCE, INVENTORY_BUT_WIDTH, INVENTORY_BUT_HEIGHT, TRUE);
		for (i = CHARACTER_STAT_ARMOUR_TYPE; i <= CHARACTER_STAT_ARMOUR_ABILITY; i++)
		{
			MoveWindow(hSubItems[i], x, y, BIG_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
			y += BIG_STAT_HEIGHT + BIG_DISTANCE;
		}

		// 3. Inventory
		Inventory& inventory = *game.getPlayer().getInventory();

		x = 364, y = 390;
		MoveWindow(hSubItems[CHARACTER_STAT_INVENTORY], x, y, BIG_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);

		y += BIG_STAT_HEIGHT + BIG_DISTANCE;
		// Buttons and item text
		int size = inventory.size();
		for (i = 0; i < size; i++)
		{
			MoveWindow(hSubItems[CHARACTER_BUT_INVENTORY_ITEM1 + i], x, y, INVENTORY_BUT_WIDTH, INVENTORY_BUT_HEIGHT, TRUE);
			MoveWindow(hSubItems[CHARACTER_STAT_INVENTORY_ITEM1_NAME + i], x, y + INVENTORY_BUT_HEIGHT, INVENTORY_BUT_WIDTH - 1, SMALL_STAT_HEIGHT + 3, TRUE);

			x += INVENTORY_BUT_WIDTH + SMALL_DISTANCE;
			if (i == 6)
			{
				x -= (INVENTORY_BUT_WIDTH + SMALL_DISTANCE) * 7;
				y += INVENTORY_BUT_WIDTH + SMALL_DISTANCE;
			}
		}
		// Interaction buttons
		MoveWindow(hSubItems[CHARACTER_BUT_INVENTORY_UNEQUIP_ITEM], 0, 0, 100, 30, TRUE);
		MoveWindow(hSubItems[CHARACTER_BUT_INVENTORY_EQUIP_ITEM], 0, 0, 100, 30, TRUE);
		MoveWindow(hSubItems[CHARACTER_BUT_INVENTORY_INSPECT_ITEM], 0, 0, 100, 30, TRUE);
		MoveWindow(hSubItems[CHARACTER_BUT_INVENTORY_DESTROY_ITEM], 0, 0, 100, 30, TRUE);

		// TODO
		// Inspect window
		/*x = 475, y = 113;
		MoveWindow(hSubItems[CHARACTER_INSPECT], x, y, 400, 500, TRUE);
		x += 140, y += BIG_DISTANCE;
		MoveWindow(hSubItems[CHARACTER_BUT_INSPECT_ICON], x, y, INVENTORY_BUT_WIDTH, INVENTORY_BUT_HEIGHT, TRUE);
		MoveWindow(hSubItems[CHARACTER_STAT_INSPECT_TYPE], x, y + INVENTORY_BUT_HEIGHT, INVENTORY_BUT_WIDTH - 1, SMALL_STAT_HEIGHT + 3, TRUE);
		x -= 66, y += INVENTORY_BUT_HEIGHT + SMALL_STAT_HEIGHT + BIG_DISTANCE * 2;

		MoveWindow(hSubItems[CHARACTER_STAT_INSPECT_DAMAGE], x, y, BIG_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
		MoveWindow(hSubItems[CHARACTER_STAT_INSPECT_ARMOUR_DEFENSE], x, y, BIG_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
		MoveWindow(hSubItems[CHARACTER_STAT_INSPECT_SHIELD_DEFENSE], x, y, BIG_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
		y += BIG_STAT_HEIGHT + BIG_DISTANCE;

		MoveWindow(hSubItems[CHARACTER_STAT_INSPECT_STRENGTH_SCALE], x, y, BIG_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
		MoveWindow(hSubItems[CHARACTER_STAT_INSPECT_SHIELD_BLOCK_CHANCE], x, y, BIG_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
		y += BIG_STAT_HEIGHT + BIG_DISTANCE;

		MoveWindow(hSubItems[CHARACTER_STAT_INSPECT_DEXTERITY_SCALE], x, y, BIG_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
		y += BIG_STAT_HEIGHT + BIG_DISTANCE;

		MoveWindow(hSubItems[CHARACTER_STAT_INSPECT_VALUE], x, y, BIG_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
		y += BIG_STAT_HEIGHT + BIG_DISTANCE;

		MoveWindow(hSubItems[CHARACTER_STAT_INSPECT_QUANTITY], x, y, BIG_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
		y += BIG_STAT_HEIGHT + BIG_DISTANCE;

		MoveWindow(hSubItems[CHARACTER_STAT_INSPECT_TOTAL_VALUE], x, y, BIG_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
		y += BIG_STAT_HEIGHT * 2 + BIG_DISTANCE;

		MoveWindow(hSubItems[CHARACTER_BUT_INSPECT_CLOSE], x + 50, y, BIG_STAT_WIDTH - 100, BIG_STAT_HEIGHT, TRUE);*/

		// Back button
		MoveWindow(hSubItems[CHARACTER_BUT_BACK], 364, 680, 300, BIG_STAT_HEIGHT, TRUE);
	}
	break;
	}
}

void CityMenu::handleInput(HWND hWnd, UINT m, WPARAM wp, LPARAM lp)
{
	RECT windowRect;
	GetWindowRect(hWnd, &windowRect);
	int i, j;

	switch (m)
	{
	case WM_COMMAND:
		switch (game.getBackground())
		{
		default:case Game::Background::CITY_MENU:
		{
			if ((HWND)lp == hItems[BUT_ARENA])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Hiding all windows except log
				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_HIDE);
				ShowWindow(hItems[EDIT_MESSAGES_LOG], SW_SHOW);

				// Erasing previous sub menu items
				for (HWND hItem : hSubItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubItems.clear();

				// Creating new sub menu items
				hSubItems.resize(ARENA_ITEM_NUMBER);

				hSubItems[ARENA_BUT_FIGHT] = CreateWindow("BUTTON", l.getMessage(Localized::FIGHT).c_str(),
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
					0, 0, 0, 0, hWnd, 0, hInst, 0
				);
				hSubItems[ARENA_BUT_BET] = CreateWindow("BUTTON", l.getMessage(Localized::BET).c_str(),
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
					0, 0, 0, 0, hWnd, 0, hInst, 0
				);
				hSubItems[ARENA_BUT_TRAIN] = CreateWindow("BUTTON", l.getMessage(Localized::TRAIN).c_str(),
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
					0, 0, 0, 0, hWnd, 0, hInst, 0
				);
				hSubItems[ARENA_BUT_BACK] = CreateWindow("BUTTON", l.getMessage(Localized::BACK).c_str(),
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
					0, 0, 0, 0, hWnd, 0, hInst, 0
				);

				game.setBackground(Game::Background::CITY_MENU_ARENA);

				updateWindow(hWnd);
			}
			if ((HWND)lp == hItems[BUT_QUEST])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Hiding all windows except log
				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_HIDE);
				ShowWindow(hItems[EDIT_MESSAGES_LOG], SW_SHOW);

				// Erasing previous sub menu items
				for (HWND hItem : hSubItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubItems.clear();

				// Creating new sub menu items
				hSubItems.resize(QUEST_ITEM_NUMBER);

				hSubItems[QUEST_BUT_LANISTA] = CreateWindow("BUTTON", l.getMessage(Localized::LANISTA).c_str(),
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
					0, 0, 0, 0, hWnd, 0, hInst, 0
				);
				hSubItems[QUEST_BUT_TAVERN] = CreateWindow("BUTTON", l.getMessage(Localized::TAVERN).c_str(),
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
					0, 0, 0, 0, hWnd, 0, hInst, 0
				);
				hSubItems[QUEST_BUT_TALK_TO_PEOPLE] = CreateWindow("BUTTON", l.getMessage(Localized::TALK_TO_PEOPLE).c_str(),
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
					0, 0, 0, 0, hWnd, 0, hInst, 0
				);
				hSubItems[ARENA_BUT_BACK] = CreateWindow("BUTTON", l.getMessage(Localized::BACK).c_str(),
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
					0, 0, 0, 0, hWnd, 0, hInst, 0
				);

				game.setBackground(Game::Background::CITY_MENU_QUEST);

				updateWindow(hWnd);
			}
			if ((HWND)lp == hItems[BUT_MARKET])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Hiding all windows
				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_HIDE);

				// Erasing previous sub menu items
				for (HWND hItem : hSubItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubItems.clear();

				// Creating new sub menu items
				hSubItems.resize(MARKET_ITEM_NUMBER);

				for (i = MARKET_STAT_MARKET; i <= MARKET_STAT_TRADER_ITEM14; i++)
					hSubItems[i] = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				for (i = MARKET_BUT_SELL; i <= MARKET_BUT_BACK; i++)
					hSubItems[i] = CreateWindow("BUTTON", "", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				for (int i = MARKET_STAT_ITEM_TYPE; i <= MARKET_STAT_ITEM_TOTAL_VALUE; i++)
					ShowWindow(hSubItems[i], SW_HIDE);

				ShowWindow(hSubItems[MARKET_BUT_SELL], SW_HIDE);
				ShowWindow(hSubItems[MARKET_BUT_BUY], SW_HIDE);
				ShowWindow(hSubItems[MARKET_BUT_UNEQUIP_ITEM], SW_HIDE);
				ShowWindow(hSubItems[MARKET_BUT_EQUIP_ITEM], SW_HIDE);
				ShowWindow(hSubItems[MARKET_BUT_DESTROY_ITEM], SW_HIDE);

				game.setBackground(Game::Background::CITY_MENU_MARKET);

				// Update item's prices based on player's charisma
				Inventory& rInventory = *game.getWorldMap().getCurrentCity().getTrader().getInventory();
				int charisma = game.getPlayer().getCharisma();
				for (i = 0; i < rInventory.size(); i++)
					if (rInventory[i].first->getItemType() != Item::ItemType::GOLD)
						rInventory[i].first->calculatePrice(charisma);

				updateWindow(hWnd);
			}
			if ((HWND)lp == hItems[BUT_CHARACTER])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Hiding all windows
				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_HIDE);

				// Erasing previous sub menu items
				for (HWND hItem : hSubItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubItems.clear();

				// Creating new sub menu items
				hSubItems.resize(CHARACTER_ITEM_NUMBER);

				for (i = CHARACTER_STAT_INFO; i <= CHARACTER_STAT_INVENTORY_ITEM14_NAME; i++)
					hSubItems[i] = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_BACK; i++)
					hSubItems[i] = CreateWindow("BUTTON", "", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				
				ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_UNEQUIP_ITEM], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_EQUIP_ITEM], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_INSPECT_ITEM], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_DESTROY_ITEM], SW_HIDE);
				
				Player& rPlayer = game.getPlayer();
				pas.hp = rPlayer.getHP();
				pas.fullHP = rPlayer.getFullHP();
				pas.unnassignedAttributes = rPlayer.getUnnassignedAttributes();
				pas.strength = rPlayer.getStrength();
				pas.constitution = rPlayer.getConstitution();
				pas.dexterity = rPlayer.getDexterity();
				pas.intelligence = rPlayer.getIntelligence();
				pas.wisdom = rPlayer.getWisdom();
				pas.charisma = rPlayer.getCharisma();

				game.setBackground(Game::Background::CITY_MENU_CHARACTER);

				updateWindow(hWnd);
				game.updateBackground();
			}
			if ((HWND)lp == hItems[BUT_REST])
			{
				Player& rPlayer = game.getPlayer();
				if (rPlayer.getHP() < rPlayer.getFullHP())
				{
					playSound(SoundEnum::SOUND_BUTTON_CLICK);
					int regen = BASIC_REGEN * rPlayer.getFullHP() / 100;

					// Heal player
					if (rPlayer.getHP() + regen >= rPlayer.getFullHP())
					{
						regen = rPlayer.getFullHP() - rPlayer.getHP();
						rPlayer.setHP(rPlayer.getFullHP());
						logStr += l.getMessage(Localized::REST_REGEN) + " " + to_string(regen) + " " + l.getMessage(Localized::HEALTH_POINTS_GENITIVE) + "\r\n"
							+ l.getMessage(Localized::REST_REGEN_FULL) + " (" + to_string(rPlayer.getFullHP()) + " " + l.getMessage(Localized::HP) + ")\r\n\r\n";
					}
					else
					{
						rPlayer.setHP(rPlayer.getHP() + regen);
						logStr += l.getMessage(Localized::REST_REGEN) + " " + to_string(regen) + " " + l.getMessage(Localized::HEALTH_POINTS_GENITIVE)
							+ " (" + to_string(rPlayer.getHP()) + " / " + to_string(rPlayer.getFullHP()) + " " + l.getMessage(Localized::HP) + ")\r\n\r\n";
					}

					// Heal opponents across all arenas
					for (i = Cities::ROME; i < Cities::CITIES_NUMBER; i++)
					{
						// Get opponents
						vector<unique_ptr<NPC>>& opponents = game.getWorldMap().getCity(i).getArena().getGladiators();

						// Check every opponent
						int size = opponents.size();
						for (j = 0; j < size; j++)
						{
							NPC& opponent = *opponents[j];
							int hp = opponent.getHP();
							int fullHP = opponent.getFullHP();
							if (hp < fullHP)
							{
								if (hp + BASIC_REGEN < fullHP)
									opponent.setHP(hp + BASIC_REGEN);
								else
									opponent.setHP(fullHP);
							}
						}
					}
					rPlayer.setExhaustion(false);
				}
				else
					logStr += l.getMessage(Localized::REST_FULL) + "\r\n\r\n";
				SendMessage(hItems[EDIT_MESSAGES_LOG], WM_SETTEXT, 0, (LPARAM)logStr.c_str());
				SendMessage(hItems[EDIT_MESSAGES_LOG], EM_SCROLL, SB_BOTTOM, 0);
			}
			if ((HWND)lp == hItems[BUT_MAP])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Destroying all buttons
				for (HWND hItem : hSubMenuItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubMenuItems.clear();

				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_HIDE);

				game.setDisplayState(DisplayState::WORLD_MAP);
				game.setBackground(Game::Background::WORLD_MAP);
				updateWindow(hWnd);
				break;
			}
			if ((HWND)lp == hItems[BUT_MENU] || LOWORD(wp) == IDCANCEL)
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				game.getMenuManager().setMenu(new GameMenu(hWnd));
				game.setBackground(Game::Background::GAME_MENU);
				updateWindow(hWnd);
				break;
			}
		}
		break;

		case Game::Background::CITY_MENU_ARENA:
		{
			if ((HWND)lp == hSubItems[ARENA_BUT_FIGHT])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Hiding all windows
				ShowWindow(hItems[EDIT_MESSAGES_LOG], SW_HIDE);
				for (HWND hItem : hSubItems)
					ShowWindow(hItem, SW_HIDE);

				// Erasing previous sub menu items
				for (HWND hItem : hSubMenuItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubMenuItems.clear();

				// Creating new sub menu items
				hSubMenuItems.resize(ARENA_FIGHT_ITEM_NUMBER);
				hSubMenuItems[ARENA_FIGHT_STATIC_OPPONENTS] = CreateWindow("STATIC", l.getMessage(Localized::CHOOSE_OPPONENT).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				// Outputing player
				{
					for (i = ARENA_FIGHT_STATIC_NAME; i <= ARENA_FIGHT_STATIC_PLAYER_FAME; i++)
						hSubMenuItems[i] = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

					Player& rPlayer = game.getPlayer();
					
					if (rPlayer.getLeftHand())
					{
						if (rPlayer.getLeftHand()->getWeaponType() != Weapon::WeaponType::SHIELD)
						{
							ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_DAMAGE], SW_SHOW);
							ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_SHIELD_DEFENSE], SW_HIDE);
							ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_SHIELD_BLOCK_CHANCE], SW_HIDE);
						}
						else
						{
							ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_DAMAGE], SW_HIDE);
							ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_SHIELD_DEFENSE], SW_SHOW);
							ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_SHIELD_BLOCK_CHANCE], SW_SHOW);
						}
					}

					// Name
					buf = rPlayer.getName();
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_NAME], WM_SETTEXT, 0, (LPARAM)buf.c_str());

					// Level
					buf = l.getMessage(Localized::LEVEL) + ": " + to_string(rPlayer.getLevel());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEVEL], WM_SETTEXT, 0, (LPARAM)buf.c_str());

					// Right hand
					if (rPlayer.getRightHand())
					{
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)l.getWeaponTypeName(*rPlayer.getRightHand()).c_str());

						buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getRightHand()->getDamage()) + " (+" + to_string(rPlayer.getRightHand()->getDamageAddition()) + ")";
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
					}
					else
					{
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::EMPTY_HAND).c_str());

						ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_RIGHT_HAND_DAMAGE], SW_HIDE);
					}

					// Left hand
					if (rPlayer.getLeftHand())
					{
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)l.getWeaponTypeName(*rPlayer.getLeftHand()).c_str());

						if (rPlayer.getLeftHand()->getWeaponType() != Weapon::WeaponType::SHIELD)
						{
							buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getLeftHand()->getDamage()) + " (+" + to_string(rPlayer.getLeftHand()->getDamageAddition()) + ")";
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
						}
						else
						{
							buf = l.getMessage(Localized::BLOCK_DEFENSE) + ": " + to_string(rPlayer.getLeftHand()->getShieldDefPercentAddition());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_SHIELD_DEFENSE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
							buf = l.getMessage(Localized::BLOCK_CHANCE) + ": " + to_string(rPlayer.getLeftHand()->getShieldProbAddition());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_SHIELD_BLOCK_CHANCE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
						}
					}
					else
					{
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::EMPTY_HAND).c_str());

						ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_DAMAGE], SW_HIDE);
						ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_SHIELD_DEFENSE], SW_HIDE);
						ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_SHIELD_BLOCK_CHANCE], SW_HIDE);
					}

					// Health
					buf = l.getMessage(Localized::HEALTH) + ": " + to_string(rPlayer.getHP());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_HEALTH], WM_SETTEXT, 0, (LPARAM)buf.c_str());

					// Armour
					if (rPlayer.getArmour())
					{
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)l.getArmourTypeName(*rPlayer.getArmour()).c_str());

						buf = l.getMessage(Localized::ARMOUR_DEFENSE) + ": " + to_string(rPlayer.getArmour()->getDefense()) + " (+" + to_string(rPlayer.getArmour()->getDefAddition()) + ")";
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
					}

					// Stats
					buf = l.getMessage(Localized::STRENGTH) + ": " + to_string(rPlayer.getStrength());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_STRENGTH], WM_SETTEXT, 0, (LPARAM)buf.c_str());

					buf = l.getMessage(Localized::CONSTITUTION) + ": " + to_string(rPlayer.getConstitution());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)buf.c_str());

					buf = l.getMessage(Localized::DEXTERITY) + ": " + to_string(rPlayer.getDexterity());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_DEXTERITY], WM_SETTEXT, 0, (LPARAM)buf.c_str());

					buf = l.getMessage(Localized::INTELLIGENCE) + ": " + to_string(rPlayer.getIntelligence());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

					buf = l.getMessage(Localized::WISDOM) + ": " + to_string(rPlayer.getWisdom());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_WISDOM], WM_SETTEXT, 0, (LPARAM)buf.c_str());

					buf = l.getMessage(Localized::CHARISMA) + ": " + to_string(rPlayer.getCharisma());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_CHARISMA], WM_SETTEXT, 0, (LPARAM)buf.c_str());

					buf = l.getMessage(Localized::AGE) + ": " + to_string(rPlayer.getAge());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_AGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

					buf = l.getMessage(Localized::FAME) + ": " + to_string(rPlayer.getFame());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_FAME], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				}

				// Outputing opponents
				Arena& rArena = game.getWorldMap().getCurrentCity().getArena();
				for (i = ARENA_FIGHT_BUT_OPPONENT1; i <= ARENA_FIGHT_BUT_OPPONENT15; i++)
				{
					const NPC& rOpponent = *rArena.getGladiator(i);
					buf = l.getNPCName(rOpponent) + " (" + l.getMessage(Localized::LVL) + ": " + to_string(rOpponent.getLevel()) + ")";
					hSubMenuItems[i] = CreateWindow("BUTTON",
						buf.c_str(),
						WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_AUTORADIOBUTTON | BS_OWNERDRAW | BS_LEFTTEXT,
						0, 0, 0, 0, hWnd, 0, hInst, 0);
				}

				hSubMenuItems[ARENA_FIGHT_BUT_BACK] = CreateWindow("BUTTON", l.getMessage(Localized::BACK).c_str(), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubMenuItems[ARENA_FIGHT_BUT_FIGHT] = CreateWindow("BUTTON", l.getMessage(Localized::FIGHT).c_str(), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				for (i = ARENA_FIGHT_STATIC_NAME; i <= ARENA_FIGHT_STATIC_FAME; i++)
					ShowWindow(hSubMenuItems[i], SW_HIDE);

				game.setBackground(Game::Background::CITY_MENU_ARENA_FIGHT);

				updateWindow(hWnd);
			}
			if ((HWND)lp == hSubItems[ARENA_BUT_BET])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// TODO
			}

			if ((HWND)lp == hSubItems[ARENA_BUT_TRAIN])
			{
				Player& rPlayer = game.getPlayer();
				if (!rPlayer.getExhaustion())
				{
					// Experience is random number between 30 to 1/3 of experience needed for current lvlup
					int level = rPlayer.getLevel();
					int experience = rand() % (rPlayer.calculateExperienceForOneLevel(level + 1) / 3) + 30;

					rPlayer.gainExperience(experience);
					// Update log
					logStr += l.getMessage(Localized::TRAIN_GAIN) + " " + to_string(experience) + " " + l.getMessage(Localized::EXPERIENCE_GENITIVE) +
						" (" + to_string(rPlayer.getExperience()) + " / " + to_string(rPlayer.calculateExperienceForOneLevel(rPlayer.getLevel() + 1)) + ")\r\n\r\n";
					if (level < rPlayer.getLevel())
					{
						logStr += l.getMessage(Localized::LEVELED_UP) + " " +
							to_string(rPlayer.getLevel()) +
							" (" + to_string(rPlayer.getUnnassignedAttributes()) + " " + l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES_GENITIVE) + ")\r\n\r\n";
						playSound(SoundEnum::SOUND_LEVEL_UP);
					}

					rPlayer.setExhaustion(true);
				}
				else
					logStr += l.getMessage(Localized::TRAIN_NEED_REST) + "\r\n\r\n";
				SendMessage(hItems[EDIT_MESSAGES_LOG], WM_SETTEXT, 0, (LPARAM)logStr.c_str());
				SendMessage(hItems[EDIT_MESSAGES_LOG], EM_SCROLL, SB_BOTTOM, 0);
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
			}

			if ((HWND)lp == hSubItems[ARENA_BUT_BACK] || LOWORD(wp) == IDCANCEL)
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

				game.setBackground(Game::Background::CITY_MENU);

				updateWindow(hWnd);
				break;
			}
		}
		break;

		case Game::Background::CITY_MENU_QUEST:
		{
			if ((HWND)lp == hSubItems[QUEST_BUT_LANISTA])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Hiding all windows except messages log
				for (HWND hItem : hSubItems)
					ShowWindow(hItem, SW_HIDE);

				// Erasing previous sub menu items
				for (HWND hItem : hSubMenuItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubMenuItems.clear();

				// Creating new sub menu items
				hSubMenuItems.resize(LANISTA_ITEM_NUMBER);

				buf = l.getMessage(Localized::BUY_FREEDOM) + " (" + to_string(game.getPlayer().getInventory()->getItemQuantity(0)) + " / " + to_string(MONEY_NEEDED_FOR_FREEDOM) + ")";
				hSubMenuItems[LANISTA_BUT_BUY_FREEDOM] = CreateWindow("BUTTON", buf.c_str(),
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
					0, 0, 0, 0, hWnd, 0, hInst, 0);

				hSubMenuItems[LANISTA_BUT_TAKE_QUEST] = CreateWindow("BUTTON", l.getMessage(Localized::TAKE_QUEST).c_str(),
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
					0, 0, 0, 0, hWnd, 0, hInst, 0);

				buf = l.getMessage(Localized::ASK_FOR_PROMOTION) + " (" + to_string(CHARISMA_FOR_PROMOTION) + " " + l.getMessage(Localized::CHARISMA_GENITIVE) + ")";
				hSubMenuItems[LANISTA_BUT_ASK_FOR_PROMOTION] = CreateWindow("BUTTON", buf.c_str(),
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
					0, 0, 0, 0, hWnd, 0, hInst, 0);

				hSubMenuItems[LANISTA_BUT_BACK] = CreateWindow("BUTTON", l.getMessage(Localized::BACK).c_str(),
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
					0, 0, 0, 0, hWnd, 0, hInst, 0);

				// If player is free - hide button to buy freedom
				if (game.getPlayer().getFreedom())
					ShowWindow(hSubMenuItems[LANISTA_BUT_BUY_FREEDOM], SW_HIDE);

				// If player is already promoted - hide button to ask for promotion
				if (game.getWorldMap().getCurrentCity().getPromotionStatus())
					ShowWindow(hSubMenuItems[LANISTA_BUT_ASK_FOR_PROMOTION], SW_HIDE);

				game.setBackground(Game::Background::CITY_MENU_LANISTA);

				updateWindow(hWnd);
			}

			if ((HWND)lp == hSubItems[QUEST_BUT_TAVERN])
			{
				// TODO
			}

			if ((HWND)lp == hSubItems[QUEST_BUT_TALK_TO_PEOPLE])
			{
				// If quest wasn completed before and player has high enough level - start Shandar and Abhilasha quest
				if (!game.getWorldMap().getCurrentCity().getQuestCompletion() &&
					game.getPlayer().getLevel() >= game.getWorldMap().getCurrentCity().getLevel())
				{
					playSound(SoundEnum::SOUND_BUTTON_CLICK);
					// Destroying all windows
					for (HWND hItem : hSubItems)
						if (hItem != NULL)
							DestroyWindow(hItem);
					hSubItems.clear();
					ShowWindow(hItems[EDIT_MESSAGES_LOG], SW_HIDE);

					// Quest can only be started once
					game.getWorldMap().getCurrentCity().setQuestCompletion(true);

					// Display quest
					game.setBackground(Game::Background::CITY_MENU);
					game.getStoryScreen().displayScreen(hWnd, StoryScreen::Screen::QUEST_PERUGIA_SHANDAR);
					return;
				}
				else
				{
					// Update log
					logStr += l.getMessage(Localized::WALK_STREETS_NOTHING) + "\r\n\r\n";
					SendMessage(hItems[EDIT_MESSAGES_LOG], WM_SETTEXT, 0, (LPARAM)logStr.c_str());
					SendMessage(hItems[EDIT_MESSAGES_LOG], EM_SCROLL, SB_BOTTOM, 0);
				}
			}

			if ((HWND)lp == hSubItems[QUEST_BUT_BACK] || LOWORD(wp) == IDCANCEL)
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

				game.setBackground(Game::Background::CITY_MENU);

				updateWindow(hWnd);
				break;
			}
		}
		break;

		case Game::Background::CITY_MENU_LANISTA:
		{
			if ((HWND)lp == hSubMenuItems[LANISTA_BUT_BUY_FREEDOM])
			{
				Player& rPlayer = game.getPlayer();
				// If player has enough money - he becomes free
				if (rPlayer.getInventory()->getItemQuantity(0) >= MONEY_NEEDED_FOR_FREEDOM)
				{
					playSound(SoundEnum::SOUND_BUTTON_CLICK);

					rPlayer.setFreedom(true);
					rPlayer.getInventory()->removeItem(0, MONEY_NEEDED_FOR_FREEDOM);

					// Update log
					logStr += l.getMessage(Localized::GAINED_FREEDOM) + "\r\n\r\n";
					SendMessage(hItems[EDIT_MESSAGES_LOG], WM_SETTEXT, 0, (LPARAM)logStr.c_str());
					SendMessage(hItems[EDIT_MESSAGES_LOG], EM_SCROLL, SB_BOTTOM, 0);

					// Destroying all windows
					for (HWND hItem : hSubMenuItems)
						if (hItem != NULL)
							DestroyWindow(hItem);
					hSubMenuItems.clear();
					for (HWND hItem : hSubItems)
						if (hItem != NULL)
							DestroyWindow(hItem);
					hSubItems.clear();
					ShowWindow(hItems[EDIT_MESSAGES_LOG], SW_HIDE);
					game.setBackground(Game::Background::CITY_MENU);

					// Display story
					game.setProgressionStage(Game::Progression::GAINED_FREEDOM);
					game.getStoryScreen().displayScreen(hWnd, StoryScreen::Screen::GAINED_FREEDOM);
					return;
				}
			}

			if ((HWND)lp == hSubMenuItems[LANISTA_BUT_TAKE_QUEST])
			{
				// TODO
			}

			if ((HWND)lp == hSubMenuItems[LANISTA_BUT_ASK_FOR_PROMOTION])
			{
				static int n = 0;
				if (!game.getWorldMap().getCurrentCity().getPromotionStatus())
				{
					if (game.getPlayer().getCharisma() >= CHARISMA_FOR_PROMOTION)
					{
						game.getWorldMap().getCurrentCity().setPromotion(true);
						// Update log
						logStr += l.getMessage(Localized::YOU_HAVE_BEEN_PROMOTED) + "\r\n\r\n";
						SendMessage(hItems[EDIT_MESSAGES_LOG], WM_SETTEXT, 0, (LPARAM)logStr.c_str());
						SendMessage(hItems[EDIT_MESSAGES_LOG], EM_SCROLL, SB_BOTTOM, 0);
						n = 0;
					}
					else if (n == 1)
					{
						// Update log
						logStr += l.getMessage(Localized::NOT_ENOUGH_CHARISMA_FOR_PROMOTION_AGRESSIVELY) + "\r\n\r\n";
						SendMessage(hItems[EDIT_MESSAGES_LOG], WM_SETTEXT, 0, (LPARAM)logStr.c_str());
						SendMessage(hItems[EDIT_MESSAGES_LOG], EM_SCROLL, SB_BOTTOM, 0);
						n++;
					}
					else if (n == 3)
					{
						ShowWindow(hSubMenuItems[LANISTA_BUT_ASK_FOR_PROMOTION], SW_HIDE);
						game.updateBackground();
						updateWindow(hWnd);
					}
					else
					{
						// Update log
						logStr += l.getMessage(Localized::NOT_ENOUGH_CHARISMA_FOR_PROMOTION) + "\r\n\r\n";
						SendMessage(hItems[EDIT_MESSAGES_LOG], WM_SETTEXT, 0, (LPARAM)logStr.c_str());
						SendMessage(hItems[EDIT_MESSAGES_LOG], EM_SCROLL, SB_BOTTOM, 0);
						n++;
					}
				}
				else if (n == 1)
				{
					// Update log
					logStr += l.getMessage(Localized::ALREADY_PROMOTED_AGRESSIVELY) + "\r\n\r\n";
					SendMessage(hItems[EDIT_MESSAGES_LOG], WM_SETTEXT, 0, (LPARAM)logStr.c_str());
					SendMessage(hItems[EDIT_MESSAGES_LOG], EM_SCROLL, SB_BOTTOM, 0);
					n++;
				}
				else if (n == 3)
				{
					ShowWindow(hSubMenuItems[LANISTA_BUT_ASK_FOR_PROMOTION], SW_HIDE);
					game.updateBackground();
					updateWindow(hWnd);
				}
				else
				{
					// Update log
					logStr += l.getMessage(Localized::ALREADY_PROMOTED) + "\r\n\r\n";
					SendMessage(hItems[EDIT_MESSAGES_LOG], WM_SETTEXT, 0, (LPARAM)logStr.c_str());
					SendMessage(hItems[EDIT_MESSAGES_LOG], EM_SCROLL, SB_BOTTOM, 0);
					n++;
				}
			}

			if ((HWND)lp == hSubMenuItems[LANISTA_BUT_BACK] || LOWORD(wp) == IDCANCEL)
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Destroying all buttons
				for (HWND hItem : hSubMenuItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubMenuItems.clear();

				// Showing menu buttons
				for (HWND hItem : hSubItems)
					ShowWindow(hItem, SW_SHOW);
				ShowWindow(hItems[EDIT_MESSAGES_LOG], SW_SHOW);

				game.setBackground(Game::Background::CITY_MENU_QUEST);

				updateWindow(hWnd);
			}
		}
		break;

		case Game::Background::CITY_MENU_ARENA_FIGHT:
		{
			// Selecting opponent
			{
				for (i = ARENA_FIGHT_BUT_OPPONENT1; i <= ARENA_FIGHT_BUT_OPPONENT15; i++)
				{
					if ((HWND)lp == hSubMenuItems[i])
						outputOpponent(hWnd, i);
				}
			}

			// Starting fight
			if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_FIGHT])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				if (selectedOpponent != -1 && game.getPlayer().getHP() > 30)
				{
					// Destroying all buttons
					for (HWND hItem : hSubMenuItems)
						if (hItem != NULL)
							DestroyWindow(hItem);
					hSubMenuItems.clear();

					game.setDisplayState(DisplayState::FIGHTING);
					game.setBackground(Game::Background::FIGHTING_ARENA);
					updateWindow(hWnd);

					// Fight
					City& currentCity = game.getWorldMap().getCurrentCity();
					Arena& currentArena = currentCity.getArena();

					FightStatus fightStatus = game.getFighting().fight(
						hWnd,
						game.getPlayer(),
						*currentArena.getGladiator(selectedOpponent)
					);

					// Fight result
					switch (fightStatus)
					{
					// In case of opponent's death he's replaced on arena with new gladiator
					case FightStatus::OPPONENT_LOST:
						currentArena.changeGladiator(selectedOpponent, currentCity.getLevel());
						// Sort NPCs by level and fame
						sort(currentArena.getGladiators().begin(), currentArena.getGladiators().end(), compareNPC);
						break;
					case FightStatus::OPPONNENT_SURRENDERED:
						break;
					case FightStatus::PLAYER_SURRENDERED:
						break;
					case FightStatus::PLAYER_LOST:
						break;
					}

					// Return to city
					game.setBackground(Game::Background::CITY_MENU);
					selectedOpponent = -1;

					// If autosave is enabled - save game
					game.saveToFile(game.getAutoSaveStatus());
					break;
				}
				else if (selectedOpponent == -1)
					MessageBox(hWnd, l.getMessage(Localized::HAVE_TO_CHOOSE_OPPONENT).c_str(), l.getMessage(Localized::OPPONENT_NOT_SELECTED).c_str(), MB_OK | MB_ICONINFORMATION);
				else
					MessageBox(hWnd, l.getMessage(Localized::FIGHT_NEED_REST).c_str(), l.getMessage(Localized::FIGHT_INJURED).c_str(), MB_OK | MB_ICONINFORMATION);
			}

			// Return
			if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_BACK] || LOWORD(wp) == IDCANCEL)
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Destroying all buttons
				for (HWND hItem : hSubMenuItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubMenuItems.clear();

				// Showing menu buttons
				for (HWND hItem : hSubItems)
					ShowWindow(hItem, SW_SHOW);
				ShowWindow(hItems[EDIT_MESSAGES_LOG], SW_SHOW);

				game.setBackground(Game::Background::CITY_MENU_ARENA);
				selectedOpponent = -1;

				updateWindow(hWnd);
			}
		}
		break;

		case Game::Background::CITY_MENU_ARENA_BET:
		{
			// TODO
		}
		break;

		case Game::Background::CITY_MENU_MARKET:
		{
			// Player's items buttons
			for (i = MARKET_BUT_INVENTORY_ITEM1; i <= MARKET_BUT_INVENTORY_ITEM14; i++)
			{
				if ((HWND)lp == hSubItems[i])
				{
					if (i == selectedItem) return;

					playSound(SoundEnum::SOUND_BUTTON_CLICK);
					if (selectedItem != -1)
					{
						int id = selectedItem;
						selectedItem = i;
						updateWindow(hSubItems[id]);
					}
					else
						selectedItem = i;
					auto itemPair = (*game.getPlayer().getInventory())[i - MARKET_BUT_INVENTORY_ITEM1];
					unique_ptr<Item>& rItem = itemPair.first;

					if (rItem->getItemType() != Item::ItemType::GOLD)
					{
						ShowWindow(hSubItems[MARKET_BUT_SELL], SW_SHOW);
						ShowWindow(hSubItems[MARKET_BUT_BUY], SW_HIDE);
					}
					else
					{
						ShowWindow(hSubItems[MARKET_BUT_SELL], SW_HIDE);
						ShowWindow(hSubItems[MARKET_BUT_BUY], SW_HIDE);
					}

					outputMarketItem(hWnd, rItem, itemPair.second);
					manageMarketItems(hWnd, i);
				}
			}

			// Player's equipment buttons
			for (i = MARKET_BUT_RIGHT_HAND; i <= MARKET_BUT_ARMOUR; i++)
			{
				if ((HWND)lp == hSubItems[i])
				{
					if (i == selectedItem) return;

					Player& rPlayer = game.getPlayer();
					if ((i == MARKET_BUT_RIGHT_HAND && !rPlayer.getRightHand()) ||
						(i == MARKET_BUT_LEFT_HAND && !rPlayer.getLeftHand()) ||
						(i == MARKET_BUT_ARMOUR && !rPlayer.getArmour()))
						return;

					playSound(SoundEnum::SOUND_BUTTON_CLICK);
					if (selectedItem != -1)
					{
						int id = selectedItem;
						selectedItem = i;
						updateWindow(hSubItems[id]);
					}
					else
						selectedItem = i;

					unique_ptr<Weapon> pRightHand;
					unique_ptr<Weapon> pLeftHand;
					unique_ptr<Armour> pArmour;

					if (rPlayer.getRightHand())
						pRightHand = make_unique<Weapon>(Weapon(*rPlayer.getRightHand()));
					if (rPlayer.getLeftHand())
						pLeftHand = make_unique<Weapon>(Weapon(*rPlayer.getLeftHand()));
					if (rPlayer.getArmour())
						pArmour = make_unique<Armour>(Armour(*rPlayer.getArmour()));

					unique_ptr<Item> pItem;

					switch (i)
					{
					case MARKET_BUT_RIGHT_HAND: pItem = move(pRightHand); break;
					case MARKET_BUT_LEFT_HAND: pItem = move(pLeftHand); break;
					case MARKET_BUT_ARMOUR: pItem = move(pArmour); break;
					}

					ShowWindow(hSubItems[MARKET_BUT_SELL], SW_HIDE);
					ShowWindow(hSubItems[MARKET_BUT_BUY], SW_HIDE);

					outputMarketItem(hWnd, pItem);
					manageMarketItems(hWnd, i);
				}
			}

			// Trader's items button
			for (i = MARKET_BUT_TRADER_ITEM1; i <= MARKET_BUT_TRADER_ITEM14; i++)
			{
				if ((HWND)lp == hSubItems[i])
				{
					if (i == selectedItem) return;
					playSound(SoundEnum::SOUND_BUTTON_CLICK);
					if (selectedItem != -1)
					{
						int id = selectedItem;
						selectedItem = i;
						updateWindow(hSubItems[id]);
					}
					else
						selectedItem = i;
					auto itemPair = (*game.getWorldMap().getCurrentCity().getTrader().getInventory())[i - MARKET_BUT_TRADER_ITEM1];
					unique_ptr<Item>& rItem = itemPair.first;

					if (rItem->getItemType() != Item::ItemType::GOLD)
					{
						ShowWindow(hSubItems[MARKET_BUT_SELL], SW_HIDE);
						ShowWindow(hSubItems[MARKET_BUT_BUY], SW_SHOW);
					}
					else
					{
						ShowWindow(hSubItems[MARKET_BUT_SELL], SW_HIDE);
						ShowWindow(hSubItems[MARKET_BUT_BUY], SW_HIDE);
					}
					outputMarketItem(hWnd, rItem, itemPair.second);
				}
			}

			// Button buy
			if ((HWND)lp == hSubItems[MARKET_BUT_BUY])
			{
				playSound(SoundEnum::SOUND_GOLD);
				// Get inventories
				Inventory& rPlayerInventory = *game.getPlayer().getInventory();
				if (rPlayerInventory.size() < MAX_INVENTORY_SIZE)
				{
					Inventory& rTraderInventory = *game.getWorldMap().getCurrentCity().getTrader().getInventory();
					// Get item
					auto itemPair = rTraderInventory[selectedItem - MARKET_BUT_TRADER_ITEM1];
					unique_ptr<Item>& rItem = itemPair.first;
					// Get cost
					int cost = rItem->getPrice();

					// Check if player has enough gold
					int playerGold = rPlayerInventory.getItemQuantity(0); // 0 is ID for gold

					if (cost <= playerGold)
					{
						// Get item ID
						int id = rItem->getID();
						// Make copy of item
						unique_ptr<Item> itemCopy = rTraderInventory.getItem(id)->clone(); // TODO: update item stats
						itemCopy->calculatePrice(game.getPlayer().getCharisma(), true);
						// Remove item from trader
						rTraderInventory.removeItem(id);
						// Add item copy to player
						rPlayerInventory.addItem(move(itemCopy));
						// Remove gold from player
						rPlayerInventory.removeItem(0, cost); // 0 is ID for gold
						// Add gold to trader
						rTraderInventory.addItem(make_unique<Item>(Item(Item::ItemType::GOLD)), cost);

						// Update windows
						selectedItem = -1;
						for (int i = MARKET_STAT_ITEM_TYPE; i <= MARKET_STAT_ITEM_TOTAL_VALUE; i++)
							ShowWindow(hSubItems[i], SW_HIDE);
						ShowWindow(hSubItems[MARKET_BUT_SELL], SW_HIDE);
						ShowWindow(hSubItems[MARKET_BUT_BUY], SW_HIDE);
						ShowWindow(hSubItems[MARKET_BUT_TRADER_ITEM1 + rTraderInventory.size()], SW_HIDE);
						ShowWindow(hSubItems[MARKET_STAT_TRADER_ITEM1 + rTraderInventory.size()], SW_HIDE);
						game.updateBackground();
						updateWindow(hWnd);
					}
					else
						MessageBox(hWnd, l.getMessage(Localized::PLAYER_DOESNT_HAVE_GOLD).c_str(), l.getMessage(Localized::CANT_BUY_ITEM).c_str(), MB_OK | MB_ICONINFORMATION);
				}
				else
					MessageBox(hWnd, l.getMessage(Localized::PLAYER_DOESNT_HAVE_STORAGE).c_str(), l.getMessage(Localized::CANT_BUY_ITEM).c_str(), MB_OK | MB_ICONINFORMATION);
				
			}

			// Button sell
			if ((HWND)lp == hSubItems[MARKET_BUT_SELL])
			{
				playSound(SoundEnum::SOUND_GOLD);
				// Get inventories
				Inventory& rPlayerInventory = *game.getPlayer().getInventory();
				Inventory& rTraderInventory = *game.getWorldMap().getCurrentCity().getTrader().getInventory();
				// Get item
				auto itemPair = rPlayerInventory[selectedItem - MARKET_BUT_INVENTORY_ITEM1];
				unique_ptr<Item>& rItem = itemPair.first;
				// Get cost
				int cost = rItem->getPrice();

				// Check if trader has enough gold
				int traderGold = rTraderInventory.getItemQuantity(0); // 0 is ID for gold

				if (cost <= traderGold)
				{
					// Remove item
					rPlayerInventory.removeItem(rItem->getID());
					// Remove gold from trader
					rTraderInventory.removeItem(0, cost); // 0 is ID for gold
					// Add gold to player
					rPlayerInventory.addItem(make_unique<Item>(Item(Item::ItemType::GOLD)), cost);

					// Update windows
					selectedItem = -1;
					for (int i = MARKET_STAT_ITEM_TYPE; i <= MARKET_STAT_ITEM_TOTAL_VALUE; i++)
						ShowWindow(hSubItems[i], SW_HIDE);
					ShowWindow(hSubItems[MARKET_BUT_SELL], SW_HIDE);
					ShowWindow(hSubItems[MARKET_BUT_BUY], SW_HIDE);
					ShowWindow(hSubItems[MARKET_BUT_EQUIP_ITEM], SW_HIDE);
					ShowWindow(hSubItems[MARKET_BUT_UNEQUIP_ITEM], SW_HIDE);
					ShowWindow(hSubItems[MARKET_BUT_DESTROY_ITEM], SW_HIDE);
					ShowWindow(hSubItems[MARKET_BUT_INVENTORY_ITEM1 + rPlayerInventory.size()], SW_HIDE);
					ShowWindow(hSubItems[MARKET_STAT_INVENTORY_ITEM1 + rPlayerInventory.size()], SW_HIDE);
					game.updateBackground();

					if (game.getProgressionStage() == Game::Progression::FIRST_VICTORY && // If current story stage is that player's saving up money for freedom
						game.getPlayer().getInventory()->getItemQuantity(0) >= MONEY_NEEDED_FOR_FREEDOM) // If player has enough money for freedom
					{
						// Destroying all buttons
						for (HWND hItem : hSubItems)
							if (hItem != NULL)
								DestroyWindow(hItem);
						hSubItems.clear();

						game.setProgressionStage(Game::Progression::ENOUGH_MONEY_FOR_FREEDOM);
						game.getPlayer().getInventory()->removeItem(0, MONEY_NEEDED_FOR_FREEDOM);
						game.getStoryScreen().displayScreen(hWnd, StoryScreen::Screen::STOLEN_MONEY);

						game.setBackground(Game::Background::CITY_MENU);
						selectedItem = -1;
						return;
					}

					updateWindow(hWnd);
				}
				else
					MessageBox(hWnd, l.getMessage(Localized::TRADER_DOESNT_HAVE_GOLD).c_str(), l.getMessage(Localized::CANT_SELL_ITEM).c_str(), MB_OK | MB_ICONINFORMATION);
			}

			// Equip button
			if ((HWND)lp == hSubItems[MARKET_BUT_EQUIP_ITEM])
			{
				Player& rPlayer = game.getPlayer();

				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Adjust item index for button index
				selectedItem -= MARKET_BUT_INVENTORY_ITEM1;

				Inventory& rInventory = *rPlayer.getInventory();
				int itemID = rInventory[selectedItem].first->getID();

				rPlayer.equipItemFromInventory(itemID);

				ShowWindow(hSubItems[MARKET_BUT_EQUIP_ITEM], SW_HIDE);
				ShowWindow(hSubItems[MARKET_BUT_UNEQUIP_ITEM], SW_HIDE);
				ShowWindow(hSubItems[MARKET_BUT_DESTROY_ITEM], SW_HIDE);
				ShowWindow(hSubItems[MARKET_BUT_SELL], SW_HIDE);

				int size = rInventory.size();
				for (i = size; i < MAX_INVENTORY_SIZE; i++)
				{
					ShowWindow(hSubItems[MARKET_BUT_INVENTORY_ITEM1 + i], SW_HIDE);
					ShowWindow(hSubItems[MARKET_STAT_INVENTORY_ITEM1 + i], SW_HIDE);
				}
				for (i = 0; i < size; i++)
				{
					updateWindow(hSubItems[MARKET_BUT_INVENTORY_ITEM1 + i]);
					updateWindow(hSubItems[MARKET_STAT_INVENTORY_ITEM1 + i]);
				}

				game.updateBackground();
				updateWindow(hWnd);
			}

			// Unequip button
			if ((HWND)lp == hSubItems[MARKET_BUT_UNEQUIP_ITEM])
			{
				Player& rPlayer = game.getPlayer();
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Adjust item index for button index
				selectedItem -= MARKET_BUT_RIGHT_HAND;
				/*
				selectedItem
				0 -- rightHand
				1 -- leftHand
				2 -- armour
				*/
				int itemID;
				switch (selectedItem)
				{
				case 0: itemID = rPlayer.getRightHand()->getID(); break; 
				case 1: itemID = rPlayer.getLeftHand()->getID(); break; 
				case 2: itemID = rPlayer.getArmour()->getID(); break; 
				default: throw out_of_range("Wrong item ID"); break;
				}

				rPlayer.unequipItem(itemID);

				ShowWindow(hSubItems[MARKET_BUT_UNEQUIP_ITEM], SW_HIDE);

				int size = rPlayer.getInventory()->size();
				for (i = 0; i < size; i++)
				{
					ShowWindow(hSubItems[MARKET_BUT_INVENTORY_ITEM1 + i], SW_SHOW);
					updateWindow(hSubItems[MARKET_BUT_INVENTORY_ITEM1 + i]);
					ShowWindow(hSubItems[MARKET_STAT_INVENTORY_ITEM1 + i], SW_SHOW);
					updateWindow(hSubItems[MARKET_STAT_INVENTORY_ITEM1 + i]);
				}

				game.updateBackground();
				updateWindow(hWnd);
			}

			// Destroy button
			if ((HWND)lp == hSubItems[MARKET_BUT_DESTROY_ITEM])
			{
				Player& rPlayer = game.getPlayer();
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Adjust item index for button index
				selectedItem -= MARKET_BUT_INVENTORY_ITEM1;

				Inventory& rInventory = *rPlayer.getInventory();
				int itemID = rInventory[selectedItem].first->getID();
				int quantity = rInventory.getItemQuantity(itemID);
				rInventory.removeItem(itemID, quantity);

				for (int i = MARKET_STAT_ITEM_TYPE; i <= MARKET_STAT_ITEM_TOTAL_VALUE; i++)
					ShowWindow(hSubItems[i], SW_HIDE);
				ShowWindow(hSubItems[MARKET_BUT_EQUIP_ITEM], SW_HIDE);
				ShowWindow(hSubItems[MARKET_BUT_UNEQUIP_ITEM], SW_HIDE);
				ShowWindow(hSubItems[MARKET_BUT_DESTROY_ITEM], SW_HIDE);
				ShowWindow(hSubItems[MARKET_BUT_SELL], SW_HIDE);

				int size = rInventory.size();
				for (i = size; i < MAX_INVENTORY_SIZE; i++)
				{
					ShowWindow(hSubItems[MARKET_BUT_INVENTORY_ITEM1 + i], SW_HIDE);
					ShowWindow(hSubItems[MARKET_STAT_INVENTORY_ITEM1 + i], SW_HIDE);
				}
				for (i = 0; i < size; i++)
				{
					updateWindow(hSubItems[MARKET_BUT_INVENTORY_ITEM1 + i]);
					updateWindow(hSubItems[MARKET_STAT_INVENTORY_ITEM1 + i]);
				}

				game.updateBackground();
			}

			// Back button
			if ((HWND)lp == hSubItems[MARKET_BUT_BACK] || LOWORD(wp) == IDCANCEL)
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

				game.setBackground(Game::Background::CITY_MENU);
				selectedItem = -1;

				updateWindow(hWnd);
				break;
			}
		}
		break;

		case Game::Background::CITY_MENU_CHARACTER:
		{
			Player& rPlayer = game.getPlayer();
			int i;
			double average = ATTRIBUTE_MAX_DIFFERENCE;

			// 1. Player attributes

			// Plus buttons
			// Strength
			if ((HWND)lp == hSubItems[CHARACTER_BUT_STRENGTH_PLUS])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				pas.unnassignedAttributes--;
				pas.strength++;
				pas.updateMaxHP();
				average += pas.calculateAverage();

				// Buttons appearance
				ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_MINUS], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_SHOW);
				if (pas.unnassignedAttributes == 0)
				{
					for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_CHARISMA_PLUS; i++)
						ShowWindow(hSubItems[i], SW_HIDE);
				}
				else
				{
					if (pas.strength <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_HIDE);
					if (pas.constitution <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_HIDE);
					if (pas.dexterity <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_HIDE);
					if (pas.intelligence <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_HIDE);
					if (pas.wisdom <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_HIDE);
					if (pas.charisma <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_HIDE);
				}
				// Text
				// Update weapon damage
				if (rPlayer.getRightHand())
				{
					rPlayer.getRightHand()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getRightHand()->getDamage()) + " (+" + to_string(rPlayer.getRightHand()->getDamageAddition()) + ")";
					SendMessage(hSubItems[CHARACTER_STAT_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				}
				if (rPlayer.getLeftHand())
				{
					rPlayer.getLeftHand()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getLeftHand()->getDamage()) + " (+" + to_string(rPlayer.getLeftHand()->getDamageAddition()) + ")";
					SendMessage(hSubItems[CHARACTER_STAT_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				}
				// Update armour defense
				if (rPlayer.getArmour())
				{
					rPlayer.getArmour()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::ARMOUR_DEFENSE) + ": " + to_string(rPlayer.getArmour()->getDefense()) + " (+" + to_string(rPlayer.getArmour()->getDefAddition()) + ")";
					SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				}
				// Update stats
				buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				buf = l.getMessage(Localized::STRENGTH) + ": " + to_string(pas.strength);
				SendMessage(hSubItems[CHARACTER_STAT_STRENGTH], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				buf = l.getMessage(Localized::HEALTH) + ": (" + to_string(pas.hp) + " / " + to_string(pas.fullHP) + " )";
				SendMessage(hSubItems[CHARACTER_STAT_HEALTH], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				game.updateBackground();
			}
			// Constitution
			if ((HWND)lp == hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				pas.unnassignedAttributes--;
				pas.constitution++;
				pas.updateMaxHP();
				average += pas.calculateAverage();

				// Buttons appearance
				ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_MINUS], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_SHOW);
				if (pas.unnassignedAttributes == 0)
				{
					for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_CHARISMA_PLUS; i++)
						ShowWindow(hSubItems[i], SW_HIDE);
				}
				else
				{
					if (pas.strength <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_HIDE);
					if (pas.constitution <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_HIDE);
					if (pas.dexterity <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_HIDE);
					if (pas.intelligence <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_HIDE);
					if (pas.wisdom <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_HIDE);
					if (pas.charisma <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_HIDE);
				}
				// Text
				buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				buf = l.getMessage(Localized::CONSTITUTION) + ": " + to_string(pas.constitution);
				SendMessage(hSubItems[CHARACTER_STAT_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				buf = l.getMessage(Localized::HEALTH) + ": (" + to_string(pas.hp) + " / " + to_string(pas.fullHP) + " )";
				SendMessage(hSubItems[CHARACTER_STAT_HEALTH], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				game.updateBackground();
			}
			// Dexterity
			if ((HWND)lp == hSubItems[CHARACTER_BUT_DEXTERITY_PLUS])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				pas.unnassignedAttributes--;
				pas.dexterity++;
				average += pas.calculateAverage();

				// Buttons appearance
				ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_MINUS], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_SHOW);
				if (pas.unnassignedAttributes == 0)
				{
					for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_CHARISMA_PLUS; i++)
						ShowWindow(hSubItems[i], SW_HIDE);
				}
				else
				{
					if (pas.strength <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_HIDE);
					if (pas.constitution <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_HIDE);
					if (pas.dexterity <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_HIDE);
					if (pas.intelligence <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_HIDE);
					if (pas.wisdom <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_HIDE);
					if (pas.charisma <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_HIDE);
				}
				// Text
				// Update weapon damage
				if (rPlayer.getRightHand())
				{
					rPlayer.getRightHand()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getRightHand()->getDamage()) + " (+" + to_string(rPlayer.getRightHand()->getDamageAddition()) + ")";
					SendMessage(hSubItems[CHARACTER_STAT_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				}
				if (rPlayer.getLeftHand())
				{
					rPlayer.getLeftHand()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getLeftHand()->getDamage()) + " (+" + to_string(rPlayer.getLeftHand()->getDamageAddition()) + ")";
					SendMessage(hSubItems[CHARACTER_STAT_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				}
				// Update armour defense
				if (rPlayer.getArmour())
				{
					rPlayer.getArmour()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::ARMOUR_DEFENSE) + ": " + to_string(rPlayer.getArmour()->getDefense()) + " (+" + to_string(rPlayer.getArmour()->getDefAddition()) + ")";
					SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				}
				// Update stats
				buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				buf = l.getMessage(Localized::DEXTERITY) + ": " + to_string(pas.dexterity);
				SendMessage(hSubItems[CHARACTER_STAT_DEXTERITY], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				game.updateBackground();
			}
			// Intelligence
			if ((HWND)lp == hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				pas.unnassignedAttributes--;
				pas.intelligence++;
				average += pas.calculateAverage();

				// Buttons appearance
				ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_MINUS], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_SHOW);
				if (pas.unnassignedAttributes == 0)
				{
					for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_CHARISMA_PLUS; i++)
						ShowWindow(hSubItems[i], SW_HIDE);
				}
				else
				{
					if (pas.strength <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_HIDE);
					if (pas.constitution <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_HIDE);
					if (pas.dexterity <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_HIDE);
					if (pas.intelligence <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_HIDE);
					if (pas.wisdom <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_HIDE);
					if (pas.charisma <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_HIDE);
				}
				// Text
				buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				buf = l.getMessage(Localized::INTELLIGENCE) + ": " + to_string(pas.intelligence);
				SendMessage(hSubItems[CHARACTER_STAT_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				game.updateBackground();
			}
			// Wisdom
			if ((HWND)lp == hSubItems[CHARACTER_BUT_WISDOM_PLUS])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				pas.unnassignedAttributes--;
				pas.wisdom++;
				average += pas.calculateAverage();

				// Buttons appearance
				ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_MINUS], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_SHOW);
				if (pas.unnassignedAttributes == 0)
				{
					for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_CHARISMA_PLUS; i++)
						ShowWindow(hSubItems[i], SW_HIDE);
				}
				else
				{
					if (pas.strength <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_HIDE);
					if (pas.constitution <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_HIDE);
					if (pas.dexterity <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_HIDE);
					if (pas.intelligence <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_HIDE);
					if (pas.wisdom <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_HIDE);
					if (pas.charisma <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_HIDE);
				}
				// Text
				buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				buf = l.getMessage(Localized::WISDOM) + ": " + to_string(pas.wisdom);
				SendMessage(hSubItems[CHARACTER_STAT_WISDOM], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				game.updateBackground();
			}
			// Charisma
			if ((HWND)lp == hSubItems[CHARACTER_BUT_CHARISMA_PLUS])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				pas.unnassignedAttributes--;
				pas.charisma++;
				average += pas.calculateAverage();

				// Buttons appearance
				ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_MINUS], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_SHOW);
				if (pas.unnassignedAttributes == 0)
				{
					for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_CHARISMA_PLUS; i++)
						ShowWindow(hSubItems[i], SW_HIDE);
				}
				else
				{
					if (pas.strength <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_HIDE);
					if (pas.constitution <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_HIDE);
					if (pas.dexterity <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_HIDE);
					if (pas.intelligence <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_HIDE);
					if (pas.wisdom <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_HIDE);
					if (pas.charisma <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_HIDE);
				}
				// Text
				buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				buf = l.getMessage(Localized::CHARISMA) + ": " + to_string(pas.charisma);
				SendMessage(hSubItems[CHARACTER_STAT_CHARISMA], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				// Update item's price
				if (rPlayer.getRightHand())
					rPlayer.getRightHand()->calculatePrice(pas.charisma, true);
				if (rPlayer.getLeftHand())
					rPlayer.getLeftHand()->calculatePrice(pas.charisma, true);
				if (rPlayer.getArmour())
					rPlayer.getArmour()->calculatePrice(pas.charisma, true);
				Inventory& rInventory = *rPlayer.getInventory();
				for (i = 0; i < rInventory.size(); i++)
					if (rInventory[i].first->getItemType() != Item::ItemType::GOLD)
						rInventory[i].first->calculatePrice(pas.charisma, true);

				game.updateBackground();
			}

			// Minus buttons
			// Strength
			if ((HWND)lp == hSubItems[CHARACTER_BUT_STRENGTH_MINUS])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				pas.unnassignedAttributes++;
				pas.strength--;
				pas.updateMaxHP();
				average += pas.calculateAverage();

				// Buttons appearance
				if (pas.strength <= average)
					ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_SHOW);
				if (pas.strength == rPlayer.getStrength())
				{
					ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_MINUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.constitution > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.dexterity > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.intelligence > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.wisdom > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.charisma > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_HIDE);
					game.updateBackground();
				}

				// Apply changes, reset changes
				if (pas.strength == rPlayer.getStrength() &&
					pas.constitution == rPlayer.getConstitution() &&
					pas.dexterity == rPlayer.getDexterity() &&
					pas.intelligence == rPlayer.getIntelligence() &&
					pas.wisdom == rPlayer.getWisdom() ||
					pas.charisma == rPlayer.getCharisma()
					)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_HIDE);
					ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_HIDE);
					game.updateBackground();
				}

				// Text
				// Update weapon damage
				if (rPlayer.getRightHand())
				{
					rPlayer.getRightHand()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getRightHand()->getDamage()) + " (+" + to_string(rPlayer.getRightHand()->getDamageAddition()) + ")";
					SendMessage(hSubItems[CHARACTER_STAT_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				}
				if (rPlayer.getLeftHand())
				{
					rPlayer.getLeftHand()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getLeftHand()->getDamage()) + " (+" + to_string(rPlayer.getLeftHand()->getDamageAddition()) + ")";
					SendMessage(hSubItems[CHARACTER_STAT_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				}
				// Update armour defense
				if (rPlayer.getArmour())
				{
					rPlayer.getArmour()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::ARMOUR_DEFENSE) + ": " + to_string(rPlayer.getArmour()->getDefense()) + " (+" + to_string(rPlayer.getArmour()->getDefAddition()) + ")";
					SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				}
				// Update stats
				buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				buf = l.getMessage(Localized::STRENGTH) + ": " + to_string(pas.strength);
				SendMessage(hSubItems[CHARACTER_STAT_STRENGTH], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				buf = l.getMessage(Localized::HEALTH) + ": (" + to_string(pas.hp) + " / " + to_string(pas.fullHP) + " )";
				SendMessage(hSubItems[CHARACTER_STAT_HEALTH], WM_SETTEXT, 0, (LPARAM)buf.c_str());
			}
			// Constitution
			if ((HWND)lp == hSubItems[CHARACTER_BUT_CONSTITUTION_MINUS])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				pas.unnassignedAttributes++;
				pas.constitution--;
				pas.updateMaxHP();
				average += pas.calculateAverage();

				// Buttons appearance
				if (pas.constitution <= average)
					ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_SHOW);
				if (pas.constitution == rPlayer.getConstitution())
				{
					ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_MINUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.strength > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.dexterity > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.intelligence > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.wisdom > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.charisma > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_HIDE);
					game.updateBackground();
				}

				// Apply changes, reset changes
				if (pas.strength == rPlayer.getStrength() &&
					pas.constitution == rPlayer.getConstitution() &&
					pas.dexterity == rPlayer.getDexterity() &&
					pas.intelligence == rPlayer.getIntelligence() &&
					pas.wisdom == rPlayer.getWisdom() &&
					pas.charisma == rPlayer.getCharisma()
					)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_HIDE);
					ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_HIDE);
					game.updateBackground();
				}

				// Text
				buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				buf = l.getMessage(Localized::CONSTITUTION) + ": " + to_string(pas.constitution);
				SendMessage(hSubItems[CHARACTER_STAT_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				buf = l.getMessage(Localized::HEALTH) + ": (" + to_string(pas.hp) + " / " + to_string(pas.fullHP) + " )";
				SendMessage(hSubItems[CHARACTER_STAT_HEALTH], WM_SETTEXT, 0, (LPARAM)buf.c_str());
			}
			// Dexterity
			if ((HWND)lp == hSubItems[CHARACTER_BUT_DEXTERITY_MINUS])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				pas.unnassignedAttributes++;
				pas.dexterity--;
				average += pas.calculateAverage();

				// Buttons appearance
				if (pas.dexterity <= average)
					ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_SHOW);
				if (pas.dexterity == rPlayer.getDexterity())
				{
					ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_MINUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.strength > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.constitution > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.intelligence > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.wisdom > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.charisma > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_HIDE);
					game.updateBackground();
				}

				// Apply changes, reset changes
				if (pas.strength == rPlayer.getStrength() &&
					pas.constitution == rPlayer.getConstitution() &&
					pas.dexterity == rPlayer.getDexterity() &&
					pas.intelligence == rPlayer.getIntelligence() &&
					pas.wisdom == rPlayer.getWisdom() &&
					pas.charisma == rPlayer.getCharisma()
					)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_HIDE);
					ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_HIDE);
					game.updateBackground();
				}

				// Text
				// Update weapon damage
				if (rPlayer.getRightHand())
				{
					rPlayer.getRightHand()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getRightHand()->getDamage()) + " (+" + to_string(rPlayer.getRightHand()->getDamageAddition()) + ")";
					SendMessage(hSubItems[CHARACTER_STAT_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				}
				if (rPlayer.getLeftHand())
				{
					rPlayer.getLeftHand()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getLeftHand()->getDamage()) + " (+" + to_string(rPlayer.getLeftHand()->getDamageAddition()) + ")";
					SendMessage(hSubItems[CHARACTER_STAT_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				}
				// Update armour defense
				if (rPlayer.getArmour())
				{
					rPlayer.getArmour()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::ARMOUR_DEFENSE) + ": " + to_string(rPlayer.getArmour()->getDefense()) + " (+" + to_string(rPlayer.getArmour()->getDefAddition()) + ")";
					SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				}
				// Update stats
				buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				buf = l.getMessage(Localized::DEXTERITY) + ": " + to_string(pas.dexterity);
				SendMessage(hSubItems[CHARACTER_STAT_DEXTERITY], WM_SETTEXT, 0, (LPARAM)buf.c_str());
			}
			// Intelligence
			if ((HWND)lp == hSubItems[CHARACTER_BUT_INTELLIGENCE_MINUS])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				pas.unnassignedAttributes++;
				pas.intelligence--;
				average += pas.calculateAverage();

				// Buttons appearance
				if (pas.intelligence <= average)
					ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_SHOW);
				if (pas.intelligence == rPlayer.getIntelligence())
				{
					ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_MINUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.strength > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.constitution > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.dexterity > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.wisdom > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.charisma > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_HIDE);
					game.updateBackground();
				}

				// Apply changes, reset changes
				if (pas.strength == rPlayer.getStrength() &&
					pas.constitution == rPlayer.getConstitution() &&
					pas.dexterity == rPlayer.getDexterity() &&
					pas.intelligence == rPlayer.getIntelligence() &&
					pas.wisdom == rPlayer.getWisdom() &&
					pas.charisma == rPlayer.getCharisma()
					)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_HIDE);
					ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_HIDE);
					game.updateBackground();
				}

				// Text
				buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				buf = l.getMessage(Localized::INTELLIGENCE) + ": " + to_string(pas.intelligence);
				SendMessage(hSubItems[CHARACTER_STAT_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
			}
			// Wisdom
			if ((HWND)lp == hSubItems[CHARACTER_BUT_WISDOM_MINUS])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				pas.unnassignedAttributes++;
				pas.wisdom--;
				average += pas.calculateAverage();

				// Buttons appearance
				if (pas.wisdom <= average)
					ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_SHOW);
				if (pas.wisdom == rPlayer.getWisdom())
				{
					ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_MINUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.strength > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.constitution > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.dexterity > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.intelligence > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.charisma > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_HIDE);
					game.updateBackground();
				}

				// Apply changes, reset changes
				if (pas.strength == rPlayer.getStrength() &&
					pas.constitution == rPlayer.getConstitution() &&
					pas.dexterity == rPlayer.getDexterity() &&
					pas.intelligence == rPlayer.getIntelligence() &&
					pas.wisdom == rPlayer.getWisdom() &&
					pas.charisma == rPlayer.getCharisma()
					)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_HIDE);
					ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_HIDE);
					game.updateBackground();
				}

				// Text
				buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				buf = l.getMessage(Localized::WISDOM) + ": " + to_string(pas.wisdom);
				SendMessage(hSubItems[CHARACTER_STAT_WISDOM], WM_SETTEXT, 0, (LPARAM)buf.c_str());
			}
			// Charisma
			if ((HWND)lp == hSubItems[CHARACTER_BUT_CHARISMA_MINUS])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				pas.unnassignedAttributes++;
				pas.charisma--;
				average += pas.calculateAverage();

				// Buttons appearance
				if (pas.charisma <= average)
					ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_SHOW);
				if (pas.charisma == rPlayer.getCharisma())
				{
					ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_MINUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.strength > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.constitution > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.dexterity > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.intelligence > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.wisdom > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_HIDE);
					game.updateBackground();
				}

				// Apply changes, reset changes
				if (pas.strength == rPlayer.getStrength() &&
					pas.constitution == rPlayer.getConstitution() &&
					pas.dexterity == rPlayer.getDexterity() &&
					pas.intelligence == rPlayer.getIntelligence() &&
					pas.wisdom == rPlayer.getWisdom() &&
					pas.charisma == rPlayer.getCharisma()
					)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_HIDE);
					ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_HIDE);
					game.updateBackground();
				}

				// Text
				buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				buf = l.getMessage(Localized::CHARISMA) + ": " + to_string(pas.charisma);
				SendMessage(hSubItems[CHARACTER_STAT_CHARISMA], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				// Update item's price
				if (rPlayer.getRightHand())
					rPlayer.getRightHand()->calculatePrice(pas.charisma, true);
				if (rPlayer.getLeftHand())
					rPlayer.getLeftHand()->calculatePrice(pas.charisma, true);
				if (rPlayer.getArmour())
					rPlayer.getArmour()->calculatePrice(pas.charisma, true);
				Inventory& rInventory = *rPlayer.getInventory();
				for (i = 0; i < rInventory.size(); i++)
					if (rInventory[i].first->getItemType() != Item::ItemType::GOLD)
						rInventory[i].first->calculatePrice(pas.charisma, true);
			}

			// Reset changes
			if ((HWND)lp == hSubItems[CHARACTER_BUT_RESET_CHANGES])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Update stats
				pas.unnassignedAttributes = rPlayer.getUnnassignedAttributes();
				pas.strength = rPlayer.getStrength();
				pas.constitution = rPlayer.getConstitution();
				pas.dexterity = rPlayer.getDexterity();
				pas.intelligence = rPlayer.getIntelligence();
				pas.wisdom = rPlayer.getWisdom();
				pas.charisma = rPlayer.getCharisma();
				average += pas.calculateAverage();

				// Button appearance
				if (pas.strength <= average)
					ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_SHOW);
				else
					ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_HIDE);
				if (pas.constitution <= average)
					ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_SHOW);
				else
					ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_HIDE);
				if (pas.dexterity <= average)
					ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_SHOW);
				else
					ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_HIDE);
				if (pas.intelligence <= average)
					ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_SHOW);
				else
					ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_HIDE);
				if (pas.wisdom <= average)
					ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_SHOW);
				else
					ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_HIDE);
				if (pas.charisma <= average)
					ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_SHOW);
				else
					ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_HIDE);

				for (i = CHARACTER_BUT_STRENGTH_MINUS; i <= CHARACTER_BUT_CHARISMA_MINUS; i++)
					ShowWindow(hSubItems[i], SW_HIDE);

				ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_HIDE);

				// Text
				// Update weapons stats
				if (rPlayer.getRightHand())
				{
					rPlayer.getRightHand()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getRightHand()->getDamage()) + " (+" + to_string(rPlayer.getRightHand()->getDamageAddition()) + ")";
					SendMessage(hSubItems[CHARACTER_STAT_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
					rPlayer.getRightHand()->calculatePrice(pas.charisma, true);
				}
				if (rPlayer.getLeftHand())
				{
					rPlayer.getLeftHand()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getLeftHand()->getDamage()) + " (+" + to_string(rPlayer.getLeftHand()->getDamageAddition()) + ")";
					SendMessage(hSubItems[CHARACTER_STAT_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
					rPlayer.getLeftHand()->calculatePrice(pas.charisma, true);
				}
				// Update armour stats
				if (rPlayer.getArmour())
				{
					rPlayer.getArmour()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::ARMOUR_DEFENSE) + ": " + to_string(rPlayer.getArmour()->getDefense()) + " (+" + to_string(rPlayer.getArmour()->getDefAddition()) + ")";
					SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
					rPlayer.getArmour()->calculatePrice(pas.charisma, true);
				}
				// Update stats
				buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				buf = l.getMessage(Localized::STRENGTH) + ": " + to_string(pas.strength);
				SendMessage(hSubItems[CHARACTER_STAT_STRENGTH], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				buf = l.getMessage(Localized::CONSTITUTION) + ": " + to_string(pas.constitution);
				SendMessage(hSubItems[CHARACTER_STAT_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				buf = l.getMessage(Localized::DEXTERITY) + ": " + to_string(pas.dexterity);
				SendMessage(hSubItems[CHARACTER_STAT_DEXTERITY], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				buf = l.getMessage(Localized::INTELLIGENCE) + ": " + to_string(pas.intelligence);
				SendMessage(hSubItems[CHARACTER_STAT_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				buf = l.getMessage(Localized::WISDOM) + ": " + to_string(pas.wisdom);
				SendMessage(hSubItems[CHARACTER_STAT_WISDOM], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				buf = l.getMessage(Localized::CHARISMA) + ": " + to_string(pas.charisma);
				SendMessage(hSubItems[CHARACTER_STAT_CHARISMA], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				buf = l.getMessage(Localized::HEALTH) + ": (" + to_string(pas.hp) + " / " + to_string(pas.fullHP) + " )";
				SendMessage(hSubItems[CHARACTER_STAT_HEALTH], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				game.updateBackground();
			}

			// Apply changes
			if ((HWND)lp == hSubItems[CHARACTER_BUT_APPLY_CHANGES])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Update player stats
				rPlayer.setUnnassignedAttributes(pas.unnassignedAttributes);
				rPlayer.setStrength(pas.strength);
				rPlayer.setConstitution(pas.constitution);
				rPlayer.setDexterity(pas.dexterity);
				rPlayer.setIntelligence(pas.intelligence);
				rPlayer.setWisdom(pas.wisdom);
				rPlayer.setCharisma(pas.charisma);
				rPlayer.setHP(pas.hp);
				rPlayer.setFullHP(pas.fullHP);

				// Button appearance
				for (i = CHARACTER_BUT_STRENGTH_MINUS; i <= CHARACTER_BUT_CHARISMA_MINUS; i++)
					ShowWindow(hSubItems[i], SW_HIDE);

				ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_HIDE);

				game.updateBackground();
			}

			// 2. Inventory
			for (i = CHARACTER_BUT_INVENTORY_ITEM1; i <= CHARACTER_BUT_ARMOUR; i++)
			{
				if ((HWND)lp == hSubItems[i])
				{
					playSound(SoundEnum::SOUND_BUTTON_CLICK);
					selectedItem = i;
					manageInventory(hWnd, i);
					break;
				}
			}

			// Unequip item
			if ((HWND)lp == hSubItems[CHARACTER_BUT_INVENTORY_UNEQUIP_ITEM])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Adjust item index for button index
				selectedItem -= CHARACTER_BUT_RIGHT_HAND;
				/*
				selectedItem
				0 -- rightHand
				1 -- leftHand
				2 -- armour
				*/
				int itemID;
				switch (selectedItem)
				{
				case 0:
					itemID = rPlayer.getRightHand()->getID();
					ShowWindow(hSubItems[CHARACTER_STAT_RIGHT_HAND_DAMAGE], SW_HIDE);
					ShowWindow(hSubItems[CHARACTER_STAT_RIGHT_HAND_STRENGTH_SCALE], SW_HIDE);
					ShowWindow(hSubItems[CHARACTER_STAT_RIGHT_HAND_DEXTERITY_SCALE], SW_HIDE);
					break;

				case 1:
					itemID = rPlayer.getLeftHand()->getID();
					ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_DAMAGE], SW_HIDE);
					ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_STRENGTH_SCALE], SW_HIDE);
					ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_DEXTERITY_SCALE], SW_HIDE);
					break;

				case 2:
					itemID = rPlayer.getArmour()->getID();
					ShowWindow(hSubItems[CHARACTER_STAT_ARMOUR_DEFENSE], SW_HIDE);
					ShowWindow(hSubItems[CHARACTER_STAT_ARMOUR_STRENGTH_SCALE], SW_HIDE);
					ShowWindow(hSubItems[CHARACTER_STAT_ARMOUR_DEXTERITY_SCALE], SW_HIDE);
					ShowWindow(hSubItems[CHARACTER_STAT_ARMOUR_ABILITY], SW_HIDE);
					break;

				default: throw out_of_range("Wrong item ID"); break;
				}

				rPlayer.unequipItem(itemID);

				ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_UNEQUIP_ITEM], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_INSPECT_ITEM], SW_HIDE);

				int size = rPlayer.getInventory()->size();
				for (i = 0; i < size; i++)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_ITEM1 + i], SW_SHOW);
					updateWindow(hSubItems[CHARACTER_BUT_INVENTORY_ITEM1 + i]);
					ShowWindow(hSubItems[CHARACTER_STAT_INVENTORY_ITEM1_NAME + i], SW_SHOW);
					updateWindow(hSubItems[CHARACTER_STAT_INVENTORY_ITEM1_NAME + i]);
				}

				game.updateBackground();
				updateWindow(hWnd);
			}

			// Equip item
			if ((HWND)lp == hSubItems[CHARACTER_BUT_INVENTORY_EQUIP_ITEM])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Adjust item index for button index
				selectedItem -= CHARACTER_BUT_INVENTORY_ITEM1;

				Inventory& rInventory = *rPlayer.getInventory();
				int itemID = rInventory[selectedItem].first->getID();

				rPlayer.equipItemFromInventory(itemID);

				ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_EQUIP_ITEM], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_INSPECT_ITEM], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_DESTROY_ITEM], SW_HIDE);

				int size = rInventory.size();
				for (i = size; i < MAX_INVENTORY_SIZE; i++)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_ITEM1 + i], SW_HIDE);
					ShowWindow(hSubItems[CHARACTER_STAT_INVENTORY_ITEM1_NAME + i], SW_HIDE);
				}
				for (i = 0; i < size; i++)
				{
					updateWindow(hSubItems[CHARACTER_BUT_INVENTORY_ITEM1 + i]);
					updateWindow(hSubItems[CHARACTER_STAT_INVENTORY_ITEM1_NAME + i]);
				}

				game.updateBackground();
				updateWindow(hWnd);
			}

			// Inspect item
			if ((HWND)lp == hSubItems[CHARACTER_BUT_INVENTORY_INSPECT_ITEM])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_EQUIP_ITEM], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_UNEQUIP_ITEM], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_INSPECT_ITEM], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_DESTROY_ITEM], SW_HIDE);
				game.updateBackground();

				// 1. Determine which item to inspect
				unique_ptr<Item> pItem;
				int quantity = 1;

				/*
				selectedItem
				less than 0 --item from inventory
				0 --rightHand
				1 --leftHand
				2 --armour
				*/
				switch (selectedItem - CHARACTER_BUT_RIGHT_HAND)
				{
				case 0: // rightHand
					pItem = make_unique<Weapon>(*rPlayer.getRightHand());
					break;

				case 1: // leftHand
					pItem = make_unique<Weapon>(*rPlayer.getLeftHand());
					break;

				case 2: // armour
					pItem = make_unique<Armour>(*rPlayer.getArmour());
					break;

				default: // item from inventory
					
					auto itemPair = (*game.getPlayer().getInventory())[selectedItem - CHARACTER_BUT_INVENTORY_ITEM1];

					if (auto weapon = dynamic_cast<Weapon*>(itemPair.first.get()))
						pItem = make_unique<Weapon>(*weapon);
					else if (auto armour = dynamic_cast<Armour*>(itemPair.first.get()))
						pItem = make_unique<Armour>(*armour);
					else
						pItem = make_unique<Item>(*itemPair.first);

					quantity = (*game.getPlayer().getInventory()).getItemQuantity(pItem->getID());
					break;
				}
				// 2. Output item based on type
				inspectItem(hWnd, move(pItem), quantity);
			}

			// Destroy item
			if ((HWND)lp == hSubItems[CHARACTER_BUT_INVENTORY_DESTROY_ITEM])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Adjust item index for button index
				selectedItem -= CHARACTER_BUT_INVENTORY_ITEM1;

				Inventory& rInventory = *rPlayer.getInventory();
				int itemID = rInventory[selectedItem].first->getID();
				int quantity = rInventory.getItemQuantity(itemID);
				rInventory.removeItem(itemID, quantity);

				ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_EQUIP_ITEM], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_INSPECT_ITEM], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_DESTROY_ITEM], SW_HIDE);

				int size = rInventory.size();
				for (i = size; i < MAX_INVENTORY_SIZE; i++)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_ITEM1 + i], SW_HIDE);
					ShowWindow(hSubItems[CHARACTER_STAT_INVENTORY_ITEM1_NAME + i], SW_HIDE);
				}
				for (i = 0; i < size; i++)
				{
					updateWindow(hSubItems[CHARACTER_BUT_INVENTORY_ITEM1 + i]);
					updateWindow(hSubItems[CHARACTER_STAT_INVENTORY_ITEM1_NAME + i]);
				}

				game.updateBackground();
			}

			// Back button
			if ((HWND)lp == hSubItems[CHARACTER_BUT_BACK] || LOWORD(wp) == IDCANCEL)
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				// Destroying all buttons
				for (HWND hItem : hSubItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubItems.clear();

				// Changing back weapon damage and armour defense based on stats in case player left without applying changes
				if (rPlayer.getRightHand())
					rPlayer.getRightHand()->update(rPlayer.getStrength(), rPlayer.getDexterity());
				if (rPlayer.getLeftHand())
					rPlayer.getLeftHand()->update(rPlayer.getStrength(), rPlayer.getDexterity());
				if (rPlayer.getArmour())
					rPlayer.getArmour()->update(rPlayer.getStrength(), rPlayer.getDexterity());

				// Showing main menu buttons
				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_SHOW);

				game.setBackground(Game::Background::CITY_MENU);
				selectedItem = -1;

				updateWindow(hWnd);
				break;
			}
		}
		break;
		}
	break;

	case WM_LBUTTONDOWN:
		if (game.getBackground() == Game::Background::CITY_MENU_CHARACTER)
		{
			if (
				ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_UNEQUIP_ITEM], SW_HIDE) ||
				ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_EQUIP_ITEM], SW_HIDE) ||
				ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_INSPECT_ITEM], SW_HIDE) ||
				ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_DESTROY_ITEM], SW_HIDE)
				)
			{
				ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_UNEQUIP_ITEM], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_EQUIP_ITEM], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_INSPECT_ITEM], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_DESTROY_ITEM], SW_HIDE);
				game.updateBackground();
			}
		}
		if (game.getBackground() == Game::Background::CITY_MENU_MARKET)
		{
			if (
				ShowWindow(hSubItems[MARKET_BUT_UNEQUIP_ITEM], SW_HIDE) ||
				ShowWindow(hSubItems[MARKET_BUT_EQUIP_ITEM], SW_HIDE) ||
				ShowWindow(hSubItems[MARKET_BUT_DESTROY_ITEM], SW_HIDE)
				)
			{
				ShowWindow(hSubItems[MARKET_BUT_UNEQUIP_ITEM], SW_HIDE);
				ShowWindow(hSubItems[MARKET_BUT_EQUIP_ITEM], SW_HIDE);
				ShowWindow(hSubItems[MARKET_BUT_DESTROY_ITEM], SW_HIDE);
				game.updateBackground();
			}
		}
		break;
	}	
}

bool CityMenu::stylizeWindow(HWND hWnd, UINT m, WPARAM wp, LPARAM lp, LRESULT& result)
{
	switch (m)
	{
		case WM_DRAWITEM:
		{
			LPDRAWITEMSTRUCT item = (LPDRAWITEMSTRUCT)lp;
			HDC hdc = item->hDC;

			GetClassName(item->hwndItem, str, sizeof(str) / sizeof(str[0]));

			// Set text font and background
			SelectObject(hdc, game.getFont(Game::FontSize::MEDIUM));
			SetBkMode(hdc, TRANSPARENT);

			// Get text
			int len = GetWindowTextLength(item->hwndItem);
			buf.resize(len + 1); // Resize buffer to contain button text
			GetWindowTextA(item->hwndItem, &buf[0], len + 1); // Write text into buffer

			SetTextColor(hdc, COLOR_WHITE); // Set basic text color

			// Checking window type to draw it using correct styles
			if (game.getBackground() == Game::Background::CITY_MENU)
			{
				// Fill background
				if (item->hwndItem == hItems[STAT_CITY_NAME])
					FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_ROMAN_RED_PUSHED));
				else
				{
					if (item->itemState & ODS_SELECTED)
						FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_ROMAN_RED_PUSHED)); 
					else
						FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_ROMAN_RED));
				}
				SelectObject(hdc, game.getFont(Game::FontSize::LARGE));
				DrawTextA(item->hDC, buf.c_str(), len, &item->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER); // Display text
				DrawEdge(hdc, &item->rcItem, EDGE_SUNKEN, BF_RECT); // Draw edge
				return true;
			}
			else if (game.getBackground() == Game::Background::CITY_MENU_CHARACTER)
			{
				// Portrait
				if (item->hwndItem == hSubItems[CHARACTER_STAT_PORTRAIT])
				{
					// Selecting image for button based on item type
					const string DIRECTORY = "Data/Image/Portrait/Player/",
						FORMAT = ".bmp";
					string path = DIRECTORY + to_string(game.getPlayer().getPortaitIndex()) + FORMAT;

					// Select image
					hBackgroundImage = (HBITMAP)LoadImage(0, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
					// Filling background with selected image
					hBackgroundBrush = CreatePatternBrush(hBackgroundImage);
					FillRect(item->hDC, &item->rcItem, hBackgroundBrush);
					// Drawing edge
					DrawEdge(item->hDC, &item->rcItem, EDGE_RAISED, BF_RECT);
					return true;
				}
				// Equipment
				if (item->hwndItem == hSubItems[CHARACTER_BUT_RIGHT_HAND])
				{
					drawEquippedItem(hWnd, item, 0); // 0 -- right hand
					return true;
				}
				if (item->hwndItem == hSubItems[CHARACTER_BUT_LEFT_HAND])
				{
					drawEquippedItem(hWnd, item, 1); // 1 -- left hand
					return true;
				}
				if (item->hwndItem == hSubItems[CHARACTER_BUT_ARMOUR])
				{
					drawEquippedItem(hWnd, item, 2); // 2 -- armour
					return true;
				}
				// Inventory
				for (int i = CHARACTER_BUT_INVENTORY_ITEM1; i <= CHARACTER_BUT_INVENTORY_ITEM14; i++)
				{
					if (item->hwndItem == hSubItems[i])
					{
						drawInventoryItem(hWnd, item, *game.getPlayer().getInventory(), i - CHARACTER_BUT_INVENTORY_ITEM1);
						return true;
					}
				}
			}
			else if (game.getBackground() == Game::Background::CITY_MENU_ARENA_FIGHT)
			{
				if (selectedOpponent != -1)
				{
					if (item->hwndItem == hSubMenuItems[selectedOpponent])
					{
						FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_ROMAN_RED_PUSHED));
						DrawTextA(item->hDC, buf.c_str(), len, &item->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
						DrawEdge(hdc, &item->rcItem, EDGE_RAISED, BF_RECT);
						return true;
					}
					if (item->hwndItem == hSubMenuItems[ARENA_FIGHT_STATIC_PORTRAIT])
					{
						// Selecting image for button based on item type
						const string DIRECTORY = "Data/Image/Portrait/Gladiator/",
							FORMAT = ".bmp";
						string path = DIRECTORY + to_string(game.getWorldMap().getCurrentCity().getArena().getGladiator(selectedOpponent)->getPortraitIndex()) + FORMAT;

						// Select image
						hBackgroundImage = (HBITMAP)LoadImage(0, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
						// Filling background with selected image
						hBackgroundBrush = CreatePatternBrush(hBackgroundImage);
						FillRect(item->hDC, &item->rcItem, hBackgroundBrush);
						// Drawing edge
						DrawEdge(item->hDC, &item->rcItem, EDGE_RAISED, BF_RECT);
						return true;
					}
				}
				if (item->hwndItem == hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_PORTRAIT])
				{
					// Selecting image for button based on item type
					const string DIRECTORY = "Data/Image/Portrait/Player/",
						FORMAT = ".bmp";
					string path = DIRECTORY + to_string(game.getPlayer().getPortaitIndex()) + "_small" + FORMAT;

					// Select image
					hBackgroundImage = (HBITMAP)LoadImage(0, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
					// Filling background with selected image
					hBackgroundBrush = CreatePatternBrush(hBackgroundImage);
					FillRect(item->hDC, &item->rcItem, hBackgroundBrush);
					// Drawing edge
					DrawEdge(item->hDC, &item->rcItem, EDGE_RAISED, BF_RECT);
					return true;
				}
			}
			else if (game.getBackground() == Game::Background::CITY_MENU_MARKET)
			{
				int i;
				bool isPushed = false;
				// Player inventory
				for (i = MARKET_BUT_INVENTORY_ITEM1; i <= MARKET_BUT_INVENTORY_ITEM14; i++)
				{
					if (item->hwndItem == hSubItems[i])
					{
						if (selectedItem != -1 && item->hwndItem == hSubItems[selectedItem])
							isPushed = true;

						drawInventoryItem(hWnd, item, *game.getPlayer().getInventory(), i - MARKET_BUT_INVENTORY_ITEM1, isPushed);
						return true;
					}
				}
				// Equipment
				for (i = MARKET_BUT_RIGHT_HAND; i <= MARKET_BUT_ARMOUR; i++)
				{
					if (item->hwndItem == hSubItems[i])
					{
						if (selectedItem != -1 && item->hwndItem == hSubItems[selectedItem])
							isPushed = true;

						drawEquippedItem(hWnd, item, i - MARKET_BUT_RIGHT_HAND, isPushed);
						return true;
					}
				}
				// Trader inventory
				for (i = MARKET_BUT_TRADER_ITEM1; i <= MARKET_BUT_TRADER_ITEM14; i++)
				{
					if (item->hwndItem == hSubItems[i])
					{
						if (selectedItem != -1 && item->hwndItem == hSubItems[selectedItem])
							isPushed = true;
						
						drawInventoryItem(hWnd, item, *game.getWorldMap().getCurrentCity().getTrader().getInventory(), i - MARKET_BUT_TRADER_ITEM1, isPushed);
						return true;
					}
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

void CityMenu::outputMarketItem(HWND hWnd, unique_ptr<Item>& rItem, int quantity)
{
	Player& rPlayer = game.getPlayer();
	for (int i = MARKET_STAT_ITEM_TYPE; i <= MARKET_STAT_ITEM_TOTAL_VALUE; i++)
		ShowWindow(hSubItems[i], SW_HIDE);
	game.updateBackground();

	// Item type
	ShowWindow(hSubItems[MARKET_STAT_ITEM_TYPE], SW_SHOW);
	SendMessage(hSubItems[MARKET_STAT_ITEM_TYPE], WM_SETTEXT, 0, (LPARAM)l.getItemTypeName(*rItem).c_str());
	switch (rItem->getItemType())
	{
	case Item::ItemType::WEAPON:
		if (auto weapon = dynamic_cast<Weapon*>(rItem.get()))
		{
			// Update weapon for player's stats
			weapon->update(rPlayer.getStrength(), rPlayer.getDexterity());
			// Item tier
			ShowWindow(hSubItems[MARKET_STAT_ITEM_TIER], SW_SHOW);
			buf = l.getMessage(Localized::ITEM_TIER) + " " + to_string(weapon->getTier());
			SendMessage(hSubItems[MARKET_STAT_ITEM_TIER], WM_SETTEXT, 0, (LPARAM)buf.c_str());

			switch (weapon->getWeaponType())
			{
			default: // Default for every weapon type except shield
				// Damage
				ShowWindow(hSubItems[MARKET_STAT_ITEM_DAMAGE], SW_SHOW);
				buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(weapon->getDamage()) + " (+" + to_string(weapon->getDamageAddition()) + ")";
				SendMessage(hSubItems[MARKET_STAT_ITEM_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				// Strength scale
				ShowWindow(hSubItems[MARKET_STAT_ITEM_STRENGTH_SCALE], SW_SHOW);
				buf = l.getMessage(Localized::STRENGTH_SCALE) + ": " + to_string(weapon->getStrengthAdditionPercentage()) + "%";
				SendMessage(hSubItems[MARKET_STAT_ITEM_STRENGTH_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				// Dexterity scale
				ShowWindow(hSubItems[MARKET_STAT_ITEM_DEXTERITY_SCALE], SW_SHOW);
				buf = l.getMessage(Localized::DEXTERITY_SCALE) + ": " + to_string(weapon->getDexterityAdditionPercentage()) + "%";
				SendMessage(hSubItems[MARKET_STAT_ITEM_DEXTERITY_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				break;
			case Weapon::WeaponType::SHIELD:
				// Block chance
				ShowWindow(hSubItems[MARKET_STAT_ITEM_SHIELD_CHANCE], SW_SHOW);
				buf = l.getMessage(Localized::BLOCK_CHANCE) + ": " + to_string(weapon->getShieldProbAddition()) + "%";
				SendMessage(hSubItems[MARKET_STAT_ITEM_SHIELD_CHANCE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

				// Block defense
				ShowWindow(hSubItems[MARKET_STAT_ITEM_SHIELD_BLOCK_DEFENSE], SW_SHOW);
				buf = l.getMessage(Localized::BLOCK_DEFENSE) + ": " + to_string(weapon->getShieldDefPercentAddition()) + "%";
				SendMessage(hSubItems[MARKET_STAT_ITEM_SHIELD_BLOCK_DEFENSE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				break;
			}
		}
		break;

	case Item::ItemType::ARMOUR:
		if (auto armour = dynamic_cast<Armour*>(rItem.get()))
		{
			// Update armour for player's stats
			armour->update(rPlayer.getStrength(), rPlayer.getDexterity());
			// Item tier
			ShowWindow(hSubItems[MARKET_STAT_ITEM_TIER], SW_SHOW);
			buf = l.getMessage(Localized::ITEM_TIER) + " " + to_string(armour->getTier());
			SendMessage(hSubItems[MARKET_STAT_ITEM_TIER], WM_SETTEXT, 0, (LPARAM)buf.c_str());

			// Defense
			ShowWindow(hSubItems[MARKET_STAT_ITEM_DEFENSE], SW_SHOW);
			buf = l.getMessage(Localized::ARMOUR_DEFENSE) + ": " + to_string(armour->getDefense()) + " (+" + to_string(armour->getDefAddition()) + ")";
			SendMessage(hSubItems[MARKET_STAT_ITEM_DEFENSE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

			// Strength scale
			ShowWindow(hSubItems[MARKET_STAT_ITEM_STRENGTH_SCALE], SW_SHOW);
			buf = l.getMessage(Localized::STRENGTH_SCALE) + ": " + to_string(armour->getStrengthAdditionPercentage()) + "%";
			SendMessage(hSubItems[MARKET_STAT_ITEM_STRENGTH_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

			// Dexterity scale
			ShowWindow(hSubItems[MARKET_STAT_ITEM_DEXTERITY_SCALE], SW_SHOW);
			buf = l.getMessage(Localized::DEXTERITY_SCALE) + ": " + to_string(armour->getDexterityAdditionPercentage()) + "%";
			SendMessage(hSubItems[MARKET_STAT_ITEM_DEXTERITY_SCALE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

			switch (armour->getArmourType())
			{
			case Armour::ArmourType::LIGHT:
				// Evasion chance
				ShowWindow(hSubItems[MARKET_STAT_ITEM_ARMOUR_ABILITY], SW_SHOW);
				buf = l.getMessage(Localized::EVASION_CHANCE) + ": " + to_string(armour->getEvasionProbAddition()) + "%";
				SendMessage(hSubItems[MARKET_STAT_ITEM_ARMOUR_ABILITY], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				break;
			case Armour::ArmourType::HEAVY:
				// Stun resistance chance
				ShowWindow(hSubItems[MARKET_STAT_ITEM_ARMOUR_ABILITY], SW_SHOW);
				buf = l.getMessage(Localized::STUN_RESISTANCE_CHANCE) + ": " + to_string(armour->getStunProbSubtraction()) + "%";
				SendMessage(hSubItems[MARKET_STAT_ITEM_ARMOUR_ABILITY], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				break;
			}
		}
		break;
	}

	ShowWindow(hSubItems[MARKET_STAT_ITEM_VALUE], SW_SHOW);
	buf = l.getMessage(Localized::VALUE) + ": " + to_string(rItem->getValue());
	SendMessage(hSubItems[MARKET_STAT_ITEM_VALUE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

	ShowWindow(hSubItems[MARKET_STAT_ITEM_QUANTITY], SW_SHOW);
	buf = l.getMessage(Localized::QUANTITY) + ": " + to_string(quantity);
	SendMessage(hSubItems[MARKET_STAT_ITEM_QUANTITY], WM_SETTEXT, 0, (LPARAM)buf.c_str());

	ShowWindow(hSubItems[MARKET_STAT_ITEM_TOTAL_VALUE], SW_SHOW);
	buf = l.getMessage(Localized::TOTAL_VALUE) + ": " + to_string(rItem->getPrice() * quantity);
	SendMessage(hSubItems[MARKET_STAT_ITEM_TOTAL_VALUE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
}

void CityMenu::outputOpponent(HWND hWnd, int n)
{
	RECT rect;
	int i;
	// Update button selection
	// If previously button was selected it must be updated
	if (selectedOpponent != -1)
	{
		GetWindowRect(hSubMenuItems[selectedOpponent], &rect);
		MapWindowPoints(HWND_DESKTOP, hWnd, (LPPOINT)&rect, 2);
		InvalidateRect(hWnd, &rect, 1);
	}
	if (selectedOpponent == n)
		return;

	playSound(SoundEnum::SOUND_BUTTON_CLICK);
	selectedOpponent = n;

	GetWindowRect(hSubMenuItems[selectedOpponent], &rect);
	MapWindowPoints(HWND_DESKTOP, hWnd, (LPPOINT)&rect, 2);
	InvalidateRect(hWnd, &rect, 1);

	// Update portrait
	updateWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PORTRAIT]);

	// Outputing opponent's information
	NPC& rOpponent = *game.getWorldMap().getCurrentCity().getArena().getGladiator(selectedOpponent);

	for (i = ARENA_FIGHT_STATIC_NAME; i <= ARENA_FIGHT_STATIC_LEFT_HAND_TYPE; i++)
		ShowWindow(hSubMenuItems[i], SW_SHOW);
	if (rOpponent.getLeftHand())
	{
		if (rOpponent.getLeftHand()->getWeaponType() != Weapon::WeaponType::SHIELD)
			ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], SW_SHOW);
		else
		{
			ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_SHIELD_DEFENSE], SW_SHOW);
			ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_SHIELD_BLOCK_CHANCE], SW_SHOW);
		}
	}
	for (i = ARENA_FIGHT_STATIC_HEALTH; i <= ARENA_FIGHT_STATIC_FAME; i++)
		ShowWindow(hSubMenuItems[i], SW_SHOW);

	// Name
	buf = l.getNPCName(rOpponent);
	SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_NAME], WM_SETTEXT, 0, (LPARAM)buf.c_str());

	// Level
	buf = l.getMessage(Localized::LEVEL) + ": " + to_string(rOpponent.getLevel());
	SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEVEL], WM_SETTEXT, 0, (LPARAM)buf.c_str());

	// Right hand
	if (rOpponent.getRightHand())
	{
		SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)l.getWeaponTypeName(*rOpponent.getRightHand()).c_str());

		buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rOpponent.getRightHand()->getDamage()) + " (+" + to_string(rOpponent.getRightHand()->getDamageAddition()) + ")";
		SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
	}

	// Left hand
	if (rOpponent.getLeftHand())
	{
		SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)l.getWeaponTypeName(*rOpponent.getLeftHand()).c_str());

		if (rOpponent.getLeftHand()->getWeaponType() != Weapon::WeaponType::SHIELD)
		{
			buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rOpponent.getLeftHand()->getDamage()) + " (+" + to_string(rOpponent.getLeftHand()->getDamageAddition()) + ")";
			SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

			ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_SHIELD_DEFENSE], SW_HIDE);
			ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_SHIELD_BLOCK_CHANCE], SW_HIDE);
			game.updateBackground();
		}
		else
		{
			buf = l.getMessage(Localized::BLOCK_DEFENSE) + ": " + to_string(rOpponent.getLeftHand()->getShieldDefPercentAddition());
			SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_SHIELD_DEFENSE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
			buf = l.getMessage(Localized::BLOCK_CHANCE) + ": " + to_string(rOpponent.getLeftHand()->getShieldProbAddition());
			SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_SHIELD_BLOCK_CHANCE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
		}
	}
	else
	{
		SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::EMPTY_HAND).c_str());

		ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], SW_HIDE);
		ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_SHIELD_DEFENSE], SW_HIDE);
		ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_SHIELD_BLOCK_CHANCE], SW_HIDE);
		game.updateBackground();
	}

	// Health
	buf = l.getMessage(Localized::HEALTH) + ": " + to_string(rOpponent.getHP());
	SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_HEALTH], WM_SETTEXT, 0, (LPARAM)buf.c_str());

	// Armour
	if (rOpponent.getArmour())
	{
		SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)l.getArmourTypeName(*rOpponent.getArmour()).c_str());

		buf = l.getMessage(Localized::ARMOUR_DEFENSE) + ": " + to_string(rOpponent.getArmour()->getDefense()) + " (+" + to_string(rOpponent.getArmour()->getDefAddition()) + ")";
		SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
	}

	// Stats
	buf = l.getMessage(Localized::STRENGTH) + ": " + to_string(rOpponent.getStrength());
	SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_STRENGTH], WM_SETTEXT, 0, (LPARAM)buf.c_str());

	buf = l.getMessage(Localized::CONSTITUTION) + ": " + to_string(rOpponent.getConstitution());
	SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)buf.c_str());

	buf = l.getMessage(Localized::DEXTERITY) + ": " + to_string(rOpponent.getDexterity());
	SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_DEXTERITY], WM_SETTEXT, 0, (LPARAM)buf.c_str());

	buf = l.getMessage(Localized::INTELLIGENCE) + ": " + to_string(rOpponent.getIntelligence());
	SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

	buf = l.getMessage(Localized::WISDOM) + ": " + to_string(rOpponent.getWisdom());
	SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_WISDOM], WM_SETTEXT, 0, (LPARAM)buf.c_str());

	buf = l.getMessage(Localized::CHARISMA) + ": " + to_string(rOpponent.getCharisma());
	SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CHARISMA], WM_SETTEXT, 0, (LPARAM)buf.c_str());

	buf = l.getMessage(Localized::AGE) + ": " + to_string(rOpponent.getAge());
	SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_AGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

	buf = l.getMessage(Localized::FAME) + ": " + to_string(rOpponent.getFame());
	SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_FAME], WM_SETTEXT, 0, (LPARAM)buf.c_str());
}

void CityMenu::drawInventoryItem(HWND hWnd, LPDRAWITEMSTRUCT item, Inventory& rInventory, int buttonIndex, bool isPushed)
{
	// Selecting image for button based on item type
	const string DIRECTORY = "Data/Image/Items/",
				 FORMAT = ".bmp";
	string path = "";

	// Getting item represented by button
	auto itemPair = rInventory[buttonIndex];
	unique_ptr<Item>& rItem = itemPair.first;

	switch (rItem->getItemType())
	{
	case Item::ItemType::WEAPON:
		if (auto weapon = dynamic_cast<Weapon*>(rItem.get()))
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
		if (auto armour = dynamic_cast<Armour*>(rItem.get()))
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

void CityMenu::drawEquippedItem(HWND hWnd, LPDRAWITEMSTRUCT item, int itemIndex, bool isPushed)
{
	/*
	itemIndex
	0 -- rightHand
	1 -- leftHand
	2 -- armour
	*/

	Player& rPlayer = game.getPlayer();
	// Selecting image for button based on item type
	const string DIRECTORY = "Data/Image/Items/",
		FORMAT = ".bmp";
	string path = "";
	switch (itemIndex)
	{
	case 0:
		if (rPlayer.getRightHand())
		{
			switch (rPlayer.getRightHand()->getWeaponType())
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
			path = DIRECTORY + "emptyHand"; break;
		break;
	case 1:
		if (rPlayer.getLeftHand())
		{
			switch (rPlayer.getLeftHand()->getWeaponType())
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
			path = DIRECTORY + "emptyHand"; break;
		break;
	case 2:
		if (rPlayer.getArmour())
		{
			switch (rPlayer.getArmour()->getArmourType())
			{
			case Armour::ArmourType::LIGHT: path = DIRECTORY + "lightArmour"; break;
			case Armour::ArmourType::HEAVY: path = DIRECTORY + "heavyArmour"; break;
			default: path = DIRECTORY + "error"; break;
			}
		}
		else
			path = DIRECTORY + "noArmour"; break;
		break;
	default: path = DIRECTORY + "error"; break;
	}

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

void CityMenu::inspectItem(HWND hWnd, unique_ptr<Item> pItem, int quantity)
{
	string MBName = "";
	string MBDescription = "";
	switch (pItem->getItemType())
	{
	case Item::ItemType::WEAPON:
		if (auto weapon = dynamic_cast<Weapon*>(pItem.get()))
		{
			MBName += l.getWeaponTypeName(*weapon) + " (" + l.getMessage(Localized::ITEM_TIER) + "  " + to_string(weapon->getTier()) + ")";
			if (weapon->getWeaponType() == Weapon::WeaponType::SHIELD)
			{
				MBDescription += l.getMessage(Localized::BLOCK_DEFENSE) + ": " + to_string(weapon->getShieldDefPercentAddition()) + "%" + "\n";
				MBDescription += l.getMessage(Localized::BLOCK_CHANCE) + ": " + to_string(weapon->getShieldProbAddition()) + "%" + "\n";
			}
			else
			{
				MBDescription += l.getMessage(Localized::DAMAGE) + ": " + to_string(weapon->getDamage()) + " (+" + to_string(weapon->getDamageAddition()) + ")\n";
				MBDescription += l.getMessage(Localized::STRENGTH_SCALE) + ": " + to_string(weapon->getStrengthAdditionPercentage()) + "%" + "\n";
				MBDescription += l.getMessage(Localized::DEXTERITY_SCALE) + ": " + to_string(weapon->getDexterityAdditionPercentage()) + "%" + "\n";
			}
		}
		break;

	case Item::ItemType::ARMOUR:
		if (auto armour = dynamic_cast<Armour*>(pItem.get()))
		{
			MBName += l.getArmourTypeName(*armour) + " (" + l.getMessage(Localized::ITEM_TIER) + "  " + to_string(armour->getTier()) + ")";
			MBDescription += l.getMessage(Localized::ARMOUR_DEFENSE) + ": " + to_string(armour->getDefense()) + " (+" + to_string(armour->getDefAddition()) + ")\n";
			MBDescription += l.getMessage(Localized::STRENGTH_SCALE) + ": " + to_string(armour->getStrengthAdditionPercentage()) + "%" + "\n";
			MBDescription += l.getMessage(Localized::DEXTERITY_SCALE) + ": " + to_string(armour->getDexterityAdditionPercentage()) + "%" + "\n";

			if (armour->getArmourType() == Armour::ArmourType::LIGHT)
				MBDescription += l.getMessage(Localized::EVASION_CHANCE) + ": " + to_string(armour->getEvasionProbAddition()) + "%" + "\n";
			else
				MBDescription += l.getMessage(Localized::STUN_RESISTANCE_CHANCE) + ": " + to_string(armour->getStunProbSubtraction()) + "%" + "\n";
		}
		break;
	default:
		MBName += l.getItemTypeName(*pItem);
	}

	MBDescription += l.getMessage(Localized::VALUE) + ": " + to_string(pItem->getValue()) + "\n";
	MBDescription += l.getMessage(Localized::QUANTITY) + ": " + to_string(quantity) + "\n";
	MBDescription += l.getMessage(Localized::TOTAL_VALUE) + ": " + to_string(pItem->getPrice() * quantity) + "\n";

	MessageBox(hWnd, MBDescription.c_str(), MBName.c_str(), MB_OK | MB_ICONINFORMATION);
}

void CityMenu::manageInventory(HWND hWnd, int selectedItemm)
{
	// Get cursor position
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(hWnd, &pt);

	Player& rPlayer = game.getPlayer();
	if (selectedItemm  == CHARACTER_BUT_RIGHT_HAND) // Right hand weapon
	{
		if (!rPlayer.getRightHand())
			return;

		// Hide unrelated buttons
		ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_EQUIP_ITEM], SW_HIDE);
		ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_DESTROY_ITEM], SW_HIDE);
		// Show buttons near cursor
		ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_INSPECT_ITEM], SW_SHOW);
		SetWindowPos(hSubItems[CHARACTER_BUT_INVENTORY_INSPECT_ITEM], HWND_TOP, pt.x, pt.y, 0, 0, SWP_NOSIZE);

		if (rPlayer.getInventory()->size() < MAX_INVENTORY_SIZE)
		{
			ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_UNEQUIP_ITEM], SW_SHOW);
			SetWindowPos(hSubItems[CHARACTER_BUT_INVENTORY_UNEQUIP_ITEM], HWND_TOP, pt.x, pt.y + 30, 0, 0, SWP_NOSIZE);
		}

		RedrawWindow(hSubItems[CHARACTER_BUT_RIGHT_HAND], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);
		RedrawWindow(hSubItems[CHARACTER_STAT_RIGHT_HAND_TYPE], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);
		RedrawWindow(hSubItems[CHARACTER_STAT_RIGHT_HAND_DAMAGE], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);

		game.updateBackground();
	}
	else if (selectedItem == CHARACTER_BUT_LEFT_HAND) // Left hand weapon
	{
		if (!rPlayer.getLeftHand())
			return;

		// Hide unrelated buttons
		ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_EQUIP_ITEM], SW_HIDE);
		ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_DESTROY_ITEM], SW_HIDE);
		// Show buttons near cursor
		ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_INSPECT_ITEM], SW_SHOW);
		SetWindowPos(hSubItems[CHARACTER_BUT_INVENTORY_INSPECT_ITEM], HWND_TOP, pt.x, pt.y, 0, 0, SWP_NOSIZE);

		if (rPlayer.getInventory()->size() < MAX_INVENTORY_SIZE)
		{
			ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_UNEQUIP_ITEM], SW_SHOW);
			SetWindowPos(hSubItems[CHARACTER_BUT_INVENTORY_UNEQUIP_ITEM], HWND_TOP, pt.x, pt.y + 30, 0, 0, SWP_NOSIZE);
		}

		RedrawWindow(hSubItems[CHARACTER_BUT_LEFT_HAND], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);
		RedrawWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_TYPE], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);
		RedrawWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_DAMAGE], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);

		game.updateBackground();
	}
	else if (selectedItem == CHARACTER_BUT_ARMOUR) // Armour
	{
		if (!rPlayer.getArmour())
			return;

		// Hide unrelated buttons
		ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_EQUIP_ITEM], SW_HIDE);
		ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_DESTROY_ITEM], SW_HIDE);
		// Show buttons near cursor
		ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_INSPECT_ITEM], SW_SHOW);
		SetWindowPos(hSubItems[CHARACTER_BUT_INVENTORY_INSPECT_ITEM], HWND_TOP, pt.x, pt.y, 0, 0, SWP_NOSIZE);

		if (rPlayer.getInventory()->size() < MAX_INVENTORY_SIZE)
		{
			ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_UNEQUIP_ITEM], SW_SHOW);
			SetWindowPos(hSubItems[CHARACTER_BUT_INVENTORY_UNEQUIP_ITEM], HWND_TOP, pt.x, pt.y + 30, 0, 0, SWP_NOSIZE);
		}

		RedrawWindow(hSubItems[CHARACTER_BUT_ARMOUR], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);
		RedrawWindow(hSubItems[CHARACTER_STAT_ARMOUR_TYPE], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);
		RedrawWindow(hSubItems[CHARACTER_STAT_ARMOUR_DEFENSE], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);

		game.updateBackground();
	}
	else // Inventory
	{
		// Hide unrelated buttons
		ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_EQUIP_ITEM], SW_HIDE);
		ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_UNEQUIP_ITEM], SW_HIDE);

		// Adjust for button id
		selectedItemm -= CHARACTER_BUT_INVENTORY_ITEM1;

		// Getting item represented by button
		auto itemPair = (*game.getPlayer().getInventory())[selectedItemm];
		unique_ptr<Item>& pItem = itemPair.first;

		bool isEquippable = false;
		switch (pItem->getItemType())
		{
		case Item::ItemType::WEAPON:
			if (auto weapon = dynamic_cast<Weapon*>(pItem.get()))
			{
				switch (weapon->getWeaponType())
				{
				default:
					if (!rPlayer.getRightHand() && !rPlayer.getLeftHand())
						isEquippable = true;
					else if (rPlayer.getRightHand() && rPlayer.getRightHand()->isCompatibleWith(weapon->getWeaponType()) && !rPlayer.getLeftHand())
						isEquippable = true;
					else if (rPlayer.getLeftHand() && rPlayer.getLeftHand()->isCompatibleWith(weapon->getWeaponType()) && !rPlayer.getRightHand())
						isEquippable = true;
					break;

				case Weapon::WeaponType::AXE:
					if (!rPlayer.getRightHand() && !rPlayer.getLeftHand())
						isEquippable = true;
					break;

				case Weapon::WeaponType::SPEAR:
					if (!rPlayer.getRightHand() && !rPlayer.getLeftHand())
						isEquippable = true;
					break;
				}
			}
			break;

		case Item::ItemType::ARMOUR:
			if (auto armour = dynamic_cast<Armour*>(pItem.get()))
				if (!rPlayer.getArmour())
					isEquippable = true;
			break;

		case Item::ItemType::GOLD: break;
		default: break;
		}

		if (isEquippable)
		{
			ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_EQUIP_ITEM], SW_SHOW);
			SetWindowPos(hSubItems[CHARACTER_BUT_INVENTORY_EQUIP_ITEM], HWND_TOP, pt.x, pt.y, 0, 0, SWP_NOSIZE);
			pt.y += 30;
		}
		ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_INSPECT_ITEM], SW_SHOW);
		ShowWindow(hSubItems[CHARACTER_BUT_INVENTORY_DESTROY_ITEM], SW_SHOW);

		SetWindowPos(hSubItems[CHARACTER_BUT_INVENTORY_INSPECT_ITEM], HWND_TOP, pt.x, pt.y, 0, 0, SWP_NOSIZE);
		SetWindowPos(hSubItems[CHARACTER_BUT_INVENTORY_DESTROY_ITEM], HWND_TOP, pt.x, pt.y + 30, 0, 0, SWP_NOSIZE);

		RedrawWindow(hSubItems[selectedItemm + CHARACTER_BUT_INVENTORY_ITEM1], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);
		RedrawWindow(hSubItems[selectedItemm + CHARACTER_BUT_INVENTORY_ITEM1 + 1], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);
		RedrawWindow(hSubItems[selectedItemm + CHARACTER_STAT_INVENTORY_ITEM1_NAME], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);
		RedrawWindow(hSubItems[selectedItemm + CHARACTER_STAT_INVENTORY_ITEM1_NAME + 1], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);
		if (selectedItemm < 7)
		{
			RedrawWindow(hSubItems[selectedItemm + CHARACTER_BUT_INVENTORY_ITEM1 + 7], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);
			RedrawWindow(hSubItems[selectedItemm + CHARACTER_BUT_INVENTORY_ITEM1 + 8], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);
			RedrawWindow(hSubItems[selectedItemm + CHARACTER_STAT_INVENTORY_ITEM1_NAME + 7], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);
			RedrawWindow(hSubItems[selectedItemm + CHARACTER_STAT_INVENTORY_ITEM1_NAME + 8], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);
		}
		else
			RedrawWindow(hSubItems[CHARACTER_BUT_BACK], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);
		
		game.updateBackground();
	}
}

void CityMenu::manageMarketItems(HWND hWnd, int selectedItemm)
{
	// Get cursor position
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(hWnd, &pt);

	Player& rPlayer = game.getPlayer();
	if (selectedItemm == MARKET_BUT_RIGHT_HAND) // Right hand weapon
	{
		if (!rPlayer.getRightHand())
			return;

		// Hide unrelated buttons
		ShowWindow(hSubItems[MARKET_BUT_EQUIP_ITEM], SW_HIDE);
		ShowWindow(hSubItems[MARKET_BUT_DESTROY_ITEM], SW_HIDE);
		// Show buttons near cursor
		if (rPlayer.getInventory()->size() < MAX_INVENTORY_SIZE)
		{
			ShowWindow(hSubItems[MARKET_BUT_UNEQUIP_ITEM], SW_SHOW);
			SetWindowPos(hSubItems[MARKET_BUT_UNEQUIP_ITEM], HWND_TOP, pt.x, pt.y, 0, 0, SWP_NOSIZE);
		}

		RedrawWindow(hSubItems[MARKET_BUT_RIGHT_HAND], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);
		RedrawWindow(hSubItems[MARKET_STAT_RIGHT_HAND], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);
		RedrawWindow(hSubItems[MARKET_BUT_LEFT_HAND], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);
		RedrawWindow(hSubItems[MARKET_STAT_LEFT_HAND], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);

		game.updateBackground();
	}
	else if (selectedItem == MARKET_BUT_LEFT_HAND) // Left hand weapon
	{
		if (!rPlayer.getLeftHand())
			return;

		// Hide unrelated buttons
		ShowWindow(hSubItems[MARKET_BUT_EQUIP_ITEM], SW_HIDE);
		ShowWindow(hSubItems[MARKET_BUT_DESTROY_ITEM], SW_HIDE);
		// Show buttons near cursor
		if (rPlayer.getInventory()->size() < MAX_INVENTORY_SIZE)
		{
			ShowWindow(hSubItems[MARKET_BUT_UNEQUIP_ITEM], SW_SHOW);
			SetWindowPos(hSubItems[MARKET_BUT_UNEQUIP_ITEM], HWND_TOP, pt.x, pt.y, 0, 0, SWP_NOSIZE);
		}

		RedrawWindow(hSubItems[MARKET_BUT_LEFT_HAND], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);
		RedrawWindow(hSubItems[MARKET_STAT_LEFT_HAND], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);
		RedrawWindow(hSubItems[MARKET_BUT_ARMOUR], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);
		RedrawWindow(hSubItems[MARKET_STAT_ARMOUR], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);

		game.updateBackground();
	}
	else if (selectedItem == MARKET_BUT_ARMOUR) // Armour
	{
		if (!rPlayer.getArmour())
			return;

		// Hide unrelated buttons
		ShowWindow(hSubItems[MARKET_BUT_EQUIP_ITEM], SW_HIDE);
		ShowWindow(hSubItems[MARKET_BUT_DESTROY_ITEM], SW_HIDE);
		// Show buttons near cursor
		if (rPlayer.getInventory()->size() < MAX_INVENTORY_SIZE)
		{
			ShowWindow(hSubItems[MARKET_BUT_UNEQUIP_ITEM], SW_SHOW);
			SetWindowPos(hSubItems[MARKET_BUT_UNEQUIP_ITEM], HWND_TOP, pt.x, pt.y, 0, 0, SWP_NOSIZE);
		}

		RedrawWindow(hSubItems[MARKET_BUT_ARMOUR], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);

		game.updateBackground();
	}
	else // Inventory
	{
		// Hide unrelated buttons
		ShowWindow(hSubItems[MARKET_BUT_UNEQUIP_ITEM], SW_HIDE);
		ShowWindow(hSubItems[MARKET_BUT_EQUIP_ITEM], SW_HIDE);

		// Adjust for button id
		selectedItemm -= MARKET_BUT_INVENTORY_ITEM1;

		// Getting item represented by button
		auto itemPair = (*game.getPlayer().getInventory())[selectedItemm];
		unique_ptr<Item>& pItem = itemPair.first;

		bool isEquippable = false;
		switch (pItem->getItemType())
		{
		case Item::ItemType::WEAPON:
			if (auto weapon = dynamic_cast<Weapon*>(pItem.get()))
			{
				switch (weapon->getWeaponType())
				{
				default:
					if (!rPlayer.getRightHand() && !rPlayer.getLeftHand())
						isEquippable = true;
					else if (rPlayer.getRightHand() && rPlayer.getRightHand()->isCompatibleWith(weapon->getWeaponType()) && !rPlayer.getLeftHand())
						isEquippable = true;
					else if (rPlayer.getLeftHand() && rPlayer.getLeftHand()->isCompatibleWith(weapon->getWeaponType()) && !rPlayer.getRightHand())
						isEquippable = true;
					break;

				case Weapon::WeaponType::AXE:
					if (!rPlayer.getRightHand() && !rPlayer.getLeftHand())
						isEquippable = true;
					break;

				case Weapon::WeaponType::SPEAR:
					if (!rPlayer.getRightHand() && !rPlayer.getLeftHand())
						isEquippable = true;
					break;
				}
			}
			break;

		case Item::ItemType::ARMOUR:
			if (auto armour = dynamic_cast<Armour*>(pItem.get()))
				if (!rPlayer.getArmour())
					isEquippable = true;
			break;

		case Item::ItemType::GOLD: break;
		default: break;
		}

		if (isEquippable)
		{
			ShowWindow(hSubItems[MARKET_BUT_EQUIP_ITEM], SW_SHOW);
			SetWindowPos(hSubItems[MARKET_BUT_EQUIP_ITEM], HWND_TOP, pt.x, pt.y, 0, 0, SWP_NOSIZE);
			pt.y += 30;
		}
		if (pItem->getItemType() != Item::ItemType::GOLD)
		{
			ShowWindow(hSubItems[MARKET_BUT_DESTROY_ITEM], SW_SHOW);
			SetWindowPos(hSubItems[MARKET_BUT_DESTROY_ITEM], HWND_TOP, pt.x, pt.y, 0, 0, SWP_NOSIZE);
		}

		int i = MARKET_STAT_INVENTORY_ITEM1;
		for (; i <= MARKET_STAT_INVENTORY_ITEM14; i++)
			RedrawWindow(hSubItems[i], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);

		for (i = MARKET_BUT_INVENTORY_ITEM1; i <= MARKET_BUT_INVENTORY_ITEM14; i++)
			RedrawWindow(hSubItems[i], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);

		RedrawWindow(hSubItems[MARKET_STAT_EQUIPMENT], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);

		for (i = MARKET_STAT_ITEM_TYPE; i <= MARKET_STAT_ITEM_TOTAL_VALUE; i++)
			RedrawWindow(hSubItems[i], NULL, NULL, RDW_UPDATENOW | RDW_ALLCHILDREN);

		game.updateBackground();
	}
}
