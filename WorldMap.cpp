#include "stdafx.h"
#include "WorldMap.h"

extern TCHAR str[256];
extern string buf;
extern string logStr;

extern HINSTANCE hInst;
extern Localization l;
extern Game game;

WorldMap::WorldMap() :
	hItems(Item::ITEM_NUMBER),
	cities(),
	currentCity(0),
	selectedCity(0),
	hBackgroundImage(NULL),
	hBackgroundBrush(NULL)
{ }

WorldMap::WorldMap(HWND hWnd) :
	hItems(Item::ITEM_NUMBER),
	cities(),
	currentCity(0),
	selectedCity(0),
	hBackgroundImage(NULL),
	hBackgroundBrush(NULL)
{
	hItems[STAT_MAP] = CreateWindow("STATIC", "",
		WS_CHILD | WS_VISIBLE | SS_CENTER | SS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0);

	int i = BUT_ROME_MAP;
	for (; i <= BUT_MILAN_MAP; i++)
		hItems[i] = CreateWindow("BUTTON", "",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
			0, 0, 0, 0, hWnd, 0, hInst, 0);

	for (i = BUT_ROME_LIST; i <= BUT_MILAN_LIST; i++)
	{
		hItems[i] = CreateWindow("BUTTON", "",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_AUTORADIOBUTTON | BS_OWNERDRAW | BS_LEFTTEXT,
			0, 0, 0, 0, hWnd, 0, hInst, 0);
	}

	hItems[BUT_TRAVEL_LIST] = CreateWindow("BUTTON", "",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0);

	hItems[BUT_PLAYER_ICON_MAP] = CreateWindow("BUTTON", "",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0);

	hItems[BUT_PLAYER_ICON_LIST] = CreateWindow("BUTTON", "",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0);
}

WorldMap::WorldMap(HWND hWnd, const vector<City>& C, int ccurrentCity) :
	hItems(Item::ITEM_NUMBER),
	cities(C),
	currentCity(ccurrentCity),
	selectedCity(ccurrentCity),
	hBackgroundImage(NULL),
	hBackgroundBrush(NULL)
{
	hItems[STAT_MAP] = CreateWindow("STATIC", l.getMessage(Localized::MAP).c_str(),
		WS_CHILD | WS_VISIBLE | SS_CENTER | SS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0);

	int i = BUT_ROME_MAP;
	for (; i <= BUT_MILAN_MAP; i++)
		hItems[i] = CreateWindow("BUTTON", l.getCityName(C[i]).c_str(),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
			0, 0, 0, 0, hWnd, 0, hInst, 0);

	for (i = BUT_ROME_LIST; i <= BUT_MILAN_LIST; i++)
	{
		buf = l.getCityName(C[i - BUT_ROME_LIST]) + " (" + to_string(C[i - BUT_ROME_LIST].getLevel()) + " lvl)";
		hItems[i] = CreateWindow("BUTTON", buf.c_str(),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_AUTORADIOBUTTON | BS_OWNERDRAW | BS_LEFTTEXT,
			0, 0, 0, 0, hWnd, 0, hInst, 0);
	}

	hItems[BUT_TRAVEL_LIST] = CreateWindow("BUTTON", l.getMessage(Localized::TRAVEL).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0);

	hItems[BUT_PLAYER_ICON_MAP] = CreateWindow("BUTTON", "",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0);

	hItems[BUT_PLAYER_ICON_LIST] = CreateWindow("BUTTON", "",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0);
}

