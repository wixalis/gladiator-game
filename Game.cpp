#include "stdafx.h"
#include "Game.h"

extern string buf;

Game::Game() :
	menuManager(),
	pWorldMap(make_unique<WorldMap>()),
	pFighting(make_unique<Fighting>()),
	pPlayer(make_unique<Player>()),
	displayState(DisplayState::MENU),
	currentBackground(Background::MAIN_MENU),
	backgroundChanged(true)
{
	smallFont = CreateFont(
		15,						// Size
		6,						// Width
		0,						// Lean angle
		0,						// Rotation angle
		FW_NORMAL,				// Thickness
		FALSE,					// Cursive
		FALSE,					// Underline
		FALSE,					// Crossed
		DEFAULT_CHARSET,		// Symbol set
		OUT_DEFAULT_PRECIS,		// Output preccision
		CLIP_DEFAULT_PRECIS,	// Clip precission
		DEFAULT_QUALITY,		// Output quality
		DEFAULT_PITCH |			// Pitch
		FF_ROMAN,				// Font family
		"SmallFont"				// Name
	);

	mediumFont = CreateFont(
		16,						// Size
		7,						// Width
		0,						// Lean angle
		0,						// Rotation angle
		FW_NORMAL,				// Thickness
		FALSE,					// Cursive
		FALSE,					// Underline
		FALSE,					// Crossed
		DEFAULT_CHARSET,		// Symbol set
		OUT_DEFAULT_PRECIS,		// Output preccision
		CLIP_DEFAULT_PRECIS,	// Clip precission
		DEFAULT_QUALITY,		// Output quality
		DEFAULT_PITCH |			// Pitch
		FF_ROMAN,				// Font family
		"MediumFont"			// Name
	);

	largeFont = CreateFont(
		20,						// Size
		8,						// Width
		0,						// Lean angle
		0,						// Rotation angle
		FW_NORMAL,				// Thickness
		FALSE,					// Cursive
		FALSE,					// Underline
		FALSE,					// Crossed
		DEFAULT_CHARSET,		// Symbol set
		OUT_DEFAULT_PRECIS,		// Output preccision
		CLIP_DEFAULT_PRECIS,	// Clip precission
		DEFAULT_QUALITY,		// Output quality
		DEFAULT_PITCH |			// Pitch
		FF_ROMAN,				// Font family
		"LargeFont"				// Name
	);
}

Game::Game(const Game& GAME) :
	menuManager(GAME.menuManager),
	pWorldMap(make_unique<WorldMap>(*GAME.pWorldMap)),
	pFighting(make_unique<Fighting>()),
	pPlayer(make_unique<Player>(*GAME.pPlayer)),
	displayState(GAME.displayState),
	currentBackground(GAME.currentBackground),
	backgroundChanged(GAME.backgroundChanged)
{
	// Small font
	if (GAME.smallFont != NULL)
	{
		LOGFONT lf;
		if (GetObject(GAME.smallFont, sizeof(LOGFONT), &lf))
			smallFont = CreateFontIndirect(&lf);
	}
	else
		smallFont = NULL;

	// Medium font
	if (GAME.mediumFont != NULL)
	{
		LOGFONT lf;
		if (GetObject(GAME.mediumFont, sizeof(LOGFONT), &lf))
			mediumFont = CreateFontIndirect(&lf);
	}
	else
		mediumFont = NULL;

	// Large font
	if (GAME.largeFont != NULL)
	{
		LOGFONT lf;
		if (GetObject(GAME.largeFont, sizeof(LOGFONT), &lf))
			largeFont = CreateFontIndirect(&lf);
	}
	else
		largeFont = NULL;
}

