#include "stdafx.h"
#include "StoryScreen.h"

extern TCHAR str[256];
extern string buf;
extern string logStr;

extern HINSTANCE hInst;
extern Localization l;
extern Game game;

StoryScreen::StoryScreen() :
	hItems(),
	hBackgroundImage(NULL),
	hBackgroundBrush(NULL),
	currentScreen(SCREEN_NUMBER),
	isChoiceRequired(false),
	haveFought(false),
	haveWon(false),
	currentSubScreen(-1)
{ }

StoryScreen::StoryScreen(HWND hWnd) :
	hItems(),
	hBackgroundImage(NULL),
	hBackgroundBrush(NULL),
	currentScreen(SCREEN_NUMBER),
	isChoiceRequired(false),
	haveFought(false),
	haveWon(false),
	currentSubScreen(-1)
{ }

StoryScreen::StoryScreen(const StoryScreen& SC) :
	isChoiceRequired(SC.isChoiceRequired),
	currentSubScreen(SC.currentSubScreen),
	haveFought(SC.haveFought),
	haveWon(SC.haveWon)
{
	// Resizing items' vector
	int sz = SC.hItems.size();
	hItems.resize(sz);

	HWND hItem;
	RECT rect;
	char className[256], windowText[256];
	for (int i = 0; i < sz; i++)
	{
		// Obtaining the previous parameters
		hItem = SC.hItems[i];
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

	if (SC.hBackgroundImage != NULL)
	{
		BITMAP bm;
		GetObject(SC.hBackgroundImage, sizeof(BITMAP), &bm);
		hBackgroundImage = CreateBitmapIndirect(&bm);
	}
	else
		hBackgroundImage = NULL;

	if (SC.hBackgroundBrush != NULL)
	{
		LOGBRUSH lb;
		GetObject(SC.hBackgroundBrush, sizeof(LOGBRUSH), &lb);
		hBackgroundBrush = CreateBrushIndirect(&lb);
	}
	else
		hBackgroundBrush = NULL;
}

StoryScreen& StoryScreen::operator=(const StoryScreen& SC)
{
	if (&SC == this) return *this;

	// Destroying the previous items of the menu
	for (HWND hItem : hItems)
		if (hItem != NULL)
			DestroyWindow(hItem);

	// Resizing items' vector
	int sz = SC.hItems.size();
	hItems.resize(sz);

	HWND hItem;
	RECT rect;
	char className[256], windowText[256];
	for (int i = 0; i < sz; i++)
	{
		// Obtaining the previous parameters
		hItem = SC.hItems[i];
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

	// Image
	if (hBackgroundImage != NULL)
		DeleteObject(hBackgroundImage);

	if (SC.hBackgroundImage != NULL)
	{
		BITMAP bm;
		GetObject(SC.hBackgroundImage, sizeof(BITMAP), &bm);
		hBackgroundImage = CreateBitmapIndirect(&bm);
	}
	else
		hBackgroundImage = NULL;

	if (hBackgroundBrush != NULL)
		DeleteObject(hBackgroundBrush);

	if (SC.hBackgroundBrush != NULL)
	{
		LOGBRUSH lb;
		GetObject(SC.hBackgroundBrush, sizeof(LOGBRUSH), &lb);
		hBackgroundBrush = CreateBrushIndirect(&lb);
	}
	else
		hBackgroundBrush = NULL;

	isChoiceRequired = SC.isChoiceRequired;
	haveFought = SC.haveFought;
	haveWon = SC.haveWon;
	currentSubScreen = SC.currentSubScreen;

	return *this;
}

StoryScreen::~StoryScreen()
{
	for (HWND hItem : hItems)
		if (hItem != NULL)
			DestroyWindow(hItem);

	if (hBackgroundImage != NULL)
		DeleteObject(hBackgroundImage);

	if (hBackgroundBrush != NULL)
		DeleteObject(hBackgroundBrush);
}

void StoryScreen::drawMenu(HWND hWnd, HDC hdc, int cx, int cy)
{
	const string DIRECTORY = "Data/Image/";
	const string FORMAT = ".bmp";
	string path("");

	RECT rect;
	GetClientRect(hWnd, &rect);

	// TODO: different background
	// 1. Background
	if (game.isBackgroundChanged())
	{
		switch (currentScreen)
		{
		case WELCOME_SCREEN:
		{
			path = DIRECTORY + "Story/blackScreen" + FORMAT; break; // TODO
		}
		break;

		case STOLEN_MONEY:
		{
			switch (currentSubScreen)
			{
			case STOLEN_MONEY_SCREEN_STAT_1: path = DIRECTORY + "Story/blackScreen" + FORMAT; break; // TODO
			//case STOLEN_MONEY_SCREEN_STAT_2: break; TODO
			//case STOLEN_MONEY_SCREEN_STAT_3: break; TODO
			//case STOLEN_MONEY_SCREEN_STAT_ESCAPE: break; TODO
			//case STOLEN_MONEY_SCREEN_STAT_STAY: break; TODO
			default: path = DIRECTORY + "Story/blackScreen" + FORMAT; break;
			}
		}
		break;

		case GAINED_FREEDOM:
		{
			switch (currentSubScreen)
			{
			case GAINED_FREEDOM_SCREEN_STAT_1: path = DIRECTORY + "Story/gainedFreedom" + FORMAT; break;
			//case GAINED_FREEDOM_SCREEN_STAT_2: break; TODO
			//case GAINED_FREEDOM_SCREEN_STAT_CHOSE_REBELS: break; TODO 
			//case GAINED_FREEDOM_SCREEN_STAT_CHOSE_LEGION: break; TODO
			default: path = DIRECTORY + "Story/blackScreen" + FORMAT; break;
			}
		}
		break;

		case QUEST_PERUGIA_SHANDAR:
		{
			switch (currentSubScreen)
			{
			case SHANDAR_STAT_INTRO: path = DIRECTORY + "Quest/Perugia/Shandar/intro" + FORMAT; break; 
			case SHANDAR_STAT_INTRO2: path = DIRECTORY + "Quest/Perugia/Shandar/intro" + FORMAT; break; 
			case SHANDAR_STAT_CHOICE1_ANSWER_WHO: path = DIRECTORY + "Quest/Perugia/Shandar/intro" + FORMAT; break; 
			case SHANDAR_STAT_CHOICE1_ANSWER_WHY: path = DIRECTORY + "Quest/Perugia/Shandar/intro" + FORMAT; break; 
			case SHANDAR_STAT_CHOICE1_ANSWER_WHERE: path = DIRECTORY + "Quest/Perugia/Shandar/choice1" + FORMAT; break; 
			case SHANDAR_BUT_COMEBACK_NIGHT: path = DIRECTORY + "Quest/Perugia/Shandar/lostQuest" + FORMAT; break; 
			case SHANDAR_STAT_COMEBACK_NIGHT: path = DIRECTORY + "Quest/Perugia/Shandar/comebackNight" + FORMAT; break;
			case SHANDAR_STAT_CHOICE2_ANSWER_TALK: path = DIRECTORY + "Quest/Perugia/Shandar/choice1" + FORMAT; break;
			case SHANDAR_STAT_CHOICE3_STRENGTH_LOSE: path = DIRECTORY + "Quest/Perugia/Shandar/choice3Lose" + FORMAT; break;
			case SHANDAR_STAT_CHOICE3_CHARISMA_LOSE: path = DIRECTORY + "Quest/Perugia/Shandar/choice3Lose" + FORMAT; break;
			case SHANDAR_STAT_ENTER_HOUSE: path = DIRECTORY + "Quest/Perugia/Shandar/enterHouse" + FORMAT; break;
			case SHANDAR_STAT_ENTER_HOUSE2: path = DIRECTORY + "Quest/Perugia/Shandar/enterHouse" + FORMAT; break;
			case SHANDAR_STAT_CHOICE5_FIGHT_WIN: path = DIRECTORY + "Quest/Perugia/Shandar/fightWin" + FORMAT; break;
			case SHANDAR_STAT_CHOICE6_EXTORT: path = DIRECTORY + "Quest/Perugia/Shandar/fightWin" + FORMAT; break;
			case SHANDAR_STAT_CHOICE6_EXILE: path = DIRECTORY + "Quest/Perugia/Shandar/fightWin" + FORMAT; break;
			case SHANDAR_STAT_FOUND_GIRL: path = DIRECTORY + "Quest/Perugia/Shandar/foundGirl" + FORMAT; break;
			case SHANDAR_STAT_GREET_GIRL: path = DIRECTORY + "Quest/Perugia/Shandar/foundGirl" + FORMAT; break;
			case SHANDAR_STAT_RETURNED_DAUGHTER: path = DIRECTORY + "Quest/Perugia/Shandar/reward" + FORMAT; break;
			case SHANDAR_STAT_REWARD: path = DIRECTORY + "Quest/Perugia/Shandar/reward" + FORMAT; break;
			case SHANDAR_STAT_GOODBYE: path = DIRECTORY + "Quest/Perugia/Shandar/reward" + FORMAT; break;

			case SHANDAR_STAT_CHOICE3_STRENGTH_WIN: path = DIRECTORY + "Quest/Perugia/Shandar/choice3Win" + FORMAT; break; 
			case SHANDAR_STAT_CHOICE3_CHARISMA_WIN: path = DIRECTORY + "Quest/Perugia/Shandar/choice3Win" + FORMAT; break; 
			case SHANDAR_STAT_CHOICE4_NIGHT_DEXTERITY_WIN: path = DIRECTORY + "Quest/Perugia/Shandar/comebackNight" + FORMAT; break; 
			case SHANDAR_STAT_CHOICE4_NIGHT_DEXTERITY_LOSE: path = DIRECTORY + "Quest/Perugia/Shandar/comebackNight" + FORMAT; break; 
			case SHANDAR_STAT_CHOICE5_CHARISMA_WIN: path = DIRECTORY + "Quest/Perugia/Shandar/choice5Charisma" + FORMAT; break; 
			case SHANDAR_STAT_CHOICE5_CHARISMA_LOSE: path = DIRECTORY + "Quest/Perugia/Shandar/choice5Charisma" + FORMAT; break; 
			case SHANDAR_STAT_CHOICE6_EXECUTED: path = DIRECTORY + "Quest/Perugia/Shandar/leaveLoud" + FORMAT; break; 
			case SHANDAR_STAT_CHOICE5_FIGHT_LOSE: path = DIRECTORY + "Quest/Perugia/Shandar/lostQuest" + FORMAT; break; 
			case SHANDAR_STAT_LOST_QUEST: path = DIRECTORY + "Quest/Perugia/Shandar/lostQuest" + FORMAT; break; 
			case SHANDAR_STAT_LEAVE_QUIET: path = DIRECTORY + "Quest/Perugia/Shandar/enterHouse" + FORMAT; break; 
			case SHANDAR_STAT_LEAVE_LOUD: path = DIRECTORY + "Quest/Perugia/Shandar/leaveLoud" + FORMAT; break; 
			case SHANDAR_STAT_THANKS: path = DIRECTORY + "Quest/Perugia/Shandar/thanks" + FORMAT; break; 
			case SHANDAR_STAT_END: path = DIRECTORY + "Quest/Perugia/Shandar/thanks" + FORMAT; break; 

			default: path = DIRECTORY + "Story/blackScreen" + FORMAT; break;
			}
		}
		break;

		default: path = DIRECTORY + "Story/blackScreen" + FORMAT; break;
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
}

void StoryScreen::resizeMenu(int cx, int cy)
{
	int x, y, i;
	switch (currentScreen)
	{
	case WELCOME_SCREEN:
	{
		y = cy - 120;
		const int ITEM_HEIGHT = 45, DISTANCE = 10, ITEM_WIDTH = 300;
		MoveWindow(hItems[WELCOME_SCREEN_STAT_DATE], cx - ITEM_WIDTH / 2, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[WELCOME_SCREEN_STAT_TEXT], cx - 250, y + ITEM_HEIGHT + DISTANCE, 500, 250, TRUE);
	}
	break;

	case START_GAME_SCREEN:
	{
		MoveWindow(hItems[0], cx - 250, cy - 120, 500, 220, TRUE);
	}
	break;

	case FIRST_VICTORY:
	{
		MoveWindow(hItems[0], cx - 250, cy - 120, 500, 150, TRUE);
	}
	break;

	case STOLEN_MONEY:
	{
		const int ITEM_HEIGHT = 280, ITEM_WIDTH = 500, BUTTON_HEIGHT = 30, BUTTON_WIDTH = 150, DISTANCE = 10;

		// Static windows
		MoveWindow(hItems[STOLEN_MONEY_SCREEN_STAT_1], cx - 250, cy - 200, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[STOLEN_MONEY_SCREEN_STAT_2], cx - 250, cy - 200, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[STOLEN_MONEY_SCREEN_STAT_3], cx - 250, cy - 200, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[STOLEN_MONEY_SCREEN_STAT_STAY], cx - 250, cy - 200, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[STOLEN_MONEY_SCREEN_STAT_ESCAPE], cx - 250, cy - 200, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		// Buttons
		MoveWindow(hItems[STOLEN_MONEY_SCREEN_BUT_ESCAPE], cx - BUTTON_WIDTH - DISTANCE, cy + 200, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);
		MoveWindow(hItems[STOLEN_MONEY_SCREEN_BUT_STAY], cx + DISTANCE, cy + 200, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);
	}
	break;

	case CENTURION_OFFER:
	{
		const int ITEM_HEIGHT = 350, ITEM_WIDTH = 500, BUTTON_HEIGHT = 30, BUTTON_WIDTH = 200, DISTANCE = 10;

		// Static windows
		MoveWindow(hItems[CENTURION_OFFER_SCREEN_STAT_1], cx - 250, cy - 200, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[CENTURION_OFFER_SCREEN_STAT_2], cx - 250, cy - 200, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[CENTURION_OFFER_SCREEN_STAT_ACCEPT], cx - 250, cy - 200, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[CENTURION_OFFER_SCREEN_STAT_DECLINE], cx - 250, cy - 200, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		// Buttons
		MoveWindow(hItems[CENTURION_OFFER_SCREEN_BUT_ACCEPT], cx - BUTTON_WIDTH - DISTANCE, cy + 200, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);
		MoveWindow(hItems[CENTURION_OFFER_SCREEN_BUT_DECLINE], cx + DISTANCE, cy + 200, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);
	}
	break;

	case GAINED_FREEDOM:
	{
		const int ITEM_HEIGHT = 350, ITEM_WIDTH = 500, BUTTON_HEIGHT = 35, BUTTON_WIDTH = 250, DISTANCE = 20;

		// Static windows
		MoveWindow(hItems[GAINED_FREEDOM_SCREEN_STAT_1], cx - 250, cy - 200, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[GAINED_FREEDOM_SCREEN_STAT_2], cx - 250, cy - 200, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[GAINED_FREEDOM_SCREEN_STAT_CHOSE_REBELS], cx - 250, cy - 200, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[GAINED_FREEDOM_SCREEN_STAT_CHOSE_LEGION], cx - 250, cy - 200, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		// Buttons
		MoveWindow(hItems[GAINED_FREEDOM_SCREEN_BUT_CHOOSE_REBELS], cx - BUTTON_WIDTH - DISTANCE, cy + 250, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);
		MoveWindow(hItems[GAINED_FREEDOM_SCREEN_BUT_CHOOSE_LEGION], cx + DISTANCE, cy + 250, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);
	}
	break;

	case QUEST_PERUGIA_SHANDAR:
	{
		const int ITEM_HEIGHT = 200, ITEM_WIDTH = 500, BUTTON_HEIGHT = 90, BUTTON_WIDTH = 400, DISTANCE = 20;
		int butX, butY;
		x = cx - ITEM_WIDTH, y = cy - ITEM_HEIGHT / 2;
		butX = cx + BUTTON_WIDTH / 2, butY = cy - (BUTTON_HEIGHT + DISTANCE) * 2;

		// Intro
		MoveWindow(hItems[SHANDAR_STAT_INTRO], x, y + ITEM_HEIGHT, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[SHANDAR_BUT_INTRO], butX, butY, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);

		// Choice 1 - talk with Shandar
		MoveWindow(hItems[SHANDAR_STAT_INTRO2], x, y + ITEM_HEIGHT, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		for (i = SHANDAR_BUT_CHOICE1_QUESTION_WHERE; i <= SHANDAR_BUT_CHOICE1_LEAVE; i++)
		{
			MoveWindow(hItems[i], butX, butY, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);
			butY += BUTTON_HEIGHT + DISTANCE;
		}
		butY = cy - (BUTTON_HEIGHT + DISTANCE) * 2;
		
		MoveWindow(hItems[SHANDAR_STAT_CHOICE1_ANSWER_WHO], x, y + ITEM_HEIGHT, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		MoveWindow(hItems[SHANDAR_STAT_CHOICE1_ANSWER_WHY], x, y + ITEM_HEIGHT, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		// Choice 2 - guards at entrance
		MoveWindow(hItems[SHANDAR_STAT_CHOICE1_ANSWER_WHERE], x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		for (i = SHANDAR_BUT_CHOICE2_TALK; i <= SHANDAR_BUT_CHOICE2_LEAVE; i++)
		{
			MoveWindow(hItems[i], butX, butY, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);
			butY += BUTTON_HEIGHT + DISTANCE;
		}
		butY = cy - (BUTTON_HEIGHT + DISTANCE) * 2;

		// Choice 3 - guards at entrance
		MoveWindow(hItems[SHANDAR_STAT_CHOICE2_ANSWER_TALK], x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		for (i = SHANDAR_BUT_CHOICE3_FIGHT; i <= SHANDAR_BUT_CHOICE3_CHARISMA; i++)
		{
			MoveWindow(hItems[i], butX, butY, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);
			butY += BUTTON_HEIGHT + DISTANCE;
		}
		butY = cy - (BUTTON_HEIGHT + DISTANCE) * 2;

		MoveWindow(hItems[SHANDAR_STAT_CHOICE3_STRENGTH_WIN], x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		MoveWindow(hItems[SHANDAR_STAT_CHOICE3_STRENGTH_LOSE], x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		MoveWindow(hItems[SHANDAR_STAT_CHOICE3_CHARISMA_WIN], x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		MoveWindow(hItems[SHANDAR_STAT_CHOICE3_CHARISMA_LOSE], x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		// Comeback at night
		MoveWindow(hItems[SHANDAR_BUT_COMEBACK_NIGHT], butX, butY, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);

		// Choice 4 - enter house at night
		MoveWindow(hItems[SHANDAR_STAT_COMEBACK_NIGHT], x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		for (i = SHANDAR_BUT_CHOICE4_NIGHT_ENTER; i <= SHANDAR_BUT_CHOICE4_NIGHT_DEXTERITY; i++)
		{
			MoveWindow(hItems[i], butX, butY, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);
			butY += BUTTON_HEIGHT + DISTANCE;
		}
		butY = cy - (BUTTON_HEIGHT + DISTANCE) * 2;

		MoveWindow(hItems[SHANDAR_STAT_CHOICE4_NIGHT_DEXTERITY_WIN], x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		MoveWindow(hItems[SHANDAR_STAT_CHOICE4_NIGHT_DEXTERITY_LOSE], x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		// Enter house
		MoveWindow(hItems[SHANDAR_STAT_ENTER_HOUSE], x, y - ITEM_HEIGHT, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[SHANDAR_BUT_ENTER_HOUSE], butX, butY, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);

		// Choice 5 - speak with Lucilius
		MoveWindow(hItems[SHANDAR_STAT_ENTER_HOUSE2], x, y - ITEM_HEIGHT, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		for (i = SHANDAR_BUT_CHOICE5_FIGHT; i <= SHANDAR_BUT_CHOICE5_CHARISMA; i++)
		{
			MoveWindow(hItems[i], butX, butY, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);
			butY += BUTTON_HEIGHT + DISTANCE;
		}
		butY = cy - (BUTTON_HEIGHT + DISTANCE) * 2;

		MoveWindow(hItems[SHANDAR_STAT_CHOICE5_FIGHT_LOSE], x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		MoveWindow(hItems[SHANDAR_STAT_LOST_QUEST], x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		MoveWindow(hItems[SHANDAR_STAT_CHOICE5_CHARISMA_WIN], x, y - ITEM_HEIGHT, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		MoveWindow(hItems[SHANDAR_STAT_CHOICE5_CHARISMA_LOSE], x, y - ITEM_HEIGHT, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		// Choice 6 - kill or spare Lucilius
		MoveWindow(hItems[SHANDAR_STAT_CHOICE5_FIGHT_WIN], x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		for (i = SHANDAR_BUT_CHOICE6_SPARE; i <= SHANDAR_BUT_CHOICE6_EXILE; i++)
		{
			MoveWindow(hItems[i], butX, butY, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);
			butY += BUTTON_HEIGHT + DISTANCE;
		}
		butY = cy - (BUTTON_HEIGHT + DISTANCE) * 2;

		MoveWindow(hItems[SHANDAR_STAT_CHOICE6_EXECUTED], x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		MoveWindow(hItems[SHANDAR_STAT_CHOICE6_EXTORT], x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		MoveWindow(hItems[SHANDAR_STAT_CHOICE6_EXILE], x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		for (i = SHANDAR_BUT_CHOICE6_EXTORT_EXECUTE; i <= SHANDAR_BUT_CHOICE6_WHERE_GIRL; i++)
		{
			MoveWindow(hItems[i], butX, butY, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);
			butY += BUTTON_HEIGHT + DISTANCE;
		}
		butY = cy - (BUTTON_HEIGHT + DISTANCE) * 2;

		// Found girl
		MoveWindow(hItems[SHANDAR_STAT_FOUND_GIRL], x, y - ITEM_HEIGHT, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[SHANDAR_BUT_GREET_GIRL], butX, butY, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);

		MoveWindow(hItems[SHANDAR_STAT_GREET_GIRL], x, y - ITEM_HEIGHT, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[SHANDAR_BUT_LETS_LEAVE], butX, butY, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);

		MoveWindow(hItems[SHANDAR_STAT_LEAVE_QUIET], x, y - ITEM_HEIGHT, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		MoveWindow(hItems[SHANDAR_STAT_LEAVE_LOUD], x, y - ITEM_HEIGHT, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		// Returned girl
		MoveWindow(hItems[SHANDAR_STAT_RETURNED_DAUGHTER], x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		for (i = SHANDAR_BUT_ASK_REWARD; i <= SHANDAR_BUT_GOODBYE; i++)
		{
			MoveWindow(hItems[i], butX, butY, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);
			butY += BUTTON_HEIGHT + DISTANCE;
		}
		butY = cy - (BUTTON_HEIGHT + DISTANCE) * 2;
		
		MoveWindow(hItems[SHANDAR_STAT_REWARD], x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		MoveWindow(hItems[SHANDAR_STAT_GOODBYE], x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[SHANDAR_BUT_NAME], butX, butY, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);

		MoveWindow(hItems[SHANDAR_STAT_THANKS], x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		MoveWindow(hItems[SHANDAR_STAT_END], x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
	}
	break;
	}
}

void StoryScreen::handleInput(HWND hWnd, UINT m, WPARAM wp, LPARAM lp)
{
	switch (m)
	{
	case WM_COMMAND:
	{
		switch (currentScreen)
		{
		case STOLEN_MONEY:
		{
			if ((HWND)lp == hItems[STOLEN_MONEY_SCREEN_BUT_ESCAPE])
			{
				// Player escapes and becomes free
				game.setProgressionStage(Game::Progression::CHOSE_REBELLION);
				game.getPlayer().setFreedom(true);
				currentSubScreen = STOLEN_MONEY_SCREEN_STAT_ESCAPE;
				isChoiceRequired = false;

				for (HWND item : hItems)
					ShowWindow(item, SW_HIDE);
				ShowWindow(hItems[currentSubScreen], SW_SHOW);
				game.updateBackground();
				updateWindow(hWnd);
			}

			if ((HWND)lp == hItems[STOLEN_MONEY_SCREEN_BUT_STAY])
			{
				// Player stays as gladiator on arena
				game.setProgressionStage(Game::Progression::STAYED_AFTER_GLADIATOR_OFFER);
				currentSubScreen = STOLEN_MONEY_SCREEN_STAT_STAY;
				isChoiceRequired = false;

				for (HWND item : hItems)
					ShowWindow(item, SW_HIDE);
				ShowWindow(hItems[currentSubScreen], SW_SHOW);
				game.updateBackground();
				updateWindow(hWnd);
			}
		}
		break;

		case CENTURION_OFFER:
		{
			if ((HWND)lp == hItems[CENTURION_OFFER_SCREEN_BUT_ACCEPT])
			{
				// Player joins legion
				game.setProgressionStage(Game::Progression::CHOSE_LEGION);
				game.getPlayer().setFreedom(true);
				currentSubScreen = CENTURION_OFFER_SCREEN_STAT_ACCEPT;
				isChoiceRequired = false;

				for (HWND item : hItems)
					ShowWindow(item, SW_HIDE);
				ShowWindow(hItems[currentSubScreen], SW_SHOW);
				game.updateBackground();
				updateWindow(hWnd);
			}

			if ((HWND)lp == hItems[CENTURION_OFFER_SCREEN_BUT_DECLINE])
			{
				// Player stays as gladiator on arena
				game.setProgressionStage(Game::Progression::STAYED_AFTER_CENTURION_OFFER);
				currentSubScreen = CENTURION_OFFER_SCREEN_STAT_DECLINE;
				isChoiceRequired = false;

				for (HWND item : hItems)
					ShowWindow(item, SW_HIDE);
				ShowWindow(hItems[currentSubScreen], SW_SHOW);
				game.updateBackground();
				updateWindow(hWnd);
			}
		}
		break;

		case GAINED_FREEDOM:
		{
			if ((HWND)lp == hItems[GAINED_FREEDOM_SCREEN_BUT_CHOOSE_REBELS])
			{
				// Player joins rebels
				game.setProgressionStage(Game::Progression::CHOSE_REBELLION_AS_FREE);
				currentSubScreen = GAINED_FREEDOM_SCREEN_STAT_CHOSE_REBELS;
				isChoiceRequired = false;

				for (HWND item : hItems)
					ShowWindow(item, SW_HIDE);
				ShowWindow(hItems[currentSubScreen], SW_SHOW);
				game.updateBackground();
				updateWindow(hWnd);
			}

			if ((HWND)lp == hItems[GAINED_FREEDOM_SCREEN_BUT_CHOOSE_LEGION])
			{
				// Player joins legion
				game.setProgressionStage(Game::Progression::CHOSE_LEGION_AS_FREE);
				currentSubScreen = GAINED_FREEDOM_SCREEN_STAT_CHOSE_LEGION;
				isChoiceRequired = false;

				for (HWND item : hItems)
					ShowWindow(item, SW_HIDE);
				ShowWindow(hItems[currentSubScreen], SW_SHOW);
				game.updateBackground();
				updateWindow(hWnd);
			}
		}
		break;

		case QUEST_PERUGIA_SHANDAR:
		{
			switch (currentSubScreen)
			{
			case SHANDAR_STAT_INTRO:
			{
				// Meet Shandar
				if ((HWND)lp == hItems[SHANDAR_BUT_INTRO])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_INTRO], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_INTRO], SW_HIDE);

					// Show new windows
					ShowWindow(hItems[SHANDAR_STAT_INTRO2], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHO], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHY], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHERE], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE1_LEAVE], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_STAT_INTRO2;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
			}
			break;

			case SHANDAR_STAT_INTRO2:
			{
				// Who is Lucilius
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHO])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_INTRO2], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHO], SW_HIDE);

					// Show new windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE1_ANSWER_WHO], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_STAT_CHOICE1_ANSWER_WHO;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
				// Why should I help you
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHY])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_INTRO2], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHY], SW_HIDE);

					// Show new windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE1_ANSWER_WHY], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_STAT_CHOICE1_ANSWER_WHY;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
				// Where is Lucilius
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHERE])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_INTRO2], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHO], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHY], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHERE], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE1_LEAVE], SW_HIDE);

					// Show new windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE1_ANSWER_WHERE], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE2_TALK], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE2_LEAVE], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_STAT_CHOICE1_ANSWER_WHERE;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
				// Leave
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE1_LEAVE])
				{
					// Destroying all windows
					for (HWND hItem : hItems)
						if (hItem != NULL)
							DestroyWindow(hItem);
					hItems.clear();

					game.setDisplayState(DisplayState::MENU);
					game.updateBackground();

					updateWindow(hWnd);
				}
			}
			break;

			case SHANDAR_STAT_CHOICE1_ANSWER_WHO:
			{
				// Why should I help you
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHY])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE1_ANSWER_WHO], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHY], SW_HIDE);

					// Show new windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE1_ANSWER_WHY], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_STAT_CHOICE1_ANSWER_WHY;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
				// Where is Lucilius
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHERE])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE1_ANSWER_WHO], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHO], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHY], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHERE], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE1_LEAVE], SW_HIDE);

					// Show new windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE1_ANSWER_WHERE], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE2_TALK], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE2_LEAVE], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_STAT_CHOICE1_ANSWER_WHERE;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
				// Leave
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE1_LEAVE])
				{
					// Destroying all windows
					for (HWND hItem : hItems)
						if (hItem != NULL)
							DestroyWindow(hItem);
					hItems.clear();

					game.setDisplayState(DisplayState::MENU);
					game.updateBackground();

					updateWindow(hWnd);
				}
			}
			break;

			case SHANDAR_STAT_CHOICE1_ANSWER_WHY:
			{
				// Who is Lucilius
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHO])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE1_ANSWER_WHY], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHO], SW_HIDE);

					// Show new windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE1_ANSWER_WHO], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_STAT_CHOICE1_ANSWER_WHO;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
				// Where is Lucilius
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHERE])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE1_ANSWER_WHY], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHO], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHY], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHERE], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE1_LEAVE], SW_HIDE);

					// Show new windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE1_ANSWER_WHERE], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE2_TALK], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE2_LEAVE], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_STAT_CHOICE1_ANSWER_WHERE;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
				// Leave
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE1_LEAVE])
				{
					// Destroying all windows
					for (HWND hItem : hItems)
						if (hItem != NULL)
							DestroyWindow(hItem);
					hItems.clear();

					game.setDisplayState(DisplayState::MENU);
					game.updateBackground();

					updateWindow(hWnd);
				}
			}
			break;

			case SHANDAR_STAT_CHOICE1_ANSWER_WHERE:
			{
				// Talk with guards
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE2_TALK])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE1_ANSWER_WHERE], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE2_TALK], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE2_LEAVE], SW_HIDE);

					// Show new windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE2_ANSWER_TALK], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE3_FIGHT], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE3_STRENGTH], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE3_CHARISMA], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_STAT_CHOICE2_ANSWER_TALK;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
				// Back down
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE2_LEAVE])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE1_ANSWER_WHERE], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE2_TALK], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE2_LEAVE], SW_HIDE);

					// Show new windows
					ShowWindow(hItems[SHANDAR_BUT_COMEBACK_NIGHT], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_BUT_COMEBACK_NIGHT;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
			}
			break;

			case SHANDAR_BUT_COMEBACK_NIGHT:
			{
				// Come back at night
				if ((HWND)lp == hItems[SHANDAR_BUT_COMEBACK_NIGHT])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_BUT_COMEBACK_NIGHT], SW_HIDE);

					// Show new windows
					ShowWindow(hItems[SHANDAR_STAT_COMEBACK_NIGHT], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE4_NIGHT_ENTER], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE4_NIGHT_DEXTERITY], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_STAT_COMEBACK_NIGHT;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
			}
			break;

			case SHANDAR_STAT_COMEBACK_NIGHT:
			{
				// Enter into the house
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE4_NIGHT_ENTER])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_COMEBACK_NIGHT], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE4_NIGHT_ENTER], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE4_NIGHT_DEXTERITY], SW_HIDE);

					// Show new windows
					ShowWindow(hItems[SHANDAR_STAT_ENTER_HOUSE], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_ENTER_HOUSE], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_STAT_ENTER_HOUSE;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
				// Dexterity - Sneak into the house
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE4_NIGHT_DEXTERITY])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_COMEBACK_NIGHT], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE4_NIGHT_ENTER], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE4_NIGHT_DEXTERITY], SW_HIDE);

					// Check if player has enough dexterity
					if (game.getPlayer().getDexterity() >= ATTRIBUTES_NEEDED)
					{
						// Show new windows
						ShowWindow(hItems[SHANDAR_STAT_CHOICE4_NIGHT_DEXTERITY_WIN], SW_SHOW);

						// Update subscreen
						currentSubScreen = SHANDAR_STAT_CHOICE4_NIGHT_DEXTERITY_WIN;
					}
					else
					{
						// Show new windows
						ShowWindow(hItems[SHANDAR_STAT_CHOICE4_NIGHT_DEXTERITY_LOSE], SW_SHOW);

						// Update subscreen
						currentSubScreen = SHANDAR_STAT_CHOICE4_NIGHT_DEXTERITY_LOSE;
					}

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
			}
			break;

			case SHANDAR_STAT_CHOICE2_ANSWER_TALK:
			{
				// Fight
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE3_FIGHT])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE2_ANSWER_TALK], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE3_FIGHT], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE3_STRENGTH], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE3_CHARISMA], SW_HIDE);

					// Display fighting
					game.setDisplayState(DisplayState::FIGHTING);
					game.setBackground(Game::Background::FIGHTING_QUEST_PERUGIA_SHANDAR_STREET);
					updateWindow(hWnd);

					// Fight
					FightStatus fightStatus = game.getFighting().fight(
						hWnd,
						game.getPlayer(),
						*generateNPC()
					);

					// Fight result
					switch (fightStatus)
					{
					case FightStatus::OPPONENT_LOST:
					{
						haveWon = true;
						// Update subscreen
						currentSubScreen = SHANDAR_STAT_ENTER_HOUSE;
					}
					break;

					case FightStatus::OPPONNENT_SURRENDERED:
					{
						haveWon = true;
						// Update subscreen
						currentSubScreen = SHANDAR_STAT_ENTER_HOUSE;
					}
					break;

					case FightStatus::PLAYER_SURRENDERED:
					{
						// Update subscreen
						currentSubScreen = SHANDAR_BUT_COMEBACK_NIGHT;

						// Update window
						game.updateBackground();
						updateWindow(hWnd);
					}
					break;

					case FightStatus::PLAYER_LOST:
					{
						// Update subscreen
						currentSubScreen = SHANDAR_BUT_COMEBACK_NIGHT;

						// Update window
						game.updateBackground();
						updateWindow(hWnd);
					}
					break;
					}
					return;
				}
				// Strength
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE3_STRENGTH])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE2_ANSWER_TALK], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE3_STRENGTH], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE3_CHARISMA], SW_HIDE);

					// Check if player has enough strength
					if (game.getPlayer().getStrength() >= ATTRIBUTES_NEEDED)
					{
						ShowWindow(hItems[SHANDAR_BUT_CHOICE3_FIGHT], SW_HIDE);
						// Show new windows
						ShowWindow(hItems[SHANDAR_STAT_CHOICE3_STRENGTH_WIN], SW_SHOW);

						// Update subscreen
						currentSubScreen = SHANDAR_STAT_CHOICE3_STRENGTH_WIN;
					}
					else
					{
						// Show new windows
						ShowWindow(hItems[SHANDAR_STAT_CHOICE3_STRENGTH_LOSE], SW_SHOW);

						// Update subscreen
						currentSubScreen = SHANDAR_STAT_CHOICE3_STRENGTH_LOSE;
					}

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
				// Charisma
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE3_CHARISMA])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE2_ANSWER_TALK], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE3_STRENGTH], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE3_CHARISMA], SW_HIDE);

					// Check if player has enough charisma
					if (game.getPlayer().getCharisma() >= ATTRIBUTES_NEEDED)
					{
						ShowWindow(hItems[SHANDAR_BUT_CHOICE3_FIGHT], SW_HIDE);
						// Show new windows
						ShowWindow(hItems[SHANDAR_STAT_CHOICE3_CHARISMA_WIN], SW_SHOW);

						// Update subscreen
						currentSubScreen = SHANDAR_STAT_CHOICE3_CHARISMA_WIN;
					}
					else
					{
						// Show new windows
						ShowWindow(hItems[SHANDAR_STAT_CHOICE3_CHARISMA_LOSE], SW_SHOW);

						// Update subscreen
						currentSubScreen = SHANDAR_STAT_CHOICE3_CHARISMA_LOSE;
					}

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
			}
			break;

			case SHANDAR_STAT_CHOICE3_STRENGTH_LOSE:
			{
				// Fight
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE3_FIGHT])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE2_ANSWER_TALK], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE3_FIGHT], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE3_STRENGTH], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE3_CHARISMA], SW_HIDE);

					// Display fighting
					game.setDisplayState(DisplayState::FIGHTING);
					game.setBackground(Game::Background::FIGHTING_QUEST_PERUGIA_SHANDAR_STREET);
					updateWindow(hWnd);

					// Fight
					FightStatus fightStatus = game.getFighting().fight(
						hWnd,
						game.getPlayer(),
						*generateNPC()
					);

					// Fight result
					switch (fightStatus)
					{
					case FightStatus::OPPONENT_LOST:
					{
						haveWon = true;
						// Update subscreen
						currentSubScreen = SHANDAR_STAT_ENTER_HOUSE;
					}
					break;

					case FightStatus::OPPONNENT_SURRENDERED:
					{
						haveWon = true;
						// Update subscreen
						currentSubScreen = SHANDAR_STAT_ENTER_HOUSE;
					}
					break;

					case FightStatus::PLAYER_SURRENDERED:
					{
						// Update subscreen
						currentSubScreen = SHANDAR_BUT_COMEBACK_NIGHT;

						// Update window
						game.updateBackground();
						updateWindow(hWnd);
					}
					break;

					case FightStatus::PLAYER_LOST:
					{
						// Update subscreen
						currentSubScreen = SHANDAR_BUT_COMEBACK_NIGHT;

						// Update window
						game.updateBackground();
						updateWindow(hWnd);
					}
					break;
					}
					return;
				}
				// Back down
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE2_LEAVE])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE3_STRENGTH_LOSE], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE3_FIGHT], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE2_LEAVE], SW_HIDE);

					// Show new windows
					ShowWindow(hItems[SHANDAR_BUT_COMEBACK_NIGHT], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_BUT_COMEBACK_NIGHT;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
			}
			break;

			case SHANDAR_STAT_CHOICE3_CHARISMA_LOSE:
			{
				// Fight
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE3_FIGHT])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE2_ANSWER_TALK], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE3_FIGHT], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE3_STRENGTH], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE3_CHARISMA], SW_HIDE);

					// Display fighting
					game.setDisplayState(DisplayState::FIGHTING);
					game.setBackground(Game::Background::FIGHTING_QUEST_PERUGIA_SHANDAR_STREET);
					updateWindow(hWnd);

					// Fight
					FightStatus fightStatus = game.getFighting().fight(
						hWnd,
						game.getPlayer(),
						*generateNPC()
					);

					// Fight result
					switch (fightStatus)
					{
					case FightStatus::OPPONENT_LOST:
					{
						haveWon = true;
						// Update subscreen
						currentSubScreen = SHANDAR_STAT_ENTER_HOUSE;
					}
					break;

					case FightStatus::OPPONNENT_SURRENDERED:
					{
						haveWon = true;
						// Update subscreen
						currentSubScreen = SHANDAR_STAT_ENTER_HOUSE;
					}
					break;

					case FightStatus::PLAYER_SURRENDERED:
					{
						// Update subscreen
						currentSubScreen = SHANDAR_BUT_COMEBACK_NIGHT;

						// Update window
						game.updateBackground();
						updateWindow(hWnd);
					}
					break;

					case FightStatus::PLAYER_LOST:
					{
						// Update subscreen
						currentSubScreen = SHANDAR_BUT_COMEBACK_NIGHT;

						// Update window
						game.updateBackground();
						updateWindow(hWnd);
					}
					break;
					}
					return;
				}
				// Back down
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE2_LEAVE])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE3_CHARISMA_LOSE], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE3_FIGHT], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE2_LEAVE], SW_HIDE);

					// Show new windows
					ShowWindow(hItems[SHANDAR_BUT_COMEBACK_NIGHT], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_BUT_COMEBACK_NIGHT;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
			}
			break;

			case SHANDAR_STAT_ENTER_HOUSE:
			{
				// Talk with Lucilius
				if ((HWND)lp == hItems[SHANDAR_BUT_ENTER_HOUSE])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_ENTER_HOUSE], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_ENTER_HOUSE], SW_HIDE);

					// Show new windows
					ShowWindow(hItems[SHANDAR_STAT_ENTER_HOUSE2], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE5_FIGHT], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE5_CHARISMA], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_STAT_ENTER_HOUSE2;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
			}
			break;

			case SHANDAR_STAT_ENTER_HOUSE2:
			{
				// Fight
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE5_FIGHT])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_ENTER_HOUSE2], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE5_FIGHT], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE5_CHARISMA], SW_HIDE);

					// Display fighting
					game.setDisplayState(DisplayState::FIGHTING);
					game.setBackground(Game::Background::FIGHTING_QUEST_PERUGIA_SHANDAR_HOUSE);
					updateWindow(hWnd);

					// Fight
					FightStatus fightStatus = game.getFighting().fight(
						hWnd,
						game.getPlayer(),
						*generateNPC()
					);

					// Fight result
					switch (fightStatus)
					{
					case FightStatus::OPPONENT_LOST:
					{
						// Update subscreen
						currentSubScreen = SHANDAR_STAT_CHOICE5_FIGHT_WIN;
					}
					break;

					case FightStatus::OPPONNENT_SURRENDERED:
					{
						// Update subscreen
						currentSubScreen = SHANDAR_STAT_CHOICE5_FIGHT_WIN;
					}
					break;

					case FightStatus::PLAYER_SURRENDERED:
					{
						// Update subscreen
						currentSubScreen = SHANDAR_STAT_CHOICE5_FIGHT_LOSE;
						isChoiceRequired = true;
					}
					break;

					case FightStatus::PLAYER_LOST:
					{
						// Update subscreen
						currentSubScreen = SHANDAR_STAT_CHOICE5_FIGHT_LOSE;
						isChoiceRequired = true;
					}
					break;
					}
					return;
				}
				// Charisma
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE5_CHARISMA])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_ENTER_HOUSE2], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE5_CHARISMA], SW_HIDE);

					// Check if player has enough charisma
					if (game.getPlayer().getCharisma() >= ATTRIBUTES_NEEDED)
					{
						ShowWindow(hItems[SHANDAR_BUT_CHOICE5_FIGHT], SW_HIDE);
						// Show new windows
						ShowWindow(hItems[SHANDAR_STAT_CHOICE5_CHARISMA_WIN], SW_SHOW);

						// Update subscreen
						currentSubScreen = SHANDAR_STAT_CHOICE5_CHARISMA_WIN;
					}
					else
					{
						// Show new windows
						ShowWindow(hItems[SHANDAR_STAT_CHOICE5_CHARISMA_LOSE], SW_SHOW);

						// Update subscreen
						currentSubScreen = SHANDAR_STAT_CHOICE5_CHARISMA_LOSE;
					}

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
			}
			break;

			case SHANDAR_STAT_CHOICE5_FIGHT_WIN:
			{
				// Spare
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE6_SPARE])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE5_FIGHT_WIN], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE6_SPARE], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE6_EXECUTE], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE6_EXTORT], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE6_EXILE], SW_HIDE);

					// Show new windows
					ShowWindow(hItems[SHANDAR_STAT_FOUND_GIRL], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_GREET_GIRL], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_STAT_FOUND_GIRL;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
				// Execute
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE6_EXECUTE])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE5_FIGHT_WIN], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE6_SPARE], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE6_EXECUTE], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE6_EXTORT], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE6_EXILE], SW_HIDE);

					// Show new windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE6_EXECUTED], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_STAT_CHOICE6_EXECUTED;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
				// Extort
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE6_EXTORT])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE5_FIGHT_WIN], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE6_SPARE], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE6_EXECUTE], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE6_EXTORT], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE6_EXILE], SW_HIDE);

					// Give player gold
					game.getPlayer().getInventory()->addItem(make_unique<Item>(Item(Item::ItemType::GOLD)), HIGH_VALUE_ITEM_LEVEL2);
					logStr += l.getMessage(Localized::YOU_HAVE_GAINED) + " " + to_string(HIGH_VALUE_ITEM_LEVEL2) + " " + l.getMessage(Localized::GOLD_GENITIVE) + "\r\n\r\n";
					playSound(SoundEnum::SOUND_GOLD);

					// Show new windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE6_EXTORT], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE6_EXTORT_EXECUTE], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE6_WHERE_GIRL], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_STAT_CHOICE6_EXTORT;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
				// Exile
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE6_EXILE])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE5_FIGHT_WIN], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE6_SPARE], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE6_EXECUTE], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE6_EXTORT], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE6_EXILE], SW_HIDE);

					// Show new windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE6_EXILE], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE6_WHERE_GIRL], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_STAT_CHOICE6_EXILE;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
			}
			break;

			case SHANDAR_STAT_CHOICE6_EXTORT:
			{
				// Execute Lucilius anyway
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE6_EXTORT_EXECUTE])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE6_EXTORT], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE6_EXTORT_EXECUTE], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE6_WHERE_GIRL], SW_HIDE);

					// Show new windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE6_EXECUTED], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_STAT_CHOICE6_EXECUTED;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
				// Ask about girl
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE6_WHERE_GIRL])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE6_EXTORT], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE6_EXTORT_EXECUTE], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE6_WHERE_GIRL], SW_HIDE);

					// Show new windows
					ShowWindow(hItems[SHANDAR_STAT_FOUND_GIRL], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_GREET_GIRL], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_STAT_FOUND_GIRL;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
			}
			break;

			case SHANDAR_STAT_CHOICE6_EXILE:
			{
				// Ask about girl
				if ((HWND)lp == hItems[SHANDAR_BUT_CHOICE6_WHERE_GIRL])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE6_EXILE], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_CHOICE6_WHERE_GIRL], SW_HIDE);

					// Show new windows
					ShowWindow(hItems[SHANDAR_STAT_FOUND_GIRL], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_GREET_GIRL], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_STAT_FOUND_GIRL;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
			}
			break;

			case SHANDAR_STAT_FOUND_GIRL:
			{
				// Talk with Abhilasha
				if ((HWND)lp == hItems[SHANDAR_BUT_GREET_GIRL])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_FOUND_GIRL], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_GREET_GIRL], SW_HIDE);

					// Show new windows
					ShowWindow(hItems[SHANDAR_STAT_GREET_GIRL], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_LETS_LEAVE], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_STAT_GREET_GIRL;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
			}
			break;

			case SHANDAR_STAT_GREET_GIRL:
			{
				// Return to Shandar
				if ((HWND)lp == hItems[SHANDAR_BUT_LETS_LEAVE])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_GREET_GIRL], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_LETS_LEAVE], SW_HIDE);

					// Check how player dealt with Lucilius
					if (haveFought)
					{
						// Show new windows
						ShowWindow(hItems[SHANDAR_STAT_LEAVE_LOUD], SW_SHOW);

						// Update subscreen
						currentSubScreen = SHANDAR_STAT_LEAVE_LOUD;
					}
					else
					{
						// Show new windows
						ShowWindow(hItems[SHANDAR_STAT_LEAVE_QUIET], SW_SHOW);

						// Update subscreen
						currentSubScreen = SHANDAR_STAT_LEAVE_QUIET;
					}

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
			}
			break;

			case SHANDAR_STAT_RETURNED_DAUGHTER:
			{
				// Ask for reward
				if ((HWND)lp == hItems[SHANDAR_BUT_ASK_REWARD])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_RETURNED_DAUGHTER], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_ASK_REWARD], SW_HIDE);

					// Give player gold
					game.getPlayer().getInventory()->addItem(make_unique<Item>(Item(Item::ItemType::GOLD)), HIGH_VALUE_ITEM_LEVEL1);
					logStr += l.getMessage(Localized::YOU_HAVE_GAINED) + " " + to_string(HIGH_VALUE_ITEM_LEVEL1) + " " + l.getMessage(Localized::GOLD_GENITIVE) + "\r\n\r\n";
					playSound(SoundEnum::SOUND_GOLD);

					// Show new windows
					ShowWindow(hItems[SHANDAR_STAT_REWARD], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_STAT_REWARD;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
				// Say goodbye
				if ((HWND)lp == hItems[SHANDAR_BUT_GOODBYE])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_RETURNED_DAUGHTER], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_ASK_REWARD], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_GOODBYE], SW_HIDE);

					// Show new windows
					ShowWindow(hItems[SHANDAR_STAT_GOODBYE], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_NAME], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_STAT_GOODBYE;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
			}
			break;

			case SHANDAR_STAT_REWARD:
			{
				// Say goodbye
				if ((HWND)lp == hItems[SHANDAR_BUT_GOODBYE])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_RETURNED_DAUGHTER], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_GOODBYE], SW_HIDE);

					// Show new windows
					ShowWindow(hItems[SHANDAR_STAT_GOODBYE], SW_SHOW);
					ShowWindow(hItems[SHANDAR_BUT_NAME], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_STAT_GOODBYE;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
			}
			break;

			case SHANDAR_STAT_GOODBYE:
			{
				// Tell your name
				if ((HWND)lp == hItems[SHANDAR_BUT_NAME])
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_GOODBYE], SW_HIDE);
					ShowWindow(hItems[SHANDAR_BUT_NAME], SW_HIDE);

					// Show new windows
					ShowWindow(hItems[SHANDAR_STAT_THANKS], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_STAT_THANKS;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
			}
			break;
			}
		}
		break;
		}
	}
	break;
	case WM_LBUTTONDOWN:
	{
		// Delay so player won't accidentaly skip a scene
		Sleep(STORY_SLEEP_TIME);
		switch (currentScreen)
		{
		case WELCOME_SCREEN:
		{
			// Destroying all windows
			for (HWND hItem : hItems)
				if (hItem != NULL)
					DestroyWindow(hItem);
			hItems.clear();

			game.setDisplayState(DisplayState::MENU);
			game.updateBackground();

			updateWindow(hWnd);
		}
		break;

		case START_GAME_SCREEN:
		{
			// Destroying all windows
			for (HWND hItem : hItems)
				if (hItem != NULL)
					DestroyWindow(hItem);
			hItems.clear();

			game.setDisplayState(DisplayState::MENU);
			game.updateBackground();

			updateWindow(hWnd);
		}
		break;

		case FIRST_VICTORY:
		{
			// Destroying all windows
			for (HWND hItem : hItems)
				if (hItem != NULL)
					DestroyWindow(hItem);
			hItems.clear();

			game.setDisplayState(DisplayState::MENU);
			game.updateBackground();

			updateWindow(hWnd);
		}
		break;

		case STOLEN_MONEY:
		{
			if (!isChoiceRequired)
			{
				// Destroying all windows
				for (HWND hItem : hItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hItems.clear();

				game.setDisplayState(DisplayState::MENU);
				game.updateBackground();
			}
			else if (currentSubScreen < STOLEN_MONEY_SCREEN_STAT_3)
			{
				currentSubScreen++;
				for (HWND item : hItems)
					ShowWindow(item, SW_HIDE);
				ShowWindow(hItems[currentSubScreen], SW_SHOW);
			}
			else
			{
				ShowWindow(hItems[STOLEN_MONEY_SCREEN_BUT_ESCAPE], SW_SHOW);
				ShowWindow(hItems[STOLEN_MONEY_SCREEN_BUT_STAY], SW_SHOW);
			}
			updateWindow(hWnd);
		}
		break;

		case CENTURION_OFFER:
		{
			if (!isChoiceRequired)
			{
				// Destroying all windows
				for (HWND hItem : hItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hItems.clear();

				game.setDisplayState(DisplayState::MENU);
				game.updateBackground();
			}
			else if (currentSubScreen < CENTURION_OFFER_SCREEN_STAT_2)
			{
				currentSubScreen++;
				for (HWND item : hItems)
					ShowWindow(item, SW_HIDE);
				ShowWindow(hItems[currentSubScreen], SW_SHOW);
			}
			else
			{
				ShowWindow(hItems[CENTURION_OFFER_SCREEN_BUT_ACCEPT], SW_SHOW);
				ShowWindow(hItems[CENTURION_OFFER_SCREEN_BUT_DECLINE], SW_SHOW);
			}
			updateWindow(hWnd);
		}
		break;

		case GAINED_FREEDOM:
		{
			if (!isChoiceRequired)
			{
				// Destroying all windows
				for (HWND hItem : hItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hItems.clear();

				game.setDisplayState(DisplayState::MENU);
				game.updateBackground();
			}
			else if (currentSubScreen < GAINED_FREEDOM_SCREEN_STAT_2)
			{
				currentSubScreen++;
				for (HWND item : hItems)
					ShowWindow(item, SW_HIDE);
				ShowWindow(hItems[currentSubScreen], SW_SHOW);
			}
			else
			{
				ShowWindow(hItems[GAINED_FREEDOM_SCREEN_BUT_CHOOSE_REBELS], SW_SHOW);
				ShowWindow(hItems[GAINED_FREEDOM_SCREEN_BUT_CHOOSE_LEGION], SW_SHOW);
			}
			updateWindow(hWnd);
		}
		break;

		case QUEST_PERUGIA_SHANDAR:
		{
			switch (currentSubScreen)
			{
			// Guards at the gate encounter
			{
			case SHANDAR_STAT_CHOICE3_STRENGTH_WIN:
			{
				// Hide previous windows
				ShowWindow(hItems[SHANDAR_STAT_CHOICE3_STRENGTH_WIN], SW_HIDE);

				// Show new windows
				ShowWindow(hItems[SHANDAR_STAT_ENTER_HOUSE], SW_SHOW);
				ShowWindow(hItems[SHANDAR_BUT_ENTER_HOUSE], SW_SHOW);

				// Update subscreen
				currentSubScreen = SHANDAR_STAT_ENTER_HOUSE;

				// Update window
				game.updateBackground();
				updateWindow(hWnd);
			}
			break;

			case SHANDAR_STAT_CHOICE3_CHARISMA_WIN:
			{
				// Hide previous windows
				ShowWindow(hItems[SHANDAR_STAT_CHOICE3_CHARISMA_WIN], SW_HIDE);

				// Show new windows
				ShowWindow(hItems[SHANDAR_STAT_ENTER_HOUSE], SW_SHOW);
				ShowWindow(hItems[SHANDAR_BUT_ENTER_HOUSE], SW_SHOW);

				// Update subscreen
				currentSubScreen = SHANDAR_STAT_ENTER_HOUSE;

				// Update window
				game.updateBackground();
				updateWindow(hWnd);
			}
			break;
			}

			// Night
			{
			case SHANDAR_BUT_COMEBACK_NIGHT:
			{
				// Show new windows
				ShowWindow(hItems[SHANDAR_BUT_COMEBACK_NIGHT], SW_SHOW);

				// Update window
				game.updateBackground();
				updateWindow(hWnd);
			}
			break;

			case SHANDAR_STAT_CHOICE4_NIGHT_DEXTERITY_WIN:
			{
				// Hide previous windows
				ShowWindow(hItems[SHANDAR_STAT_CHOICE4_NIGHT_DEXTERITY_WIN], SW_HIDE);

				// Show new windows
				ShowWindow(hItems[SHANDAR_STAT_FOUND_GIRL], SW_SHOW);
				ShowWindow(hItems[SHANDAR_BUT_GREET_GIRL], SW_SHOW);

				// Update subscreen
				currentSubScreen = SHANDAR_STAT_FOUND_GIRL;

				// Update window
				game.updateBackground();
				updateWindow(hWnd);
			}
			break;

			case SHANDAR_STAT_CHOICE4_NIGHT_DEXTERITY_LOSE:
			{
				// Hide previous windows
				ShowWindow(hItems[SHANDAR_STAT_CHOICE4_NIGHT_DEXTERITY_LOSE], SW_HIDE);

				// Show new windows
				ShowWindow(hItems[SHANDAR_STAT_ENTER_HOUSE], SW_SHOW);
				ShowWindow(hItems[SHANDAR_BUT_ENTER_HOUSE], SW_SHOW);

				// Update subscreen
				currentSubScreen = SHANDAR_STAT_ENTER_HOUSE;

				// Update window
				game.updateBackground();
				updateWindow(hWnd);
			}
			break;
			}

			// Lucilius encounter
			{
			case SHANDAR_STAT_ENTER_HOUSE:
			{
				// Show new windows
				ShowWindow(hItems[SHANDAR_STAT_ENTER_HOUSE], SW_SHOW);
				ShowWindow(hItems[SHANDAR_BUT_ENTER_HOUSE], SW_SHOW);
				
				// Update window
				game.updateBackground();
				updateWindow(hWnd);
			}
			break;

			case SHANDAR_STAT_CHOICE5_CHARISMA_WIN:
			{
				// Hide previous windows
				ShowWindow(hItems[SHANDAR_STAT_CHOICE5_CHARISMA_WIN], SW_HIDE);

				// Show new windows
				ShowWindow(hItems[SHANDAR_STAT_FOUND_GIRL], SW_SHOW);
				ShowWindow(hItems[SHANDAR_BUT_GREET_GIRL], SW_SHOW);

				// Update subscreen
				currentSubScreen = SHANDAR_STAT_FOUND_GIRL;

				// Update window
				game.updateBackground();
				updateWindow(hWnd);
			}
			break;

			case SHANDAR_STAT_CHOICE5_CHARISMA_LOSE:
			{
				// Hide previous windows
				ShowWindow(hItems[SHANDAR_BUT_CHOICE5_CHARISMA], SW_HIDE);

				// Display fighting
				game.setDisplayState(DisplayState::FIGHTING);
				game.setBackground(Game::Background::FIGHTING_QUEST_PERUGIA_SHANDAR_HOUSE);
				updateWindow(hWnd);

				// Fight
				FightStatus fightStatus = game.getFighting().fight(
					hWnd,
					game.getPlayer(),
					*generateNPC()
				);

				// Fight result
				switch (fightStatus)
				{
				case FightStatus::OPPONENT_LOST:
				{
					// Update subscreen
					currentSubScreen = SHANDAR_STAT_CHOICE5_FIGHT_WIN;
				}
				break;

				case FightStatus::OPPONNENT_SURRENDERED:
				{
					// Update subscreen
					currentSubScreen = SHANDAR_STAT_CHOICE5_FIGHT_WIN;
				}
				break;

				case FightStatus::PLAYER_SURRENDERED:
				{
					// Update subscreen
					currentSubScreen = SHANDAR_STAT_CHOICE5_FIGHT_LOSE;
					isChoiceRequired = true;
				}
				break;

				case FightStatus::PLAYER_LOST:
				{
					// Update subscreen
					currentSubScreen = SHANDAR_STAT_CHOICE5_FIGHT_LOSE;
					isChoiceRequired = true;
				}
				break;
				}
				return;
			}
			break;

			case SHANDAR_STAT_CHOICE6_EXECUTED:
			{
				// Hide previous windows
				ShowWindow(hItems[SHANDAR_STAT_CHOICE6_EXECUTED], SW_HIDE);

				// Show new windows
				ShowWindow(hItems[SHANDAR_STAT_FOUND_GIRL], SW_SHOW);
				ShowWindow(hItems[SHANDAR_BUT_GREET_GIRL], SW_SHOW);

				// Update subscreen
				currentSubScreen = SHANDAR_STAT_FOUND_GIRL;

				// Update window
				game.updateBackground();
				updateWindow(hWnd);
			}
			break;

			case SHANDAR_STAT_CHOICE5_FIGHT_WIN:
			{
				// Show new windows
				ShowWindow(hItems[SHANDAR_STAT_CHOICE5_FIGHT_WIN], SW_SHOW);
				ShowWindow(hItems[SHANDAR_BUT_CHOICE6_SPARE], SW_SHOW);
				ShowWindow(hItems[SHANDAR_BUT_CHOICE6_EXECUTE], SW_SHOW);
				ShowWindow(hItems[SHANDAR_BUT_CHOICE6_EXTORT], SW_SHOW);
				ShowWindow(hItems[SHANDAR_BUT_CHOICE6_EXILE], SW_SHOW);

				// Update window
				game.updateBackground();
				updateWindow(hWnd);
			}
			break;

			case SHANDAR_STAT_CHOICE5_FIGHT_LOSE:
			{
				if (!isChoiceRequired)
				{
					// Hide previous windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE5_FIGHT_LOSE], SW_HIDE);

					// Show new windows
					ShowWindow(hItems[SHANDAR_STAT_LOST_QUEST], SW_SHOW);

					// Update subscreen
					currentSubScreen = SHANDAR_STAT_LOST_QUEST;

					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
				else
				{
					// Show new windows
					ShowWindow(hItems[SHANDAR_STAT_CHOICE5_FIGHT_LOSE], SW_SHOW);
					isChoiceRequired = false;
					// Update window
					game.updateBackground();
					updateWindow(hWnd);
				}
			}
			break;

			case SHANDAR_STAT_LOST_QUEST:
			{
				// Destroying all windows
				for (HWND hItem : hItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hItems.clear();

				game.setDisplayState(DisplayState::MENU);
				game.updateBackground();

				updateWindow(hWnd);
			}
			break;
			}

			// Return to Shandar
			{
			case SHANDAR_STAT_LEAVE_QUIET:
			{
				// Hide previous windows
				ShowWindow(hItems[SHANDAR_STAT_LEAVE_QUIET], SW_HIDE);

				// Show new windows
				ShowWindow(hItems[SHANDAR_STAT_RETURNED_DAUGHTER], SW_SHOW);
				ShowWindow(hItems[SHANDAR_BUT_ASK_REWARD], SW_SHOW);
				ShowWindow(hItems[SHANDAR_BUT_GOODBYE], SW_SHOW);

				// Update subscreen
				currentSubScreen = SHANDAR_STAT_RETURNED_DAUGHTER;

				// Update window
				game.updateBackground();
				updateWindow(hWnd);
			}
			break;

			case SHANDAR_STAT_LEAVE_LOUD:
			{
				// Hide previous windows
				ShowWindow(hItems[SHANDAR_STAT_LEAVE_LOUD], SW_HIDE);

				// Show new windows
				ShowWindow(hItems[SHANDAR_STAT_RETURNED_DAUGHTER], SW_SHOW);
				ShowWindow(hItems[SHANDAR_BUT_ASK_REWARD], SW_SHOW);
				ShowWindow(hItems[SHANDAR_BUT_GOODBYE], SW_SHOW);

				// Update subscreen
				currentSubScreen = SHANDAR_STAT_RETURNED_DAUGHTER;

				// Update window
				game.updateBackground();
				updateWindow(hWnd);
			}
			break;
			}

			// End
			{
			case SHANDAR_STAT_THANKS:
			{
				// Hide previous windows
				ShowWindow(hItems[SHANDAR_STAT_THANKS], SW_HIDE);

				// Show new windows
				ShowWindow(hItems[SHANDAR_STAT_END], SW_SHOW);

				// Update subscreen
				currentSubScreen = SHANDAR_STAT_END;

				// Update window
				game.updateBackground();
				updateWindow(hWnd);
			}
			break;

			case SHANDAR_STAT_END:
			{
				// Destroying all windows
				for (HWND hItem : hItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hItems.clear();

				// Give player experience
				Player& rPlayer = game.getPlayer();
				int playerLevel = rPlayer.getLevel();
				int experience = EXPERIENCE_PER_LEVEL * 2;
				rPlayer.gainExperience(experience);

				logStr += l.getMessage(Localized::YOU_HAVE_GAINED) + " " + to_string(experience) + " " + l.getMessage(Localized::EXPERIENCE_GENITIVE) +
					" (" + to_string(rPlayer.getExperience()) + " / " + to_string(rPlayer.calculateExperienceForOneLevel(rPlayer.getLevel() + 1)) + ")\r\n\r\n";
				// Level up
				if (rPlayer.getLevel() > playerLevel)
				{
					Sleep(600); // Sleep so sound would have time to play
					logStr += l.getMessage(Localized::LEVELED_UP) + " " + to_string(rPlayer.getLevel()) +
						" (" + to_string(rPlayer.getUnnassignedAttributes()) + " " + l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES_GENITIVE) + ")\r\n\r\n";
					playSound(SoundEnum::SOUND_LEVEL_UP);
				}

				// Return to city
				game.setBackground(Game::Background::CITY_MENU);
				game.setDisplayState(DisplayState::MENU);
				game.updateBackground();
				updateWindow(hWnd);
				// If autosave is enabled - save game
				game.saveToFile(game.getAutoSaveStatus());
			}
			break;
			}
			}
		}
		break;
		}
	}
	break;
	}
}

bool StoryScreen::stylizeWindow(HWND hWnd, UINT m, WPARAM wp, LPARAM lp, LRESULT& result)
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

		switch (currentScreen)
		{
		case WELCOME_SCREEN:
		{
			if (item->hwndItem == hItems[WELCOME_SCREEN_STAT_DATE])
			{
				SelectObject(hdc, game.getFont(Game::FontSize::LARGE));
				SetBkMode(hdc, TRANSPARENT);

				FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_DARK_BLUE)); // Fill background
				DrawTextA(item->hDC, buf.c_str(), len, &item->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER); // Display text
				DrawEdge(hdc, &item->rcItem, EDGE_SUNKEN, BF_RECT); // Draw edge
				
				return true;
			}
			if (item->hwndItem == hItems[WELCOME_SCREEN_STAT_TEXT])
			{
				SelectObject(hdc, game.getFont(Game::FontSize::LARGE));
				SetBkMode(hdc, TRANSPARENT);

				FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_DARK_BLUE)); // Fill background
				// Text padding
				RECT rect = item->rcItem;
				int padding = 10;
				rect.left += padding;
				rect.top += padding;
				rect.right -= padding;
				rect.bottom -= padding;

				DrawTextA(item->hDC, buf.c_str(), len, &rect, DT_VCENTER | DT_LEFT | DT_WORDBREAK); // Display text
				DrawEdge(hdc, &item->rcItem, EDGE_SUNKEN, BF_RECT); // Draw edge
				return true;
			}
		}
		break;

		case START_GAME_SCREEN:
		{
			if (item->hwndItem == hItems[0])
			{
				SelectObject(hdc, game.getFont(Game::FontSize::LARGE));
				SetBkMode(hdc, TRANSPARENT);

				FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_DARK_BLUE)); // Fill background
				// Text padding
				RECT rect = item->rcItem;
				int padding = 10;
				rect.left += padding;
				rect.top += padding;
				rect.right -= padding;
				rect.bottom -= padding;

				DrawTextA(item->hDC, buf.c_str(), len, &rect, DT_VCENTER | DT_LEFT | DT_WORDBREAK); // Display text
				DrawEdge(hdc, &item->rcItem, EDGE_SUNKEN, BF_RECT); // Draw edge
				return true;
			}
		}
		break;

		case FIRST_VICTORY:
		{
			if (item->hwndItem == hItems[0])
			{
				SelectObject(hdc, game.getFont(Game::FontSize::LARGE));
				SetBkMode(hdc, TRANSPARENT);

				FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_DARK_BLUE)); // Fill background
				// Text padding
				RECT rect = item->rcItem;
				int padding = 10;
				rect.left += padding;
				rect.top += padding;
				rect.right -= padding;
				rect.bottom -= padding;

				DrawTextA(item->hDC, buf.c_str(), len, &rect, DT_VCENTER | DT_LEFT | DT_WORDBREAK); // Display text
				DrawEdge(hdc, &item->rcItem, EDGE_SUNKEN, BF_RECT); // Draw edge
				return true;
			}
		}
		break;

		case STOLEN_MONEY:
		{
			for (int i = STOLEN_MONEY_SCREEN_STAT_1; i <= STOLEN_MONEY_SCREEN_STAT_STAY; i++)
			{
				if (item->hwndItem == hItems[i])
				{
					SelectObject(hdc, game.getFont(Game::FontSize::LARGE));
					SetBkMode(hdc, TRANSPARENT);

					FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_DARK_BLUE)); // Fill background
					// Text padding
					RECT rect = item->rcItem;
					int padding = 10;
					rect.left += padding;
					rect.top += padding;
					rect.right -= padding;
					rect.bottom -= padding;

					DrawTextA(item->hDC, buf.c_str(), len, &rect, DT_VCENTER | DT_LEFT | DT_WORDBREAK); // Display text
					DrawEdge(hdc, &item->rcItem, EDGE_SUNKEN, BF_RECT); // Draw edge
					return true;
				}
			}
		}
		break;

		case CENTURION_OFFER:
		{
			for (int i = CENTURION_OFFER_SCREEN_STAT_1; i <= CENTURION_OFFER_SCREEN_STAT_DECLINE; i++)
			{
				if (item->hwndItem == hItems[i])
				{
					SelectObject(hdc, game.getFont(Game::FontSize::LARGE));
					SetBkMode(hdc, TRANSPARENT);

					FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_DARK_BLUE)); // Fill background
					// Text padding
					RECT rect = item->rcItem;
					int padding = 10;
					rect.left += padding;
					rect.top += padding;
					rect.right -= padding;
					rect.bottom -= padding;

					DrawTextA(item->hDC, buf.c_str(), len, &rect, DT_VCENTER | DT_LEFT | DT_WORDBREAK); // Display text
					DrawEdge(hdc, &item->rcItem, EDGE_SUNKEN, BF_RECT); // Draw edge
					return true;
				}
			}
		}
		break;

		case GAINED_FREEDOM:
		{
			for (int i = GAINED_FREEDOM_SCREEN_STAT_1; i <= GAINED_FREEDOM_SCREEN_STAT_CHOSE_LEGION; i++)
			{
				if (item->hwndItem == hItems[i])
				{
					SelectObject(hdc, game.getFont(Game::FontSize::LARGE));
					SetBkMode(hdc, TRANSPARENT);

					FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_DARK_BLUE)); // Fill background
					// Text padding
					RECT rect = item->rcItem;
					int padding = 10;
					rect.left += padding;
					rect.top += padding;
					rect.right -= padding;
					rect.bottom -= padding;

					DrawTextA(item->hDC, buf.c_str(), len, &rect, DT_VCENTER | DT_LEFT | DT_WORDBREAK); // Display text
					DrawEdge(hdc, &item->rcItem, EDGE_SUNKEN, BF_RECT); // Draw edge
					return true;
				}
			}
		}
		break;

		case QUEST_PERUGIA_SHANDAR:
		{
			SelectObject(hdc, game.getFont(Game::FontSize::LARGE));
			SetBkMode(hdc, TRANSPARENT);

			// Fill background
			if (item->hwndItem == hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHERE])
			{
				FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_ORANGE));
			}
			else if (item->CtlType == ODT_BUTTON)
			{
				// Fill background
				if (item->itemState & ODS_SELECTED)
					FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_ROMAN_RED_PUSHED));
				else
					FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_ROMAN_RED));
			}
			else
				FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_DARK_BLUE));

			// Text padding
			RECT rect = item->rcItem;
			int padding = 10;
			rect.left += padding;
			rect.top += padding;
			rect.right -= padding;
			rect.bottom -= padding;

			DrawTextA(item->hDC, buf.c_str(), len, &rect, DT_VCENTER | DT_LEFT | DT_WORDBREAK); // Display text
			DrawEdge(hdc, &item->rcItem, EDGE_SUNKEN, BF_RECT); // Draw edge
			return true;
		}
		break;
		}
	}
	break;
	}
	return false;
}