WorldMap::WorldMap(const WorldMap& WM) :
	cities(WM.cities),
	currentCity(WM.currentCity),
	selectedCity(WM.selectedCity)
{
	// Resizing items' vector
	int sz = WM.hItems.size();
	hItems.resize(sz);

	HWND hItem;
	RECT rect;
	char className[256], windowText[256];
	for (int i = 0; i < sz; i++)
	{
		// Obtaining the previous parameters
		hItem = WM.hItems[i];
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

	if (WM.hBackgroundImage != NULL)
	{
		BITMAP bm;
		GetObject(WM.hBackgroundImage, sizeof(BITMAP), &bm);
		hBackgroundImage = CreateBitmapIndirect(&bm);
	}
	else
		hBackgroundImage = NULL;

	if (WM.hBackgroundBrush != NULL)
	{
		LOGBRUSH lb;
		GetObject(WM.hBackgroundBrush, sizeof(LOGBRUSH), &lb);
		hBackgroundBrush = CreateBrushIndirect(&lb);
	}
	else
		hBackgroundBrush = NULL;
}

WorldMap& WorldMap::operator=(const WorldMap& WM)
{
	if (&WM == this) return *this;

	cities = WM.cities;
	currentCity = WM.currentCity;
	selectedCity = WM.selectedCity;
	hBackgroundImage = WM.hBackgroundImage;
	hBackgroundBrush = WM.hBackgroundBrush;

	// Resizing items' vector
	int sz = WM.hItems.size();
	hItems.resize(sz);

	HWND hItem;
	RECT rect;
	char className[256], windowText[256];
	for (int i = 0; i < sz; i++)
	{
		// Obtaining the previous parameters
		hItem = WM.hItems[i];
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

	if (hBackgroundImage != NULL)
		DeleteObject(hBackgroundImage);

	if (WM.hBackgroundImage != NULL)
	{
		BITMAP bm;
		GetObject(WM.hBackgroundImage, sizeof(BITMAP), &bm);
		hBackgroundImage = CreateBitmapIndirect(&bm);
	}
	else
		hBackgroundImage = NULL;

	if (hBackgroundBrush != NULL)
		DeleteObject(hBackgroundBrush);

	if (WM.hBackgroundBrush != NULL)
	{
		LOGBRUSH lb;
		GetObject(WM.hBackgroundBrush, sizeof(LOGBRUSH), &lb);
		hBackgroundBrush = CreateBrushIndirect(&lb);
	}
	else
		hBackgroundBrush = NULL;

	return *this;
}

WorldMap::~WorldMap()
{
	for (HWND hItem : hItems)
		if (hItem != NULL)
			DestroyWindow(hItem);

	if (hBackgroundImage != NULL)
		DeleteObject(hBackgroundImage);

	if (hBackgroundBrush != NULL)
		DeleteObject(hBackgroundBrush);
}

City& WorldMap::getCurrentCity()
{
	return cities[currentCity];
}

int WorldMap::getCurrentCityIndex() const
{
	return currentCity;
}

City& WorldMap::getCity(int i)
{
	return cities[i];
}

void WorldMap::setCurrentCityIndex(int n)
{
	currentCity = n;
}

void WorldMap::drawWindow(HWND hWnd, HDC hdc, int cx, int cy)
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
		default:case Game::Background::WORLD_MAP:
			path = DIRECTORY + "mapBackground" + FORMAT;
			for (HWND hItem : hItems)
				ShowWindow(hItem, SW_SHOW);
			break;
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
		default:case Game::Background::WORLD_MAP:
		{
			for (int i = BUT_ROME_MAP; i <= BUT_MILAN_LIST; i++)
				ShowWindow(hItems[i], SW_SHOW);

			if (currentCity == selectedCity)
				SendMessage(hItems[BUT_TRAVEL_LIST], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::ENTER).c_str());
			else
				SendMessage(hItems[BUT_TRAVEL_LIST], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::TRAVEL).c_str());

			for (int i = BUT_ROME_MAP; i <= BUT_MILAN_MAP; i++)
			{
				buf = l.getCityName(cities[i]);
				SendMessage(hItems[i], WM_SETTEXT, 0, (LPARAM)buf.c_str());
				buf = l.getCityName(cities[i]) + "   (" + to_string(cities[i].getLevel()) + " " + l.getMessage(Localized::LVL) + ")";
				SendMessage(hItems[i + 11], WM_SETTEXT, 0, (LPARAM)buf.c_str());
			}
		}
		break;
	}
	game.backgroundChangeCompleted();
}

