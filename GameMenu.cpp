#include "stdafx.h"
#include "GameMenu.h"

extern TCHAR str[256];
extern string buf;

extern HINSTANCE hInst;
extern Localization l;
extern Game game;

GameMenu::GameMenu() :
	hItems(),
	hSubItems(),
	hBackgroundImage(NULL),
	hBackgroundBrush(NULL),
	selected(-1)
{ }

GameMenu::GameMenu(HWND hWnd) :
	hItems(menuItem::ITEM_NUMBER),
	hSubItems(),
	hBackgroundImage(NULL),
	hBackgroundBrush(NULL),
	selected(-1)
{
	char className[256] = "BUTTON";
	hItems[BUT_RESUME] = CreateWindow(className, l.getMessage(Localized::RESUME_GAME).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_SAVE] = CreateWindow(className, l.getMessage(Localized::SAVE_GAME).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_LOAD] = CreateWindow(className, l.getMessage(Localized::LOAD_GAME).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_SETTINGS] = CreateWindow(className, l.getMessage(Localized::SETTINGS).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_EXIT_MENU] = CreateWindow(className, l.getMessage(Localized::EXIT_TO_MENU).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
}

GameMenu::GameMenu(const GameMenu& GM) : hSubItems()
{
	// Resizing items' vector
	int sz = GM.hItems.size();
	hItems.resize(sz);

	HWND hItem;
	RECT rect;
	char className[256], windowText[256];
	for (int i = 0; i < sz; i++)
	{
		// Obtaining the previous parameters
		hItem = GM.hItems[i];
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

	if (GM.hBackgroundImage != NULL)
	{
		BITMAP bm;
		GetObject(GM.hBackgroundImage, sizeof(BITMAP), &bm);
		hBackgroundImage = CreateBitmapIndirect(&bm);
	}
	else
		hBackgroundImage = NULL;

	if (GM.hBackgroundBrush != NULL)
	{
		LOGBRUSH lb;
		GetObject(GM.hBackgroundBrush, sizeof(LOGBRUSH), &lb);
		hBackgroundBrush = CreateBrushIndirect(&lb);
	}
	else
		hBackgroundBrush = NULL;

	selected = GM.selected;
}

GameMenu& GameMenu::operator=(const GameMenu& GM)
{
	if (&GM == this) return *this;

	// Destroying the previous items of the menu
	for (HWND hItem : hItems)
		if (hItem != NULL)
			DestroyWindow(hItem);

	// Resizing items' vector
	int sz = GM.hItems.size();
	hItems.resize(sz);

	HWND hItem;
	RECT rect;
	char className[256], windowText[256];
	for (int i = 0; i < sz; i++)
	{
		// Obtaining the previous parameters
		hItem = GM.hItems[i];
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

	if (hBackgroundImage != NULL)
		DeleteObject(hBackgroundImage);

	if (GM.hBackgroundImage != NULL)
	{
		BITMAP bm;
		GetObject(GM.hBackgroundImage, sizeof(BITMAP), &bm);
		hBackgroundImage = CreateBitmapIndirect(&bm);
	}
	else
		hBackgroundImage = NULL;

	if (hBackgroundBrush != NULL)
		DeleteObject(hBackgroundBrush);

	if (GM.hBackgroundBrush != NULL)
	{
		LOGBRUSH lb;
		GetObject(GM.hBackgroundBrush, sizeof(LOGBRUSH), &lb);
		hBackgroundBrush = CreateBrushIndirect(&lb);
	}
	else
		hBackgroundBrush = NULL;

	selected = GM.selected;

	return *this;
}

GameMenu::~GameMenu()
{
	for (HWND hItem : hItems)
		if (hItem != NULL)
			DestroyWindow(hItem);

	for (HWND hItem : hSubItems)
		if (hItem != NULL)
			DestroyWindow(hItem);

	if (hBackgroundImage != NULL)
		DeleteObject(hBackgroundImage);

	if (hBackgroundBrush != NULL)
		DeleteObject(hBackgroundBrush);
}

void GameMenu::drawMenu(HWND hWnd, HDC hdc, int cx, int cy)
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
		default:case Game::Background::GAME_MENU: path = DIRECTORY + "menuBackground" + FORMAT; break;
		case Game::Background::GAME_MENU_SAVE: path = DIRECTORY + "menuBackground" + FORMAT; break; // TODO: background
		case Game::Background::GAME_MENU_LOAD: path = DIRECTORY + "menuBackground" + FORMAT; break; // TODO: background
		case Game::Background::GAME_MENU_SETTINGS: path = DIRECTORY + "menuBackground" + FORMAT; break; // TODO: background
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
	game.backgroundChangeCompleted();
}

void GameMenu::resizeMenu(int cx, int cy)
{
	// If background is the same - no change needed
	if (!game.isBackgroundChanged())
		return;

	int sz, x, y;

	switch (game.getBackground())
	{
	default:case Game::Background::GAME_MENU:
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

	case Game::Background::GAME_MENU_SAVE:
	{
		// TODO
	}
	break;

	case Game::Background::GAME_MENU_LOAD:
	{
		const int ITEM_WIDTH = 300, ITEM_HEIGHT = 30, DISTANCE = 10;

		MoveWindow(hSubItems[LOADING_STAT_LOAD], cx - ITEM_WIDTH / 2, DISTANCE * 2, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hSubItems[LOADING_BUT_BACK], cx - ITEM_WIDTH - DISTANCE, 670, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hSubItems[LOADING_BUT_LOAD], cx + DISTANCE, 670, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hSubItems[LOADING_LISTBOX_SAVES], cx - 250, DISTANCE * 4 + ITEM_HEIGHT, 500, 580, TRUE);
	}
	break;

	case Game::Background::GAME_MENU_SETTINGS:
	{
		const int ITEM_HEIGHT = 45, DISTANCE = 10, ITEM_WIDTH = 300, BUT_SIZE = 45;
		sz = hItems.size();
		x = cx - ITEM_WIDTH / 2;
		y = cy - sz / 2 * (ITEM_HEIGHT + DISTANCE);

		// Video settings
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


		// Back
		MoveWindow(hSubItems[SETTINGS_BUT_BACK], x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
	}
	break;
	}
}

void GameMenu::handleInput(HWND hWnd, UINT m, WPARAM wp, LPARAM lp)
{
	RECT windowRect;
	GetWindowRect(hWnd, &windowRect);

	switch (m)
	{
	case WM_COMMAND:
		switch (game.getBackground())
		{
		default:case Game::Background::GAME_MENU:
		{
			if ((HWND)lp == hItems[BUT_RESUME] || LOWORD(wp) == IDCANCEL)
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				game.getMenuManager().setMenu(new CityMenu(hWnd));
				game.setBackground(Game::Background::CITY_MENU);
				updateWindow(hWnd);
				break;
			}
			if ((HWND)lp == hItems[BUT_SAVE])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				game.saveToFile();
				// TODO: Stylize
				MessageBox(hWnd, l.getMessage(Localized::SAVE_SUCCESFUL).c_str(), "", MB_OK);
			}
			if ((HWND)lp == hItems[BUT_LOAD])
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

				game.setBackground(Game::Background::GAME_MENU_LOAD);

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
				hSubItems.resize(SETTINGS_ITEM_NUMBER);

				hSubItems[SETTINGS_STAT_VIDEO] = CreateWindow("STATIC", l.getMessage(Localized::VIDEO_SETTINGS).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[SETTINGS_STAT_SOUND] = CreateWindow("STATIC", l.getMessage(Localized::AUDIO_SETTINGS).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[SETTINGS_STAT_AUTOSAVE] = CreateWindow("STATIC", l.getMessage(Localized::AUTOSAVE_SETTINGS).c_str(), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[SETTINGS_BUT_SOUND] = CreateWindow("BUTTON", "", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[SETTINGS_BUT_AUTOSAVE] = CreateWindow("BUTTON", "", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[SETTINGS_BUT_BACK] = CreateWindow("BUTTON", l.getMessage(Localized::BACK).c_str(), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				game.setBackground(Game::Background::GAME_MENU_SETTINGS);

				updateWindow(hWnd);
			}

			if ((HWND)lp == hItems[BUT_EXIT_MENU])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				game.getMenuManager().setMenu(new MainMenu(hWnd));
				game.setBackground(Game::Background::MAIN_MENU);
				updateWindow(hWnd);
				break;
			}
		}
		break;

		case Game::Background::GAME_MENU_LOAD:
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

				game.setBackground(Game::Background::GAME_MENU);

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

				// Load game
				game.loadFromFile(path);

				// Return to game menu
				// Destroying all buttons
				for (HWND hItem : hSubItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubItems.clear();

				// Showing main menu buttons
				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_SHOW);

				game.setBackground(Game::Background::GAME_MENU);

				updateWindow(hWnd);
				break;
			}
		}
		break;

		case Game::Background::GAME_MENU_SETTINGS:
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

			if ((HWND)lp == hSubItems[SETTINGS_BUT_BACK] || LOWORD(wp) == IDCANCEL)
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

				game.setBackground(Game::Background::GAME_MENU);

				updateWindow(hWnd);
				break;
			}
		}
		break;
		}
	break;
	}
}

bool GameMenu::stylizeWindow(HWND hWnd, UINT m, WPARAM wp, LPARAM lp, LRESULT& result)
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
			if (game.getBackground() == Game::Background::GAME_MENU_SETTINGS)
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
