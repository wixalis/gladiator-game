#pragma once

class MenuManager
{
private:
	IMenu* currentMenu;
public:
	MenuManager();
	MenuManager(IMenu*);
	MenuManager(const MenuManager&);
	MenuManager& operator=(const MenuManager&);
	~MenuManager();

	void setMenu(IMenu*);

	void drawMenu(HWND, HDC, int, int);
	void resizeMenu(int, int);
	void handleInput(HWND, UINT, WPARAM, LPARAM);
	bool stylizeWindow(HWND, UINT, WPARAM, LPARAM, LRESULT&);
};