void WorldMap::resizeWindow(int cx, int cy)
{
	// If background is the same - no change needed
	if (!game.isBackgroundChanged())
		return;

	int x, y, i;

	switch (game.getBackground())
	{
	default:case Game::Background::WORLD_MAP:
	{
		const int ITEM_HEIGHT = 40, ITEM_WIDTH = 300, WINDOW_WIDTH = 336, BUT_WIDTH = 300, DISTANCE = 3;
		const int FRAME_HEIGHT = 28, FRAME_WIDTH = 110;

		MoveWindow(hItems[BUT_ROME_MAP], 417, 519, FRAME_WIDTH, FRAME_HEIGHT, TRUE);
		MoveWindow(hItems[BUT_NAPLES_MAP], 542, 602, FRAME_WIDTH, FRAME_HEIGHT, TRUE);
		MoveWindow(hItems[BUT_METAPONTO_MAP], 729, 665, FRAME_WIDTH, FRAME_HEIGHT, TRUE);
		MoveWindow(hItems[BUT_BOJANO_MAP], 558, 528, FRAME_WIDTH, FRAME_HEIGHT, TRUE);
		MoveWindow(hItems[BUT_ANCONA_MAP], 458, 315, FRAME_WIDTH, FRAME_HEIGHT, TRUE);
		MoveWindow(hItems[BUT_PERUGIA_MAP], 395, 365, FRAME_WIDTH, FRAME_HEIGHT, TRUE);
		MoveWindow(hItems[BUT_FLORENCE_MAP], 296, 335, FRAME_WIDTH, FRAME_HEIGHT, TRUE);
		MoveWindow(hItems[BUT_BOLOGNA_MAP], 314, 238, FRAME_WIDTH, FRAME_HEIGHT, TRUE);
		MoveWindow(hItems[BUT_GENOA_MAP], 147, 207, FRAME_WIDTH, FRAME_HEIGHT, TRUE);
		MoveWindow(hItems[BUT_AQUILEIA_MAP], 351, 108, FRAME_WIDTH, FRAME_HEIGHT, TRUE);
		MoveWindow(hItems[BUT_MILAN_MAP], 119, 91, FRAME_WIDTH, FRAME_HEIGHT, TRUE);

		x = 982, y = 126;
		for (i = BUT_ROME_LIST; i <= BUT_MILAN_LIST; i++)
		{
			MoveWindow(hItems[i], x + 36, y, BUT_WIDTH, ITEM_HEIGHT, TRUE);
			y += ITEM_HEIGHT + DISTANCE;
		}

		MoveWindow(hItems[STAT_MAP], x, 9, WINDOW_WIDTH, 72, TRUE);

		MoveWindow(hItems[BUT_TRAVEL_LIST], x, 644, WINDOW_WIDTH, 67, TRUE);

		x = 982;
		switch (currentCity)
		{
		case Cities::ROME:
			MoveWindow(hItems[BUT_PLAYER_ICON_LIST], x, 132, FRAME_HEIGHT, FRAME_HEIGHT, TRUE);
			MoveWindow(hItems[BUT_PLAYER_ICON_MAP], 387, 519, FRAME_HEIGHT, FRAME_HEIGHT, TRUE);
			break;

		case Cities::NAPLES:
			MoveWindow(hItems[BUT_PLAYER_ICON_LIST], x, 175, FRAME_HEIGHT, FRAME_HEIGHT, TRUE);
			MoveWindow(hItems[BUT_PLAYER_ICON_MAP], 512, 602, FRAME_HEIGHT, FRAME_HEIGHT, TRUE);
			break;

		case Cities::METAPONTO:
			MoveWindow(hItems[BUT_PLAYER_ICON_LIST], x, 218, FRAME_HEIGHT, FRAME_HEIGHT, TRUE);
			MoveWindow(hItems[BUT_PLAYER_ICON_MAP], 699, 665, FRAME_HEIGHT, FRAME_HEIGHT, TRUE);
			break;

		case Cities::BOJANO:
			MoveWindow(hItems[BUT_PLAYER_ICON_LIST], x, 261, FRAME_HEIGHT, FRAME_HEIGHT, TRUE);
			MoveWindow(hItems[BUT_PLAYER_ICON_MAP], 528, 528, FRAME_HEIGHT, FRAME_HEIGHT, TRUE);
			break;

		case Cities::ANCONA:
			MoveWindow(hItems[BUT_PLAYER_ICON_LIST], x, 304, FRAME_HEIGHT, FRAME_HEIGHT, TRUE);
			MoveWindow(hItems[BUT_PLAYER_ICON_MAP], 428, 315, FRAME_HEIGHT, FRAME_HEIGHT, TRUE);
			break;

		case Cities::PERUGIA:
			MoveWindow(hItems[BUT_PLAYER_ICON_LIST], x, 347, FRAME_HEIGHT, FRAME_HEIGHT, TRUE);
			MoveWindow(hItems[BUT_PLAYER_ICON_MAP], 365, 365, FRAME_HEIGHT, FRAME_HEIGHT, TRUE);
			break;

		case Cities::FLORENCE:
			MoveWindow(hItems[BUT_PLAYER_ICON_LIST], x, 390, FRAME_HEIGHT, FRAME_HEIGHT, TRUE);
			MoveWindow(hItems[BUT_PLAYER_ICON_MAP], 266, 335, FRAME_HEIGHT, FRAME_HEIGHT, TRUE);
			break;

		case Cities::BOLOGNA:
			MoveWindow(hItems[BUT_PLAYER_ICON_LIST], x, 433, FRAME_HEIGHT, FRAME_HEIGHT, TRUE);
			MoveWindow(hItems[BUT_PLAYER_ICON_MAP], 284, 238, FRAME_HEIGHT, FRAME_HEIGHT, TRUE);
			break;

		case Cities::GENOA:
			MoveWindow(hItems[BUT_PLAYER_ICON_LIST], x, 476, FRAME_HEIGHT, FRAME_HEIGHT, TRUE);
			MoveWindow(hItems[BUT_PLAYER_ICON_MAP], 117, 207, FRAME_HEIGHT, FRAME_HEIGHT, TRUE);
			break;

		case Cities::AQUILEIA:
			MoveWindow(hItems[BUT_PLAYER_ICON_LIST], x, 519, FRAME_HEIGHT, FRAME_HEIGHT, TRUE);
			MoveWindow(hItems[BUT_PLAYER_ICON_MAP], 321, 108, FRAME_HEIGHT, FRAME_HEIGHT, TRUE);
			break;

		case Cities::MILAN:
			MoveWindow(hItems[BUT_PLAYER_ICON_LIST], x, 562, FRAME_HEIGHT, FRAME_HEIGHT, TRUE);
			MoveWindow(hItems[BUT_PLAYER_ICON_MAP], 89, 91, FRAME_HEIGHT, FRAME_HEIGHT, TRUE);
			break;
		}
	}
	break;
	}
}