Game& Game::operator=(const Game& GAME)
{
	if (&GAME == this) return *this;

	// World map
	if (GAME.pWorldMap)
	{
		if (!pWorldMap)
			pWorldMap = make_unique<WorldMap>(*GAME.pWorldMap);
		else
			*pWorldMap = *GAME.pWorldMap;
	}
	else
		pWorldMap = nullptr;

	// Player
	if (GAME.pPlayer)
	{
		if (!pPlayer)
			pPlayer = make_unique<Player>(*GAME.pPlayer);
		else
			*pPlayer = *GAME.pPlayer;
	}
	else pPlayer = nullptr;

	pFighting = make_unique<Fighting>();

	menuManager = GAME.menuManager;

	// Small font
	if (smallFont != NULL)
	{
		DeleteObject(smallFont);
		smallFont = NULL;
	}
	if (GAME.smallFont != NULL)
	{
		LOGFONT lf;
		if (GetObject(GAME.smallFont, sizeof(LOGFONT), &lf))
			smallFont = CreateFontIndirect(&lf);
	}
	else
		smallFont = NULL;

	// Medium font
	if (mediumFont != NULL)
	{
		DeleteObject(mediumFont);
		mediumFont = NULL;
	}
	if (GAME.mediumFont != NULL)
	{
		LOGFONT lf;
		if (GetObject(GAME.mediumFont, sizeof(LOGFONT), &lf))
			mediumFont = CreateFontIndirect(&lf);
	}
	else
		mediumFont = NULL;

	// Large font
	if (largeFont != NULL)
	{
		DeleteObject(largeFont);
		largeFont = NULL;
	}
	if (GAME.largeFont != NULL)
	{
		LOGFONT lf;
		if (GetObject(GAME.largeFont, sizeof(LOGFONT), &lf))
			largeFont = CreateFontIndirect(&lf);
	}
	else
		largeFont = NULL;

	displayState = GAME.displayState;

	currentBackground = GAME.currentBackground;
	backgroundChanged = GAME.backgroundChanged;

	return *this;
}

Game::~Game()
{
	if (smallFont != NULL)
	{
		DeleteObject(smallFont);
		smallFont = NULL;
	}
	if (mediumFont != NULL)
	{
		DeleteObject(mediumFont);
		mediumFont = NULL;
	}
	if (largeFont != NULL)
	{
		DeleteObject(largeFont);
		largeFont = NULL;
	}
}

void Game::setPlayer(const Player& rPlayer)
{
	pPlayer.reset(new Player(rPlayer));
}

void Game::setWorldMap(const WorldMap& rWorldMap)
{
	pWorldMap.reset(new WorldMap(rWorldMap));
}

void Game::setFighting(const Fighting& rFighting)
{
	pFighting.reset(new Fighting(rFighting));
}

void Game::setDisplayState(DisplayState ds)
{
	displayState = ds;
}

Player& Game::getPlayer()
{
	return *pPlayer;
}

WorldMap& Game::getWorldMap() const
{
	return *pWorldMap;
}

Fighting & Game::getFighting() const
{
	return *pFighting;
}

MenuManager& Game::getMenuManager()
{
	return menuManager;
}

HFONT& Game::getFont(FontSize size)
{
	switch (size)
	{
	default: case FontSize::SMALL: return smallFont;
	case FontSize::MEDIUM: return mediumFont;
	case FontSize::LARGE: return largeFont;
	}
}

void Game::drawWindow(HWND hWnd, HDC hdc, int cx, int cy)
{
	switch (displayState)
	{
	default:case DisplayState::MENU:
		menuManager.drawMenu(hWnd, hdc, cx, cy);
		break;

	case DisplayState::WORLD_MAP:
		pWorldMap->drawWindow(hWnd, hdc, cx, cy);
		break;

	case DisplayState::FIGHTING:
		pFighting->drawWindow(hWnd, hdc, cx, cy);
		break;
	}
}

void Game::resizeWindow(int cx, int cy)
{
	switch (displayState)
	{
	default:case DisplayState::MENU:
		menuManager.resizeMenu(cx, cy);
		break;

	case DisplayState::WORLD_MAP:
		pWorldMap->resizeWindow(cx, cy);
		break;

	case DisplayState::FIGHTING:
		pFighting->resizeWindow(cx, cy);
		break;
	}
}

