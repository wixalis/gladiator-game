#pragma once

class IMenu
{
public:
	virtual ~IMenu();

	virtual void drawMenu(HWND, HDC, int, int) = 0;
	virtual void resizeMenu(int, int) = 0;
	virtual void handleInput(HWND, UINT, WPARAM, LPARAM) = 0;
	virtual bool stylizeWindow(HWND, UINT, WPARAM, LPARAM, LRESULT&) = 0;
};