void WorldMap::handleInput(HWND hWnd, UINT m, WPARAM wp, LPARAM lp)
{
	switch (m)
	{
	case WM_COMMAND:
		switch (game.getBackground())
		{
		default:case Game::Background::WORLD_MAP:
		{
			// Selecting city
			{
				if ((HWND)lp == hItems[BUT_PLAYER_ICON_LIST])
					selectCity(hWnd, static_cast<WorldMap::Item>(currentCity));

				if ((HWND)lp == hItems[BUT_PLAYER_ICON_MAP])
					selectCity(hWnd, static_cast<WorldMap::Item>(currentCity));

				if ((HWND)lp == hItems[BUT_ROME_LIST] || (HWND)lp == hItems[BUT_ROME_MAP])
					selectCity(hWnd, BUT_ROME_MAP);

				if ((HWND)lp == hItems[BUT_NAPLES_LIST] || (HWND)lp == hItems[BUT_NAPLES_MAP])
					selectCity(hWnd, BUT_NAPLES_MAP);

				if ((HWND)lp == hItems[BUT_METAPONTO_LIST] || (HWND)lp == hItems[BUT_METAPONTO_MAP])
					selectCity(hWnd, BUT_METAPONTO_MAP);

				if ((HWND)lp == hItems[BUT_BOJANO_LIST] || (HWND)lp == hItems[BUT_BOJANO_MAP])
					selectCity(hWnd, BUT_BOJANO_MAP);

				if ((HWND)lp == hItems[BUT_ANCONA_LIST] || (HWND)lp == hItems[BUT_ANCONA_MAP])
					selectCity(hWnd, BUT_ANCONA_MAP);

				if ((HWND)lp == hItems[BUT_PERUGIA_LIST] || (HWND)lp == hItems[BUT_PERUGIA_MAP])
					selectCity(hWnd, BUT_PERUGIA_MAP);

				if ((HWND)lp == hItems[BUT_FLORENCE_LIST] || (HWND)lp == hItems[BUT_FLORENCE_MAP])
					selectCity(hWnd, BUT_FLORENCE_MAP);

				if ((HWND)lp == hItems[BUT_BOLOGNA_LIST] || (HWND)lp == hItems[BUT_BOLOGNA_MAP])
					selectCity(hWnd, BUT_BOLOGNA_MAP);

				if ((HWND)lp == hItems[BUT_GENOA_LIST] || (HWND)lp == hItems[BUT_GENOA_MAP])
					selectCity(hWnd, BUT_GENOA_MAP);

				if ((HWND)lp == hItems[BUT_AQUILEIA_LIST] || (HWND)lp == hItems[BUT_AQUILEIA_MAP])
					selectCity(hWnd, BUT_AQUILEIA_MAP);

				if ((HWND)lp == hItems[BUT_MILAN_LIST] || (HWND)lp == hItems[BUT_MILAN_MAP])
					selectCity(hWnd, BUT_MILAN_MAP);

			}
			if ((HWND)lp == hItems[BUT_TRAVEL_LIST])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				if (currentCity != selectedCity)
				{
					Player& rPlayer = game.getPlayer();
					if (game.getPlayer().getFreedom())
					{
						if (rPlayer.getInventory()->getItemQuantity(0) >= CITY_TRAVEL_COST)
						{
							currentCity = selectedCity;
							SendMessage(hItems[BUT_TRAVEL_LIST], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::ENTER).c_str());
							logStr += l.getMessage(Localized::YOU_HAVE_TRAVELED) + " " + l.getCityName(getCurrentCity()) + "\r\n" +
								l.getMessage(Localized::SPENT_MONEY_ON_TRAVEL) + ": " + to_string(CITY_TRAVEL_COST) + "\r\n\r\n";
							cities[currentCity].getTrader().updateInventory();
							rPlayer.getInventory()->removeItem(0, CITY_TRAVEL_COST);
						}
						else
						{
							buf = l.getMessage(Localized::NOT_ENOUGH_MONEY_FOR_TRAVEL) + " (" + to_string(rPlayer.getInventory()->getItemQuantity(0)) + " / " + to_string(CITY_TRAVEL_COST) + ")";
							MessageBox(hWnd, buf.c_str(), l.getMessage(Localized::CANT_TRAVEL).c_str(), MB_OK);
						}
					}
					else
						MessageBox(hWnd, l.getMessage(Localized::PLAYER_IS_SLAVE).c_str(), l.getMessage(Localized::CANT_TRAVEL).c_str(), MB_OK);
				}
				else
				{
					for (HWND hItem : hItems)
						ShowWindow(hItem, SW_HIDE);

					game.setDisplayState(DisplayState::MENU);
					game.setBackground(Game::Background::CITY_MENU);
					updateWindow(hWnd);
				}
			}
		}
		break;
		}
	break;
	}
}