void Game::handleInput(HWND hWnd, UINT m, WPARAM wp, LPARAM lp)
{
	switch (displayState)
	{
	default:case DisplayState::MENU:
		menuManager.handleInput(hWnd, m, wp, lp);
		break;

	case DisplayState::WORLD_MAP:
		pWorldMap->handleInput(hWnd, m, wp, lp);
		break;

	case DisplayState::FIGHTING:
		pFighting->handleInput(hWnd, m, wp, lp);
		break;
	}
}

bool Game::stylizeWindow(HWND hWnd, UINT m, WPARAM wp, LPARAM lp)
{
	switch (displayState)
	{
	default:case DisplayState::MENU:
		return menuManager.stylizeWindow(hWnd, m, wp, lp);
		break;

	case DisplayState::WORLD_MAP:
		return pWorldMap->stylizeWindow(hWnd, m, wp, lp);
		break;

	case DisplayState::FIGHTING:
		return pFighting->stylizeWindow(hWnd, m, wp, lp);
		break;
	}
}

void Game::setBackground(Background b)
{
	currentBackground = b;
	backgroundChanged = true;
}

void Game::backgroundChangeCompleted()
{
	backgroundChanged = false;
}

void Game::updateBackground()
{
	backgroundChanged = true;
}

Game::Background Game::getBackground() const
{
	return currentBackground;
}

bool Game::isBackgroundChanged() const
{
	return backgroundChanged;
}

void Game::saveToFile()
{
	string path = "";
	const string FOLDER_SAVES = "Saves/", PLAYER_FOLDER = "Player/", GAME_DATA_FILE = "Game", FORMAT = ".sav";
	
	// Creating the folder of saved games if it doesn't exist
	BOOL success = CreateDirectory(FOLDER_SAVES.c_str(), NULL);
	if (!success && GetLastError() != ERROR_ALREADY_EXISTS)
		throw new exception("Error: Couldn't create directory for saving");

	// Creating the folder for current save
	// Naming the folder using current time
	auto now = time(nullptr);
	tm* now_tm = localtime(&now);
	char buffer[256];
	strftime(buffer, sizeof(buffer), "%d.%m.%y_%H-%M-%S", now_tm);
	
	// Name for save folder -- Player's name + Player's level + current date and time
	path += FOLDER_SAVES + game.getPlayer().getName() + " (" + to_string(game.getPlayer().getLevel()) + " lvl) -- " + buffer + "/";

	success = CreateDirectory(path.c_str(), NULL);
	if (!success && GetLastError() != ERROR_ALREADY_EXISTS)
		throw new exception("Error: Couldn't create directory for saving");

	// Saving game data
	ofstream fout(path + GAME_DATA_FILE + FORMAT, ios::binary);
	if (!fout)
		throw new exception("Error: Couldn't open file for game's saving");

	fout << game.getWorldMap().getCurrentCityIndex();

	fout.close();
	// Saving player
	buf = path + PLAYER_FOLDER;

	success = CreateDirectory(buf.c_str(), NULL);
	if (!success && GetLastError() != ERROR_ALREADY_EXISTS)
		throw new exception("Error: Couldn't create directory for player's saving");

	pPlayer->saveToFile(buf);
}

void Game::loadFromFile(const string& saveFolderName)
{
	const string FOLDER_SAVES = "Saves/", PLAYER_FOLDER = "Player/", GAME_DATA_FILE = "Game", FORMAT = ".sav";
	string path = FOLDER_SAVES + saveFolderName;

	// Loading game data
	ifstream fin(path + GAME_DATA_FILE + FORMAT, ios::binary);
	if (!fin)
		throw new exception("Error: Couldn't open file for game's loading");

	int currentCityIndex;
	fin >> currentCityIndex;
	game.getWorldMap().setCurrentCityIndex(currentCityIndex);

	fin.close();
	// Loading player
	buf = path + PLAYER_FOLDER;

	pPlayer->loadFromFile(buf);
}
