#pragma once
#include "IMenu.h"

class GameMenu :
	public IMenu
{
private:
	vector<HWND> hItems;
	vector<HWND> hSubItems;

	HBITMAP hBackgroundImage;
	HBRUSH hBackgroundBrush;

	int selected;

public:
	enum menuItem
	{
		BUT_RESUME,
		BUT_SAVE,
		BUT_LOAD,
		BUT_SETTINGS,
		BUT_EXIT_MENU,
		ITEM_NUMBER
	};
	enum LoadingItems
	{
		LOADING_STAT_LOAD,
		LOADING_LISTBOX_SAVES,
		LOADING_BUT_LOAD,
		LOADING_BUT_BACK,
		LOADING_ITEM_NUMBER
	};
	enum SettingsItem
	{
		SETTINGS_STAT_VIDEO,
		SETTINGS_STAT_SOUND,
		SETTINGS_STAT_AUTOSAVE,
		SETTINGS_BUT_SOUND,
		SETTINGS_BUT_AUTOSAVE,
		SETTINGS_BUT_BACK,
		SETTINGS_ITEM_NUMBER
	};

	GameMenu();
	GameMenu(HWND);
	GameMenu(const GameMenu&);
	GameMenu& operator=(const GameMenu&);
	virtual ~GameMenu();

	void drawMenu(HWND, HDC, int, int) override;
	void resizeMenu(int, int) override;
	void handleInput(HWND, UINT, WPARAM, LPARAM) override;
	bool stylizeWindow(HWND, UINT, WPARAM, LPARAM, LRESULT&) override;
};