bool WorldMap::stylizeWindow(HWND hWnd, UINT m, WPARAM wp, LPARAM lp, LRESULT& result)
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

			// Assing background and text color
			SetTextColor(hdc, COLOR_WHITE);
			FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_ROMAN_RED));

			// Draw text
			int len = GetWindowTextLength(item->hwndItem);
			buf.resize(len + 1); // Resize buffer to contain button text
			GetWindowTextA(item->hwndItem, &buf[0], len + 1); // Write text into buffer
			DrawTextA(item->hDC, buf.c_str(), len, &item->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER); // Display text on button

			// Checking window to draw it using correct styles

			if (item->hwndItem == hItems[selectedCity] || item->hwndItem == hItems[selectedCity + 11])
			{
				FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_ROMAN_RED_PUSHED));
				DrawTextA(item->hDC, buf.c_str(), len, &item->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
				DrawEdge(hdc, &item->rcItem, EDGE_RAISED, BF_RECT);
				return true;
			}
			if (item->hwndItem == hItems[STAT_MAP])
			{
				// Set text font and background
				SelectObject(hdc, game.getFont(Game::FontSize::LARGE));
				SetBkMode(hdc, TRANSPARENT);

				// Assing background and text color
				SetTextColor(hdc, COLOR_WHITE);

				// Draw text
				int len = GetWindowTextLength(item->hwndItem);
				buf.resize(len + 1); // Resize buffer to contain button text
				GetWindowTextA(item->hwndItem, &buf[0], len + 1); // Write text into buffer

				FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_DARK_BLUE));
				DrawTextA(item->hDC, buf.c_str(), len, &item->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
				DrawEdge(hdc, &item->rcItem, EDGE_RAISED, BF_RECT);
				return true;
			}
			if (item->hwndItem == hItems[BUT_PLAYER_ICON_LIST] || item->hwndItem == hItems[BUT_PLAYER_ICON_MAP])
			{
				// Select image
				hBackgroundImage = (HBITMAP)LoadImage(0, "Data/Image/smallIcon.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
				// Filling background with selected image
				hBackgroundBrush = CreatePatternBrush(hBackgroundImage);
				FillRect(item->hDC, &item->rcItem, hBackgroundBrush);
				// Drawing edge
				DrawEdge(item->hDC, &item->rcItem, EDGE_RAISED, BF_RECT);
				return true;
			}
			return false;
		}
		break;
	}
	return false;
}

