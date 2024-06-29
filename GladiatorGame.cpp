// GladiatorGame.cpp : Defines the entry point for the application.
// This is a game about gladiators in Ancient Rome. It is written as an educational project on C++ and WinAPI using OOP principles.

#include "stdafx.h"

char name[] = "Qwerty";

HINSTANCE hInst;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE , LPSTR lpCmdLine, int nCmdShow)
{
	hInst = hInstance;
	// 1. Creating the appearance of the window.
	WNDCLASS win;
	win.style = CS_HREDRAW | CS_VREDRAW;
	win.lpfnWndProc = WFunc;
	win.cbClsExtra = win.cbWndExtra = 0;
	win.hInstance = hInstance;
	win.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	win.hCursor = LoadCursor(NULL, IDC_ARROW);
	win.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	win.lpszMenuName = NULL;
	win.lpszClassName = name;

	// 2. Registering a window with the operating system.
	if (!RegisterClass(&win))
	{
		MessageBox(NULL, "Window is not registered", "Error", MB_OK);
		return -1;
	}

	// 3. Creating window.
	HWND hwnd;
	hwnd = CreateWindow(name, "The War of Spartacus",
						WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX | WS_VISIBLE,
						50, 50, 1366, 768, HWND_DESKTOP, NULL, hInstance, NULL);

	// 4. Creating messages pump.
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}
