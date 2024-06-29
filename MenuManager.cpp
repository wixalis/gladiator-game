#include "stdafx.h"
#include "MenuManager.h"

MenuManager::MenuManager() : currentMenu(nullptr) { }

MenuManager::MenuManager(IMenu* pMenu) : currentMenu(pMenu) { }

MenuManager::MenuManager(const MenuManager& MM) : currentMenu(MM.currentMenu) { }

MenuManager& MenuManager::operator=(const MenuManager& MM)
{
	if (&MM == this) return *this;

	if (currentMenu)
		delete currentMenu;
	currentMenu = MM.currentMenu;
	return *this;
}

MenuManager::~MenuManager()
{
	if (currentMenu)
		delete currentMenu;
}

void MenuManager::setMenu(IMenu* pMenu)
{
	if (currentMenu)
		delete currentMenu;
	currentMenu = pMenu;
}

void MenuManager::drawMenu(HWND hWnd, HDC hDC, int cx, int cy)
{
	currentMenu->drawMenu(hWnd, hDC, cx, cy);
}

void MenuManager::resizeMenu(int cx, int cy)
{
	currentMenu->resizeMenu(cx, cy);
}

void MenuManager::handleInput(HWND hWnd, UINT m, WPARAM wp, LPARAM lp)
{
	currentMenu->handleInput(hWnd, m, wp, lp);
}

bool MenuManager::stylizeWindow(HWND hWnd, UINT m, WPARAM wp, LPARAM lp, LRESULT& result)
{
	return currentMenu->stylizeWindow(hWnd, m, wp, lp, result);
}