void WorldMap::selectCity(HWND hWnd, Item city)
{
	playSound(SoundEnum::SOUND_BUTTON_CLICK);
	RECT rect;

	selectedCity = city;

	if (currentCity == selectedCity)
		SendMessage(hItems[BUT_TRAVEL_LIST], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::ENTER).c_str());
	else
		SendMessage(hItems[BUT_TRAVEL_LIST], WM_SETTEXT, 0, (LPARAM)l.getMessage(Localized::TRAVEL).c_str());

	// Update button selection
	GetWindowRect(hItems[selectedCity], &rect);
	MapWindowPoints(HWND_DESKTOP, hWnd, (LPPOINT)&rect, 2);
	InvalidateRect(hWnd, &rect, 1);
	GetWindowRect(hItems[selectedCity + 11], &rect);
	MapWindowPoints(HWND_DESKTOP, hWnd, (LPPOINT)&rect, 2);
	InvalidateRect(hWnd, &rect, 1);
}

void WorldMap::saveToFile(const string& directory) const
{
	const string PATH = directory,
		FILE_WORLD = "World",
		FOLDER_CITIES = "Cities/",
		FORMAT = ".sav";

	// Opening file for saving
	ofstream fout(PATH + FILE_WORLD + FORMAT, ios::binary);

	if (!fout)
		throw new exception("Error: Couldn't open file for world's saving");

	fout << currentCity;

	fout.close();

	// Saving cities
	// Creating the folder for cities if it doesn't exist
	BOOL success = CreateDirectory((PATH + FOLDER_CITIES).c_str(), NULL);
	if (!success && GetLastError() != ERROR_ALREADY_EXISTS)
		throw new exception("Error: Couldn't create directory for save");

	ifstream finCityName("Data/Language/World/En_CityNames.lang");
	string cityName;
	for (int i = 0; i < Cities::CITIES_NUMBER; i++)
	{
		finCityName >> cityName;
		cityName += '/';
		// Creating the folder for city if it doesn't exist
		BOOL success = CreateDirectory((PATH + FOLDER_CITIES + cityName).c_str(), NULL);
		if (!success && GetLastError() != ERROR_ALREADY_EXISTS)
			throw new exception("Error: Couldn't create directory for save");

		cities[i].saveToFile(PATH + FOLDER_CITIES + cityName);
	}
}

void WorldMap::loadFromFile(const string& directory)
{
	const string PATH = directory,
		FILE_WORLD = "World",
		FOLDER_CITIES = "Cities/",
		FORMAT = ".sav";

	// Opening file for loading
	ifstream fin(PATH + FILE_WORLD + FORMAT, ios::binary);

	if (!fin)
		throw new exception("Error: Couldn't open file for world's loading");

	fin >> currentCity;
	selectedCity = currentCity;

	fin.close();

	// Loading cities
	ifstream finCityName("Data/Language/World/En_CityNames.lang");
	string cityName;
	cities.resize(Cities::CITIES_NUMBER);
	for (int i = 0; i < Cities::CITIES_NUMBER; i++)
	{
		finCityName >> cityName;
		cityName += '/';
		cities[i].loadFromFile(PATH + FOLDER_CITIES + cityName);
	}
}
