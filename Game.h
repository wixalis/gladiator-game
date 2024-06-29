#pragma once

class Game
{
private:
	MenuManager menuManager;
	unique_ptr<WorldMap> pWorldMap;
	unique_ptr<Fighting> pFighting;
	unique_ptr<Player> pPlayer;
	unique_ptr<StoryScreen> pStoryScreen;

	HFONT smallFont, mediumFont, largeFont;
	
	DisplayState displayState;

public:
	enum FontSize
	{
		SMALL,
		MEDIUM,
		LARGE
	};
	enum Background
	{
		MAIN_MENU,
		MAIN_MENU_LOAD,
		MAIN_MENU_NEW_GAME,
		MAIN_MENU_NEW_GAME_EQUIPMENT,
		MAIN_MENU_SETTINGS,
		MAIN_MENU_SPECIALS,
		CITY_MENU,
		CITY_MENU_ARENA,
		CITY_MENU_QUEST,
		CITY_MENU_MARKET,
		CITY_MENU_CHARACTER,
		CITY_MENU_ARENA_FIGHT,
		CITY_MENU_ARENA_BET,
		CITY_MENU_LANISTA,
		GAME_MENU,
		GAME_MENU_SAVE,
		GAME_MENU_LOAD,
		GAME_MENU_SETTINGS,
		WORLD_MAP,
		FIGHTING_QUEST_PERUGIA_SHANDAR_STREET,
		FIGHTING_QUEST_PERUGIA_SHANDAR_HOUSE,
		FIGHTING_ARENA
	};
	enum Progression
	{
		START,
		FIRST_VICTORY,
		ENOUGH_MONEY_FOR_FREEDOM,
		STAYED_AFTER_GLADIATOR_OFFER,
		STAYED_AFTER_CENTURION_OFFER,
		GAINED_FREEDOM,
		CHOSE_LEGION,
		CHOSE_LEGION_AS_FREE,
		JOINED_LEGION,
		CHOSE_REBELLION,
		CHOSE_REBELLION_AS_FREE,
		JOINED_REBELLION,
		PROGRESSION_ITEM_NUMBER
	};
	Game();
	Game(const Game&);
	Game& operator=(const Game&);
	~Game();

	void setPlayer(const Player&);
	void setWorldMap(const WorldMap&);
	void setFighting(const Fighting&);
	void setStoryScreen(const StoryScreen&);
	void setDisplayState(DisplayState);
	void setProgressionStage(Progression);
	void setSoundStatus(bool);
	void setAutoSaveStatus(bool);

	Player& getPlayer();
	WorldMap& getWorldMap() const;
	Fighting& getFighting() const;
	StoryScreen& getStoryScreen() const;
	MenuManager& getMenuManager();
	HFONT& getFont(FontSize);
	Progression getProgressionStage() const;
	bool getSoundStatus() const;
	bool getAutoSaveStatus() const;

	void drawWindow(HWND, HDC, int, int);
	void resizeWindow(int, int);
	void handleInput(HWND, UINT, WPARAM, LPARAM);
	bool stylizeWindow(HWND, UINT, WPARAM, LPARAM, LRESULT&);

private:
	Progression progressionStage;
	Background currentBackground;
	bool backgroundChanged;
	bool isSoundEnabled;
	bool isAutoSaveEnabled;

public:
	// Background
	void setBackground(Background);
	void backgroundChangeCompleted();
	void updateBackground();
	Background getBackground() const;
	bool isBackgroundChanged() const;

	// Save and load
	void saveToFile(bool isAutoSave_ = false);
	void loadFromFile(const string&);
};