void StoryScreen::displayScreen(HWND hWnd, Screen screen)
{
	game.setDisplayState(DisplayState::STORY_SCREEN);

	// Destroying all previous windows
	for (HWND hItem : hItems)
		if (hItem != NULL)
			DestroyWindow(hItem);
	hItems.clear();

	isChoiceRequired = false;
	currentSubScreen = -1;

	// Create new windows
	switch (screen)
	{
	case WELCOME_SCREEN:
	{
		hItems.resize(WELCOME_SCREEN_ITEM_NUMBER);

		hItems[WELCOME_SCREEN_STAT_DATE] = CreateWindow("STATIC", l.getMessage(Localized::STORY_WELCOME_SCREEN_DATE).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[WELCOME_SCREEN_STAT_TEXT] = CreateWindow("STATIC", l.getMessage(Localized::STORY_WELCOME_SCREEN_TEXT).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
	}
	break;

	case START_GAME_SCREEN:
	{
		hItems.resize(1);

		hItems[0] = CreateWindow("STATIC", l.getMessage(Localized::STORY_START_GAME_TEXT).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
	}
	break;

	case FIRST_VICTORY:
	{
		hItems.resize(1);

		hItems[0] = CreateWindow("STATIC", l.getMessage(Localized::STORY_FIRST_VICTORY_TEXT).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
	}
	break;

	case STOLEN_MONEY:
	{
		hItems.resize(STOLEN_MONEY_SCREEN_ITEM_NUMBER);

		hItems[STOLEN_MONEY_SCREEN_STAT_1] = CreateWindow("STATIC", l.getMessage(Localized::STORY_STOLEN_MONEY_TEXT1).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[STOLEN_MONEY_SCREEN_STAT_2] = CreateWindow("STATIC", l.getMessage(Localized::STORY_STOLEN_MONEY_TEXT2).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[STOLEN_MONEY_SCREEN_STAT_3] = CreateWindow("STATIC", l.getMessage(Localized::STORY_STOLEN_MONEY_TEXT3).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[STOLEN_MONEY_SCREEN_STAT_ESCAPE] = CreateWindow("STATIC", l.getMessage(Localized::STORY_STOLEN_MONEY_ESCAPE).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[STOLEN_MONEY_SCREEN_STAT_STAY] = CreateWindow("STATIC", l.getMessage(Localized::STORY_STOLEN_MONEY_STAY).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[STOLEN_MONEY_SCREEN_BUT_ESCAPE] = CreateWindow("BUTTON", l.getMessage(Localized::ESCAPE).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[STOLEN_MONEY_SCREEN_BUT_STAY] = CreateWindow("BUTTON", l.getMessage(Localized::STAY).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);

		isChoiceRequired = true;
		currentSubScreen = STOLEN_MONEY_SCREEN_STAT_1;
	}
	break;

	case CENTURION_OFFER:
	{
		hItems.resize(CENTURION_OFFER_SCREEN_ITEM_NUMBER);

		hItems[CENTURION_OFFER_SCREEN_STAT_1] = CreateWindow("STATIC", l.getMessage(Localized::STORY_CENTURION_OFFER_TEXT1).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[CENTURION_OFFER_SCREEN_STAT_2] = CreateWindow("STATIC", l.getMessage(Localized::STORY_CENTURION_OFFER_TEXT2).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[CENTURION_OFFER_SCREEN_STAT_ACCEPT] = CreateWindow("STATIC", l.getMessage(Localized::STORY_CENTURION_OFFER_ACCEPT).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[CENTURION_OFFER_SCREEN_STAT_DECLINE] = CreateWindow("STATIC", l.getMessage(Localized::STORY_CENTURION_OFFER_DECLINE).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[CENTURION_OFFER_SCREEN_BUT_ACCEPT] = CreateWindow("BUTTON", l.getMessage(Localized::ACCEPT_OFFER).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[CENTURION_OFFER_SCREEN_BUT_DECLINE] = CreateWindow("BUTTON", l.getMessage(Localized::DECLINE_OFFER).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);

		isChoiceRequired = true;
		currentSubScreen = CENTURION_OFFER_SCREEN_STAT_1;
	}
	break;

	case GAINED_FREEDOM:
	{
		hItems.resize(CENTURION_OFFER_SCREEN_ITEM_NUMBER);

		hItems[GAINED_FREEDOM_SCREEN_STAT_1] = CreateWindow("STATIC", l.getMessage(Localized::STORY_GAINED_FREEDOM_TEXT1).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[GAINED_FREEDOM_SCREEN_STAT_2] = CreateWindow("STATIC", l.getMessage(Localized::STORY_GAINED_FREEDOM_TEXT2).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[GAINED_FREEDOM_SCREEN_STAT_CHOSE_REBELS] = CreateWindow("STATIC", l.getMessage(Localized::STORY_GAINED_FREEDOM_CHOSE_REBELS).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[GAINED_FREEDOM_SCREEN_STAT_CHOSE_LEGION] = CreateWindow("STATIC", l.getMessage(Localized::STORY_GAINED_FREEDOM_CHOSE_LEGION).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[GAINED_FREEDOM_SCREEN_BUT_CHOOSE_REBELS] = CreateWindow("BUTTON", l.getMessage(Localized::CHOOSE_REBELS).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[GAINED_FREEDOM_SCREEN_BUT_CHOOSE_LEGION] = CreateWindow("BUTTON", l.getMessage(Localized::CHOOSE_LEGION).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);

		isChoiceRequired = true;
		currentSubScreen = GAINED_FREEDOM_SCREEN_STAT_1;
	}
	break;

	case QUEST_PERUGIA_SHANDAR:
	{
		hItems.resize(SHANDAR_ITEM_NUMBER);

		// Intro
		hItems[SHANDAR_STAT_INTRO] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_INTRO).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_BUT_INTRO] = CreateWindow("BUTTON", l.getMessage(Localized::SHANDAR_BUT_INTRO).c_str(), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_INTRO2] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_INTRO2).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

		// Choice 1 - talk with Shandar
		hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHO] = CreateWindow("BUTTON", l.getMessage(Localized::SHANDAR_BUT_CHOICE1_QUESTION_WHO).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHY] = CreateWindow("BUTTON", l.getMessage(Localized::SHANDAR_BUT_CHOICE1_QUESTION_WHY).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_BUT_CHOICE1_QUESTION_WHERE] = CreateWindow("BUTTON", l.getMessage(Localized::SHANDAR_BUT_CHOICE1_QUESTION_WHERE).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_BUT_CHOICE1_LEAVE] = CreateWindow("BUTTON", l.getMessage(Localized::SHANDAR_BUT_CHOICE1_LEAVE).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_CHOICE1_ANSWER_WHO] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_CHOICE1_ANSWER_WHO).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_CHOICE1_ANSWER_WHY] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_CHOICE1_ANSWER_WHY).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_CHOICE1_ANSWER_WHERE] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_CHOICE1_ANSWER_WHERE).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

		// Choice 2 - guards at entrance
		hItems[SHANDAR_BUT_CHOICE2_TALK] = CreateWindow("BUTTON", l.getMessage(Localized::SHANDAR_BUT_CHOICE2_TALK).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_BUT_CHOICE2_LEAVE] = CreateWindow("BUTTON", l.getMessage(Localized::SHANDAR_BUT_CHOICE2_LEAVE).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_CHOICE2_ANSWER_TALK] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_CHOICE2_ANSWER_TALK).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

		// Choice 3 - guards at entrance
		hItems[SHANDAR_BUT_CHOICE3_FIGHT] = CreateWindow("BUTTON", l.getMessage(Localized::SHANDAR_BUT_CHOICE3_FIGHT).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_BUT_CHOICE3_STRENGTH] = CreateWindow("BUTTON", l.getMessage(Localized::SHANDAR_BUT_CHOICE3_STRENGTH).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_BUT_CHOICE3_CHARISMA] = CreateWindow("BUTTON", l.getMessage(Localized::SHANDAR_BUT_CHOICE3_CHARISMA).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_CHOICE3_STRENGTH_WIN] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_CHOICE3_STRENGTH_WIN).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_CHOICE3_STRENGTH_LOSE] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_CHOICE3_STRENGTH_LOSE).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_CHOICE3_CHARISMA_WIN] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_CHOICE3_CHARISMA_WIN).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_CHOICE3_CHARISMA_LOSE] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_CHOICE3_CHARISMA_LOSE).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

		// Comeback at night
		hItems[SHANDAR_BUT_COMEBACK_NIGHT] = CreateWindow("BUTTON", l.getMessage(Localized::SHANDAR_BUT_COMEBACK_NIGHT).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_COMEBACK_NIGHT] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_COMEBACK_NIGHT).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

		// Choice 4 - enter house at night
		hItems[SHANDAR_BUT_CHOICE4_NIGHT_ENTER] = CreateWindow("BUTTON", l.getMessage(Localized::SHANDAR_BUT_CHOICE4_NIGHT_ENTER).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_BUT_CHOICE4_NIGHT_DEXTERITY] = CreateWindow("BUTTON", l.getMessage(Localized::SHANDAR_BUT_CHOICE4_NIGHT_DEXTERITY).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_CHOICE4_NIGHT_DEXTERITY_WIN] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_CHOICE4_NIGHT_DEXTERITY_WIN).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_CHOICE4_NIGHT_DEXTERITY_LOSE] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_CHOICE4_NIGHT_DEXTERITY_LOSE).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

		// Enter house
		hItems[SHANDAR_STAT_ENTER_HOUSE] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_ENTER_HOUSE).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_BUT_ENTER_HOUSE] = CreateWindow("BUTTON", l.getMessage(Localized::SHANDAR_BUT_ENTER_HOUSE).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_ENTER_HOUSE2] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_ENTER_HOUSE2).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

		// Choice 5 - speak with Lucilius
		hItems[SHANDAR_BUT_CHOICE5_FIGHT] = CreateWindow("BUTTON", l.getMessage(Localized::SHANDAR_BUT_CHOICE5_FIGHT).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_BUT_CHOICE5_CHARISMA] = CreateWindow("BUTTON", l.getMessage(Localized::SHANDAR_BUT_CHOICE5_CHARISMA).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_CHOICE5_FIGHT_WIN] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_CHOICE5_FIGHT_WIN).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_CHOICE5_FIGHT_LOSE] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_CHOICE5_FIGHT_LOSE).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_LOST_QUEST] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_LOST_QUEST).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_CHOICE5_CHARISMA_WIN] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_CHOICE5_CHARISMA_WIN).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_CHOICE5_CHARISMA_LOSE] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_CHOICE5_CHARISMA_LOSE).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

		// Choice 6 - kill or spare Lucilius
		hItems[SHANDAR_BUT_CHOICE6_SPARE] = CreateWindow("BUTTON", l.getMessage(Localized::SHANDAR_BUT_CHOICE6_SPARE).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_BUT_CHOICE6_EXECUTE] = CreateWindow("BUTTON", l.getMessage(Localized::SHANDAR_BUT_CHOICE6_EXECUTE).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_BUT_CHOICE6_EXTORT] = CreateWindow("BUTTON", l.getMessage(Localized::SHANDAR_BUT_CHOICE6_EXTORT).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_BUT_CHOICE6_EXILE] = CreateWindow("BUTTON", l.getMessage(Localized::SHANDAR_BUT_CHOICE6_EXILE).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_CHOICE6_EXECUTED] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_CHOICE6_EXECUTED).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_CHOICE6_EXTORT] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_CHOICE6_EXTORT).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_BUT_CHOICE6_EXTORT_EXECUTE] = CreateWindow("BUTTON", l.getMessage(Localized::SHANDAR_BUT_CHOICE6_EXTORT_EXECUTE).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_CHOICE6_EXILE] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_CHOICE6_EXILE).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_BUT_CHOICE6_WHERE_GIRL] = CreateWindow("BUTTON", l.getMessage(Localized::SHANDAR_BUT_CHOICE6_WHERE_GIRL).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);

		// Found girl
		hItems[SHANDAR_STAT_FOUND_GIRL] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_FOUND_GIRL).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_BUT_GREET_GIRL] = CreateWindow("BUTTON", l.getMessage(Localized::SHANDAR_BUT_GREET_GIRL).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_GREET_GIRL] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_GREET_GIRL).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_BUT_LETS_LEAVE] = CreateWindow("BUTTON", l.getMessage(Localized::SHANDAR_BUT_LETS_LEAVE).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_LEAVE_QUIET] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_LEAVE_QUIET).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_LEAVE_LOUD] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_LEAVE_LOUD).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

		// Returned girl
		hItems[SHANDAR_STAT_RETURNED_DAUGHTER] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_RETURNED_DAUGHTER).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_BUT_ASK_REWARD] = CreateWindow("BUTTON", l.getMessage(Localized::SHANDAR_BUT_ASK_REWARD).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_REWARD] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_REWARD).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_BUT_GOODBYE] = CreateWindow("BUTTON", l.getMessage(Localized::SHANDAR_BUT_GOODBYE).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_GOODBYE] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_GOODBYE).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		buf = l.getMessage(Localized::SHANDAR_BUT_NAME) + " " + game.getPlayer().getName();
		hItems[SHANDAR_BUT_NAME] = CreateWindow("BUTTON", buf.c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		buf = l.getMessage(Localized::SHANDAR_STAT_THANKS) + " " + game.getPlayer().getName();
		hItems[SHANDAR_STAT_THANKS] = CreateWindow("STATIC", buf.c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[SHANDAR_STAT_END] = CreateWindow("STATIC", l.getMessage(Localized::SHANDAR_STAT_END).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

		isChoiceRequired = true;
		currentSubScreen = SHANDAR_STAT_INTRO;
	}
	break;
	}
	currentScreen = screen;
	game.updateBackground();
	updateWindow(hWnd);
}

void StoryScreen::setCurrentScreen(Screen cs) { currentScreen = cs; }

void StoryScreen::setCurrentSubScreen(int screen) { currentSubScreen = screen; }

StoryScreen::Screen StoryScreen::getCurrentScreen() const { return currentScreen; }

int StoryScreen::getCurrentSubScreen() const { return currentSubScreen; }
