#include "stdafx.h"
#include "Fighting.h"

extern TCHAR str[256];
extern string buf;
extern string logStr;

extern HINSTANCE hInst;
extern Localization l;
extern Game game;

Fighting::Fighting() :
	hItems(MenuItem::ITEM_NUMBER),
	hBackgroundImage(NULL),
	hBackgroundBrush(NULL),
	isPlayerWon(false)
{ }

Fighting::Fighting(HWND hWnd) :
	hItems(MenuItem::ITEM_NUMBER),
	hBackgroundImage(NULL),
	hBackgroundBrush(NULL),
	isPlayerWon(false)
{
	for (int i = MenuItem::STATIC_START; i <= MenuItem::STATIC_FIGHT_RESULT; i++)
		hItems[i] = CreateWindow("STATIC", "",
			WS_CHILD | WS_VISIBLE | SS_CENTER | SS_OWNERDRAW,
			0, 0, 0, 0, hWnd, 0, hInst, 0);

	hItems[MenuItem::EDIT_LOG_MESSAGES] = CreateWindow("EDIT", "",
		WS_CHILD | WS_VISIBLE | ES_READONLY | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN | WS_BORDER,
		0, 0, 0, 0, hWnd, 0, hInst, 0);

	SendMessage(hItems[EDIT_LOG_MESSAGES], WM_SETFONT, (WPARAM)game.getFont(Game::FontSize::MEDIUM), TRUE);

	hItems[MenuItem::BUT_SPARE_OPPONENT] = CreateWindow("BUTTON", l.getMessage(Localized::SPARE_OPPONENT).c_str(),
		WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0);

	hItems[MenuItem::BUT_EXECUTE_OPPONENT] = CreateWindow("BUTTON", l.getMessage(Localized::EXECUTE_OPPONENT).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0);

	hItems[MenuItem::BUT_SURRENDER] = CreateWindow("BUTTON", l.getMessage(Localized::SURRENDER).c_str(),
		WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0);

	hItems[MenuItem::BUT_CONTINUE] = CreateWindow("BUTTON", l.getMessage(Localized::CONTINUE_FIGHT).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0);

	hItems[MenuItem::BUT_END_FIGHT] = CreateWindow("BUTTON", l.getMessage(Localized::END_FIGHT).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0);

	ShowWindow(hItems[MenuItem::BUT_SPARE_OPPONENT], SW_HIDE);
	ShowWindow(hItems[MenuItem::BUT_EXECUTE_OPPONENT], SW_HIDE);
	ShowWindow(hItems[MenuItem::BUT_SURRENDER], SW_HIDE);
	ShowWindow(hItems[MenuItem::BUT_CONTINUE], SW_HIDE);
	ShowWindow(hItems[MenuItem::BUT_END_FIGHT], SW_HIDE);
}

Fighting::Fighting(const Fighting& F) :
	hItems(),
	isPlayerWon(false)
{
	// Resizing items' vector
	int sz = F.hItems.size();
	hItems.resize(sz);

	HWND hItem;
	RECT rect;
	char className[256], windowText[256];
	for (int i = 0; i < sz; i++)
	{
		// Obtaining the previous parameters
		hItem = F.hItems[i];
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

	if (F.hBackgroundImage != NULL)
	{
		BITMAP bm;
		GetObject(F.hBackgroundImage, sizeof(BITMAP), &bm);
		hBackgroundImage = CreateBitmapIndirect(&bm);
	}
	else
		hBackgroundImage = NULL;

	if (F.hBackgroundBrush != NULL)
	{
		LOGBRUSH lb;
		GetObject(F.hBackgroundBrush, sizeof(LOGBRUSH), &lb);
		hBackgroundBrush = CreateBrushIndirect(&lb);
	}
	else
		hBackgroundBrush = NULL;
}

Fighting& Fighting::operator=(const Fighting& F)
{
	if (&F == this) return *this;

	// Destroying the previous items of the menu
	for (HWND hItem : hItems)
		if (hItem != NULL)
			DestroyWindow(hItem);

	// Resizing items' vector
	int sz = F.hItems.size();
	hItems.resize(sz);

	HWND hItem;
	RECT rect;
	char className[256], windowText[256];
	for (int i = 0; i < sz; i++)
	{
		// Obtaining the previous parameters
		hItem = F.hItems[i];
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

	if (F.hBackgroundImage != NULL)
	{
		BITMAP bm;
		GetObject(F.hBackgroundImage, sizeof(BITMAP), &bm);
		hBackgroundImage = CreateBitmapIndirect(&bm);
	}
	else
		hBackgroundImage = NULL;

	if (hBackgroundBrush != NULL)
		DeleteObject(hBackgroundBrush);

	if (F.hBackgroundBrush != NULL)
	{
		LOGBRUSH lb;
		GetObject(F.hBackgroundBrush, sizeof(LOGBRUSH), &lb);
		hBackgroundBrush = CreateBrushIndirect(&lb);
	}
	else
		hBackgroundBrush = NULL;

	isPlayerWon = F.isPlayerWon;

	return *this;
}

Fighting::~Fighting()
{
	for (HWND hItem : hItems)
		if (hItem != NULL)
			DestroyWindow(hItem);

	if (hBackgroundImage != NULL)
		DeleteObject(hBackgroundImage);

	if (hBackgroundBrush != NULL)
		DeleteObject(hBackgroundBrush);
}

FightStatus Fighting::fight(HWND hWnd, Player& rPlayer, NPC& rOpponent)
{
	SendMessage(hItems[EDIT_LOG_MESSAGES], WM_SETFONT, (WPARAM)game.getFont(Game::FontSize::MEDIUM), TRUE);
	AttackResult result = AttackResult::WERE_DODGED;
	FightStatus status = FightStatus::CONTINUE;
	isPlayerWon = false;

	int damage = 0, defense = 0;

	// Windows appearances
	for (HWND hItem : hItems)
		ShowWindow(hItem, SW_SHOW);

	ShowWindow(hItems[MenuItem::STATIC_FIGHT_RESULT], SW_HIDE);
	ShowWindow(hItems[MenuItem::BUT_SPARE_OPPONENT], SW_HIDE);
	ShowWindow(hItems[MenuItem::BUT_EXECUTE_OPPONENT], SW_HIDE);
	ShowWindow(hItems[MenuItem::BUT_SURRENDER], SW_HIDE);
	ShowWindow(hItems[MenuItem::BUT_CONTINUE], SW_HIDE);
	ShowWindow(hItems[MenuItem::BUT_END_FIGHT], SW_HIDE);

	game.updateBackground();

	UpdateWindow(hWnd);

	// Fight name
	buf = rPlayer.getName() + " " + l.getMessage(Localized::VERSUS).c_str() + " " + l.getNPCName(rOpponent);
	SendMessage(hItems[MenuItem::STATIC_START], WM_SETTEXT, 0, (LPARAM)buf.c_str());

	// Show Player and Opponent health
	int playerHP = game.getPlayer().getHP(),
		opponentHP = rOpponent.getHP();

	string pHP = l.getMessage(Localized::HEALTH) + ": " + to_string(playerHP),
		oHP = l.getMessage(Localized::HEALTH) + ": " + to_string(opponentHP);

	SendMessage(hItems[MenuItem::STATIC_PLAYER_HP], WM_SETTEXT, 0, (LPARAM)pHP.c_str());
	SendMessage(hItems[MenuItem::STATIC_OPPONENT_HP], WM_SETTEXT, 0, (LPARAM)oHP.c_str());
	SendMessage(hItems[MenuItem::EDIT_LOG_MESSAGES], WM_SETTEXT, 0, (LPARAM)"");

	// Show Player and Opponent damage
	if (rPlayer.getRightHand())
		damage += rPlayer.getRightHand()->getTotalDamage();
	if (rPlayer.getLeftHand())
		damage += rPlayer.getLeftHand()->getTotalDamage();
	buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(damage);
	SendMessage(hItems[MenuItem::STATIC_PLAYER_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
	damage = 0;

	if (rOpponent.getRightHand())
		damage += rOpponent.getRightHand()->getTotalDamage();
	if (rOpponent.getLeftHand())
		damage += rOpponent.getLeftHand()->getTotalDamage();
	buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(damage);
	SendMessage(hItems[MenuItem::STATIC_OPPONENT_DAMAGE], WM_SETTEXT, 0, (LPARAM)buf.c_str());
	damage = 0;

	// Show Player and Opponent defense
	if (rPlayer.getArmour())
		defense = rPlayer.getArmour()->getTotalDefense();
	buf = l.getMessage(Localized::ARMOUR_DEFENSE) + ": " + to_string(defense) + " %";
	SendMessage(hItems[MenuItem::STATIC_PLAYER_DEFENSE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

	defense = 0;
	if (rOpponent.getArmour())
		defense = rOpponent.getArmour()->getTotalDefense();
	buf = l.getMessage(Localized::ARMOUR_DEFENSE) + ": " + to_string(defense) + " %";
	SendMessage(hItems[MenuItem::STATIC_OPPONENT_DEFENSE], WM_SETTEXT, 0, (LPARAM)buf.c_str());

	// # 1. Determining whether the opponent attacks first
	if (rOpponent.getWisdom() > rPlayer.getWisdom())
	{
		buf = l.getMessage(Localized::OPPONENT_ATTACKS_FIRST) + "\r\n\r\n";
		SendMessage(hItems[MenuItem::EDIT_LOG_MESSAGES], WM_SETTEXT, 0, (LPARAM)buf.c_str());
		SendMessage(hItems[MenuItem::EDIT_LOG_MESSAGES], EM_SCROLL, SB_BOTTOM, 0);
		Sleep(FIGHT_SLEEP_TIME);

		// Opponent's attack
		rOpponent.attack(rPlayer, result, damage);

		getAttackResult(rOpponent, Attacker::OPPONENT, result, damage);
		Sleep(FIGHT_SLEEP_TIME);

		// Checking the status of the fighting
		status = checkFightStatus(rPlayer, rOpponent);

		if (status != FightStatus::CONTINUE)
		{
			if (status == FightStatus::PLAYER_LOST)
				rPlayer.setHP(1);
			getFightResult(status, rPlayer.getHP(), rOpponent.getHP());
			ShowWindow(hItems[MenuItem::STATIC_FIGHT_RESULT], SW_SHOW);
			ShowWindow(hItems[MenuItem::BUT_END_FIGHT], SW_SHOW);
		}
	}
	else
	{
		buf = l.getMessage(Localized::PLAYER_ATTACKS_FIRST) + "\r\n\r\n";
		SendMessage(hItems[MenuItem::EDIT_LOG_MESSAGES], WM_SETTEXT, 0, (LPARAM)buf.c_str());
		SendMessage(hItems[MenuItem::EDIT_LOG_MESSAGES], EM_SCROLL, SB_BOTTOM, 0);
		Sleep(FIGHT_SLEEP_TIME);
	}

	// # 2. Fighting
	while (status == FightStatus::CONTINUE)
	{
		// Checking whether the player is not stunned
		if (result != AttackResult::STUNNED)
		{
			// Player's attack
			rPlayer.attack(rOpponent, result, damage);

			// Output of the result of player's attack
			getAttackResult(rOpponent, Attacker::PLAYER, result, damage);
			Sleep(FIGHT_SLEEP_TIME);

			// Checking the status of the fighting
			status = checkFightStatus(rPlayer, rOpponent);

			if (status != FightStatus::CONTINUE)
			{
				getFightResult(status, rPlayer.getHP(), rOpponent.getHP());
				ShowWindow(hItems[MenuItem::STATIC_FIGHT_RESULT], SW_SHOW);
				ShowWindow(hItems[MenuItem::BUT_END_FIGHT], SW_SHOW);
				break;
			}
		}
		else
			result = AttackResult::DEALT_DAMAGE;

		// Checking whether the opponent is stunned
		if (result == AttackResult::STUNNED)
		{
			result = AttackResult::DEALT_DAMAGE;
			continue; // Skip opponent's attack
		}

		// Opponent's attack
		rOpponent.attack(rPlayer, result, damage);

		getAttackResult(rOpponent, Attacker::OPPONENT, result, damage);
		Sleep(FIGHT_SLEEP_TIME);

		// Checking the status of the fighting
		status = checkFightStatus(rPlayer, rOpponent);

		if (status != FightStatus::CONTINUE)
		{
			if (status == FightStatus::PLAYER_LOST)
				rPlayer.setHP(1);
			getFightResult(status, rPlayer.getHP(), rOpponent.getHP());
			ShowWindow(hItems[MenuItem::STATIC_FIGHT_RESULT], SW_SHOW);
			ShowWindow(hItems[MenuItem::BUT_END_FIGHT], SW_SHOW);
			break;
		}
	}
	// # 3. Fight rewards
	// # 3.1 Experience reward
	int experience = EXPERIENCE_PER_LEVEL;
	int playerLevel = rPlayer.getLevel();
	int opponentLevel = rOpponent.getLevel();

	// # 3.1.1 Calculating experience based on opponent's power compared to player
	double experienceMultiplier = 1.0;
	// Level multiplier
	// If player's level greater that opponent's level - player gains 10% more experience
	// If difference is more than 5 levels - player gains another 40% more experience and vice versa
	if (playerLevel > opponentLevel)
	{
		experienceMultiplier -= 0.3;
		if (playerLevel - 5 > opponentLevel)
			experienceMultiplier -= 0.6;
	}
	else
	{
		experienceMultiplier += 0.1;
		if (playerLevel + 5 < opponentLevel)
			experienceMultiplier += 0.4;

	}
	// # 3.1.2 Stats multiplier
	int playerPhysStats = rPlayer.getStrength() + rPlayer.getConstitution() + rPlayer.getDexterity();
	int opponentPhysStats = rOpponent.getStrength() + rOpponent.getConstitution() + rOpponent.getDexterity();
	int statsDifference;
	// Player gains 10% more or less experience for every 10 physical stats difference from opponent
	if (playerPhysStats > opponentPhysStats)
	{
		statsDifference = playerPhysStats - opponentPhysStats;
		do
		{
			statsDifference -= 10;
			experienceMultiplier -= 0.1;
		} while (statsDifference > 0);
	}
	else
	{
		statsDifference = opponentPhysStats - playerPhysStats;
		do
		{
			statsDifference -= 10;
			experienceMultiplier += 0.1;
		} while (statsDifference > 0);
	}

	// # 3.1.3 Player dealt damage multiplier
	// Player's experience gain will multiply by percentage of opponent's missing health
	// For example player lost and opponent has 90% hp remaining -> player gets 10% of experience
	experienceMultiplier *= (100 - ((double)rOpponent.getHP() * 100.0 / (double)rOpponent.getFullHP())) / 100;

	// Minimum value
	if (experienceMultiplier < MIN_EXPERIENCE_MULTIPLIER)
		experienceMultiplier = MIN_EXPERIENCE_MULTIPLIER;
	// Maximum value
	if (experienceMultiplier > MAX_EXPERIENCE_MULTIPLIER)
		experienceMultiplier = MAX_EXPERIENCE_MULTIPLIER;

	experience = (int)(experience * experienceMultiplier);
	rPlayer.gainExperience(experience);

	// # 3.2 Fame reward
	// Fame number calculates based on fame equilibrium for current level and opponent's fame
	int currentFame = rPlayer.getFame();
	int opponentFame = rOpponent.getFame();
	int fameEquilibrium = calculateFameForLevel(rPlayer.getLevel());
	int fameGainedForEquilibrium = 0;
	int fameGainedFromOpponent = 0;
	int gainedFame;

	// If player's current fame is lower than it's supposed to be for current level - gain additional fame
	if (currentFame < fameEquilibrium)
		fameGainedForEquilibrium = ((currentFame + fameEquilibrium) / 2) - currentFame;

	if (fameGainedForEquilibrium > MAX_FAME_GAINED_FOR_EQUILIBRIUM)
		fameGainedForEquilibrium = MAX_FAME_GAINED_FOR_EQUILIBRIUM;

	// Calculate fame gained from opponent's power based on winner
	switch (status)
	{
	case OPPONENT_LOST:case OPPONNENT_SURRENDERED:
		if (currentFame < opponentFame)
			fameGainedFromOpponent = opponentFame / 5;
		else
			fameGainedFromOpponent = opponentFame / 10;
		break;
	case PLAYER_LOST:case PLAYER_SURRENDERED:
		if (currentFame < opponentFame)
			fameGainedFromOpponent = opponentFame / 10;
		break;
	}
	if (fameGainedFromOpponent > MAX_FAME_GAINED_FROM_OPPONENT)
		fameGainedFromOpponent = MAX_FAME_GAINED_FROM_OPPONENT;

	// Total gained fame - fame for equilibrium + fame from opponent's fame + random number (0 - 200) adjusted for current level
	gainedFame = fameGainedForEquilibrium + fameGainedFromOpponent + (rand() % FAME_DISPERSION * rPlayer.getLevel() / 100);
	rPlayer.setFame(rPlayer.getFame() + gainedFame);

	// #3.3 Gold reward
	Inventory& rPlayerInventory = *rPlayer.getInventory();
	int traderLevel = game.getWorldMap().getCurrentCity().getTrader().getLevel();
	int goldForCity;
	double goldForFameMultiplier = 1.0;
	int gainedGold;
	// Calculate reward based on city
	switch (traderLevel)
	{
	case 1: goldForCity = LOW_VALUE_ITEM_LEVEL1; break;
	case 2: goldForCity = LOW_VALUE_ITEM_LEVEL2 / 2; break;
	case 3: goldForCity = LOW_VALUE_ITEM_LEVEL3 / 2; break;
	case 4: goldForCity = LOW_VALUE_ITEM_LEVEL4 / 2; break;
	case 5: goldForCity = LOW_VALUE_ITEM_LEVEL5 / 2; break;
	}

	// Calculate reward multiplier based on fame
	currentFame = rPlayer.getFame();
	while (currentFame > 0)
	{
		currentFame -= FAME_FOR_PERC_GOLD_MULTIPLIER;
		goldForFameMultiplier += 0.01;
	}

	gainedGold = (int)(goldForCity * goldForFameMultiplier);

	// If player lost reward is 10 times smaller
	if (status == FightStatus::PLAYER_SURRENDERED || status == FightStatus::PLAYER_LOST)
		gainedGold /= 10;

	// If player was promoted by lanista he gains another 20% gold
	if (game.getWorldMap().getCurrentCity().getPromotionStatus())
		gainedGold = gainedGold * 6 / 5;

	// Give player gold
	rPlayerInventory.addItem(make_unique<Item>(Item(Item::ItemType::GOLD)), gainedGold);

	// #3.4 Loot reward
	bool gainedLoot = false;
	string itemStr;
	// 30% chance to gain loot in case of victory
	if (
		rPlayerInventory.size() < MAX_INVENTORY_SIZE &&
		rand() % 100 < VICTORY_LOOT_CHANCE &&
		(status == FightStatus::OPPONENT_LOST || status == FightStatus::OPPONNENT_SURRENDERED)
		)
	{
		// 0 -- right hand
		// 1 -- left hand
		// 2 - armour
		int itemIndex = rand() % 3;
		int itemID = -1, replacementItemID;
		Weapon::WeaponType weaponType;
		unique_ptr<Item> pItem;
		unique_ptr<Weapon> pWeapon;
		unique_ptr<Armour> pArmour;

		switch (itemIndex)
		{
		case 0: // Right hand
			if (!rOpponent.getRightHand()) break;

			// Update scales for player's stats
			rOpponent.getRightHand()->update(rPlayer.getStrength(), rPlayer.getDexterity());
			// Get weapon type for replacement
			weaponType = rOpponent.getRightHand()->getWeaponType();
			itemID = rOpponent.getRightHand()->getID();
			// Unequip opponent's item
			rOpponent.unequipItem(itemID);

			// Generate new random weapon with the same level and type
			pWeapon = generateWeapon(traderLevel, weaponType);
			replacementItemID = pWeapon->getID();
			// Add new weapon to opponent's inventory
			rOpponent.getInventory()->addItem(move(pWeapon));
			// Equip weapon from inventory
			rOpponent.equipItemFromInventory(replacementItemID);
			break;

		case 1: // Left hand
			if (!rOpponent.getLeftHand()) break;

			// Update scales for player's stats
			rOpponent.getLeftHand()->update(rPlayer.getStrength(), rPlayer.getDexterity());
			// Get weapon type for replacement
			weaponType = rOpponent.getLeftHand()->getWeaponType();
			itemID = rOpponent.getLeftHand()->getID();
			// Unequip opponent's item
			rOpponent.unequipItem(itemID);

			// Generate new random weapon with the same level and type
			pWeapon = generateWeapon(traderLevel, weaponType);
			replacementItemID = pWeapon->getID();
			// Add new weapon to opponent's inventory
			rOpponent.getInventory()->addItem(move(pWeapon));
			// Equip weapon from inventory
			rOpponent.equipItemFromInventory(replacementItemID);
			break;

		case 2: // Armour
			if (!rOpponent.getArmour()) break;

			// Update scales for player's stats
			rOpponent.getArmour()->update(rPlayer.getStrength(), rPlayer.getDexterity());
			itemID = rOpponent.getArmour()->getID();
			// Unequip opponent's item
			rOpponent.unequipItem(itemID);

			// Generate new random armour with the same level
			pArmour = generateArmour(traderLevel);
			replacementItemID = pArmour->getID();
			// Add new armour to opponent's inventory
			rOpponent.getInventory()->addItem(move(pArmour));
			// Equip armour from inventory
			rOpponent.equipItemFromInventory(replacementItemID);
			break;
		}
		if (itemID != -1)
		{
			// Remove item from opponent's inventory
			pItem = rOpponent.getInventory()->extractItem(itemID);
			// Adjust price of the item based on player's charisma
			pItem->calculatePrice(rPlayer.getCharisma(), true);
			itemStr = l.getItemTypeName(*pItem);
			// Give it to player
			rPlayerInventory.addItem(move(pItem));
			gainedLoot = true;
		}
	}

	// Information for messages log
	// Fight info
	logStr += l.getMessage(Localized::YOU_HAVE_FOUGHT_WITH) + " " + l.getNPCName(rOpponent);
	if (status == FightStatus::OPPONENT_LOST || status == FightStatus::OPPONNENT_SURRENDERED)
		logStr += ". " + l.getMessage(Localized::YOU_HAVE_WON) + "\r\n\r\n";
	else
		logStr += ". " + l.getMessage(Localized::YOU_HAVE_LOST) + "\r\n\r\n";
	// Fame and experience
	logStr += l.getMessage(Localized::YOU_HAVE_GAINED) + " " + to_string(gainedFame) + " " + l.getMessage(Localized::FAME_GENITIVE) + "\r\n\r\n" +
		l.getMessage(Localized::YOU_HAVE_GAINED) + " " + to_string(experience) + " " + l.getMessage(Localized::EXPERIENCE_GENITIVE) +
		" (" + to_string(rPlayer.getExperience()) + " / " + to_string(rPlayer.calculateExperienceForOneLevel(rPlayer.getLevel() + 1)) + ")\r\n\r\n";
	// Gold
	logStr += l.getMessage(Localized::YOU_HAVE_GAINED) + " " + to_string(gainedGold) + " " + l.getMessage(Localized::GOLD_GENITIVE) + "\r\n\r\n";
	playSound(SoundEnum::SOUND_GOLD);
	// Loot
	if (gainedLoot)
		logStr += l.getMessage(Localized::YOU_HAVE_GAINED) + " " + itemStr + "\r\n\r\n";
	// Level up
	if (rPlayer.getLevel() > playerLevel)
	{
		Sleep(600); // Sleep so sound would have time to play
		logStr += l.getMessage(Localized::LEVELED_UP) + " " + to_string(rPlayer.getLevel()) +
			" (" + to_string(rPlayer.getUnnassignedAttributes()) + " " + l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES_GENITIVE) + ")\r\n\r\n";
		playSound(SoundEnum::SOUND_LEVEL_UP);
	}

	return status;
}

void Fighting::getAttackResult(const NPC& rOpponent, const Attacker attacker, const AttackResult rresult, const int ddamage)
{
	char logText[1024];
	GetWindowText(hItems[MenuItem::EDIT_LOG_MESSAGES], logText, sizeof(logText));

	string result(logText);

	switch (attacker)
	{
	case Attacker::PLAYER:
		switch (rresult)
		{
		case AttackResult::DEALT_DAMAGE:
			result += l.getMessage(Localized::YOU_HAVE_DEALT) + " " + to_string(ddamage) +  " " + l.getMessage(Localized::DAMAGE_GENITIVE);
			playSound(SoundEnum::SOUND_FIGHT_HIT);
			break;
		case AttackResult::DEALT_CRIT_DAMAGE:
			result += l.getMessage(Localized::YOU_HAVE_DEALT) + " " + to_string(ddamage) + " " + l.getMessage(Localized::CRITICAL_DAMAGE_GENITIVE);
			playSound(SoundEnum::SOUND_FIGHT_CRIT);
			break;
		case AttackResult::STUNNED:
			result += l.getMessage(Localized::YOU_HAVE_STUNNED) + " " + to_string(ddamage) + " " + l.getMessage(Localized::DAMAGE_GENITIVE);
			playSound(SoundEnum::SOUND_FIGHT_STUNNED);
			break;
		case AttackResult::WERE_DODGED:
			result += l.getMessage(Localized::YOU_MISSED);
			playSound(SoundEnum::SOUND_FIGHT_DODGED);
			break;
		case AttackResult::WERE_BLOCKED:
			result += l.getMessage(Localized::OPPONENT_BLOCKED) + " " + to_string(ddamage) + " " + l.getMessage(Localized::DAMAGE_GENITIVE);
			playSound(SoundEnum::SOUND_FIGHT_BLOCKED);
			break;
		case AttackResult::WERE_COUNTERATTAKED:
			result += l.getMessage(Localized::OPPONENT_COUNTERATTACKED) + " " + to_string(ddamage) + " " + l.getMessage(Localized::DAMAGE_GENITIVE);
			playSound(SoundEnum::SOUND_FIGHT_COUNTERATTACKED);
			break;
		default:
			// TODO: handle error
			break;
		}
		break;

	case Attacker::OPPONENT:
		switch (rresult)
		{
		case AttackResult::DEALT_DAMAGE:
			result += l.getMessage(Localized::OPPONENT_HAVE_DEALT) + " " + to_string(ddamage) + " " + l.getMessage(Localized::DAMAGE_GENITIVE);
			playSound(SoundEnum::SOUND_FIGHT_HIT);
			break;
		case AttackResult::DEALT_CRIT_DAMAGE:
			result += l.getMessage(Localized::OPPONENT_HAVE_DEALT) + " " + to_string(ddamage) + " " + l.getMessage(Localized::CRITICAL_DAMAGE_GENITIVE);
			playSound(SoundEnum::SOUND_FIGHT_CRIT);
			break;
		case AttackResult::STUNNED:
			result += l.getMessage(Localized::OPPONENT_HAVE_STUNNED) + " " + to_string(ddamage) + " " + l.getMessage(Localized::DAMAGE_GENITIVE);
			playSound(SoundEnum::SOUND_FIGHT_STUNNED);
			break;
		case AttackResult::WERE_DODGED:
			result += l.getMessage(Localized::OPPONENT_MISSED);
			playSound(SoundEnum::SOUND_FIGHT_DODGED);
			break;
		case AttackResult::WERE_BLOCKED:
			result += l.getMessage(Localized::YOU_HAVE_BLOCKED) + " " + to_string(ddamage) + " " + l.getMessage(Localized::DAMAGE_GENITIVE);
			playSound(SoundEnum::SOUND_FIGHT_BLOCKED);
			break;
		case AttackResult::WERE_COUNTERATTAKED:
			result += l.getMessage(Localized::YOU_HAVE_COUNTERATTACKED) + " " + to_string(ddamage) + " " + l.getMessage(Localized::DAMAGE_GENITIVE);
			playSound(SoundEnum::SOUND_FIGHT_COUNTERATTACKED);
			break;
		default:
			// TODO: handle error
			return;
		}
		break;
	}
	result += "\r\n\r\n";
	SendMessage(hItems[MenuItem::EDIT_LOG_MESSAGES], WM_SETTEXT, 0, (LPARAM)result.c_str());
	SendMessage(hItems[MenuItem::EDIT_LOG_MESSAGES], EM_SCROLL, SB_BOTTOM, 0);

	int playerHP = game.getPlayer().getHP(),
		opponentHP = rOpponent.getHP();

	string pHP = l.getMessage(Localized::HEALTH) + ": " + to_string(playerHP),
		   oHP = l.getMessage(Localized::HEALTH) + ": " + to_string(opponentHP);

	SendMessage(hItems[MenuItem::STATIC_PLAYER_HP], WM_SETTEXT, 0, (LPARAM)pHP.c_str());
	SendMessage(hItems[MenuItem::STATIC_OPPONENT_HP], WM_SETTEXT, 0, (LPARAM)oHP.c_str());
}

FightStatus Fighting::checkFightStatus(const Player & rPlayer, const NPC & rOpponent)
{
	if (rOpponent.getHP() < 10)
	{
		if (!rOpponent.isAlive())
		{
			isPlayerWon = true;
			return FightStatus::OPPONENT_LOST;
		}

		// Offer to surrender to the opponent
		// TODO: if (yes)
		//ShowWindow(hItems[Item::BUT_SPARE_OPPONENT], SW_SHOW);
		//ShowWindow(hItems[Item::BUT_EXECUTE_OPPONENT], SW_SHOW);
		// return FightStatus::OPPONNENT_SURRENDERED;
	}

	if (rPlayer.getHP() < 10)
	{
		if (!rPlayer.isAlive())
			return FightStatus::PLAYER_LOST;

		// Offer to surrender to the player
		// TODO: if (yes)
		//ShowWindow(hItems[Item::BUT_SURRENDER], SW_SHOW);
		//ShowWindow(hItems[Item::BUT_CONTINUE], SW_SHOW);
		// return FightStatus::PLAYER_SURRENDERED;
	}

	return FightStatus::CONTINUE;
}

void Fighting::getFightResult(const FightStatus sstatus, const int playerHP, const int opponentHP)
{
	string result;
	switch (sstatus)
	{
	case FightStatus::OPPONENT_LOST:
		result = l.getMessage(Localized::WON_OPPONENT_DEAD);
		break;
	case FightStatus::OPPONNENT_SURRENDERED:
		result = l.getMessage(Localized::WON_OPPONENT_SURRENDERED) + ": " + to_string(opponentHP) + ".";
		break;
	case FightStatus::PLAYER_SURRENDERED:
		result = l.getMessage(Localized::LOST_SURRENDER) + ": " + to_string(playerHP) + ".";
		break;
	case FightStatus::PLAYER_LOST:
		result = l.getMessage(Localized::LOST_DEAD);
		break;
	case FightStatus::CONTINUE:
		break;
	default:
		// TODO: handle error
		break;
	}
	SendMessage(hItems[MenuItem::STATIC_FIGHT_RESULT], WM_SETTEXT, 0, (LPARAM)result.c_str());
}

void Fighting::drawWindow(HWND hWnd, HDC hdc, int cx, int cy)
{
	if (game.isBackgroundChanged())
	{
		// 1. Background
		const string DIRECTORY = "Data/Image/";
		const string FORMAT = ".bmp";
		string path("");
		RECT rect;

		GetClientRect(hWnd, &rect);

		// Composing path based on current menu
		switch (game.getBackground())
		{
		default:case Game::Background::FIGHTING_ARENA: path = DIRECTORY + "Background/fightArenaBackground" + FORMAT; break;
		case Game::Background::FIGHTING_QUEST_PERUGIA_SHANDAR_STREET: path = DIRECTORY + "Quest/Perugia/Shandar/fightStreet" + FORMAT; break;
		case Game::Background::FIGHTING_QUEST_PERUGIA_SHANDAR_HOUSE: path = DIRECTORY + "Quest/Perugia/Shandar/fightHouse" + FORMAT; break;
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

void Fighting::resizeWindow(int cx, int cy)
{
	// If background is the same - no change needed
	if (!game.isBackgroundChanged())
		return;

	int x = cx,
		y = cy;

	switch (game.getBackground())
	{
	default:case Game::Background::FIGHTING_ARENA:
	{
		const int ITEM_HEIGHT = 30, ITEM_WIDTH = 300, DISTANCE = 15;
		// Start
		MoveWindow(hItems[STATIC_START], x - (int)((ITEM_WIDTH + DISTANCE) * 0.5), y - (ITEM_HEIGHT + DISTANCE) * 4, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		// HP
		MoveWindow(hItems[STATIC_PLAYER_HP], x - ITEM_WIDTH - DISTANCE, y, ITEM_WIDTH / 2, ITEM_HEIGHT, TRUE);

		MoveWindow(hItems[STATIC_OPPONENT_HP], x + DISTANCE + ITEM_WIDTH / 2, y, ITEM_WIDTH / 2, ITEM_HEIGHT, TRUE);

		// Damage
		MoveWindow(hItems[STATIC_PLAYER_DAMAGE], x - ITEM_WIDTH - DISTANCE, y + ITEM_HEIGHT + DISTANCE, ITEM_WIDTH / 2, ITEM_HEIGHT, TRUE);

		MoveWindow(hItems[STATIC_OPPONENT_DAMAGE], x + DISTANCE + ITEM_WIDTH / 2, y + ITEM_HEIGHT + DISTANCE, ITEM_WIDTH / 2, ITEM_HEIGHT, TRUE);

		// Defense
		MoveWindow(hItems[STATIC_PLAYER_DEFENSE], x - ITEM_WIDTH - DISTANCE, y + (ITEM_HEIGHT + DISTANCE) * 2, ITEM_WIDTH / 2, ITEM_HEIGHT, TRUE);

		MoveWindow(hItems[STATIC_OPPONENT_DEFENSE], x + DISTANCE + ITEM_WIDTH / 2, y + (ITEM_HEIGHT + DISTANCE) * 2, ITEM_WIDTH / 2, ITEM_HEIGHT, TRUE);

		// Log
		MoveWindow(hItems[EDIT_LOG_MESSAGES], x - (int)((ITEM_WIDTH + DISTANCE) * 0.5), y, ITEM_WIDTH, ITEM_HEIGHT * 5, TRUE);

		// Buttons
		MoveWindow(hItems[BUT_SPARE_OPPONENT], x - ITEM_WIDTH - DISTANCE, y + ITEM_HEIGHT * 5 + DISTANCE, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		MoveWindow(hItems[BUT_EXECUTE_OPPONENT], x + DISTANCE, y + ITEM_HEIGHT * 5 + DISTANCE, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		MoveWindow(hItems[BUT_SURRENDER], x - ITEM_WIDTH - DISTANCE, y + ITEM_HEIGHT * 5 + DISTANCE, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		MoveWindow(hItems[BUT_CONTINUE], x + DISTANCE, y + ITEM_HEIGHT * 5 + DISTANCE, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		// End of the fight
		MoveWindow(hItems[STATIC_FIGHT_RESULT], x - (int)((ITEM_WIDTH + DISTANCE) * 0.5), y + ITEM_HEIGHT * 5 + DISTANCE, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		MoveWindow(hItems[BUT_END_FIGHT], x - (int)((ITEM_WIDTH + DISTANCE) * 0.5), y + ITEM_HEIGHT * 6 + DISTANCE + 2, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
	}
	break;
	}
}

void Fighting::handleInput(HWND hWnd, UINT m, WPARAM wp, LPARAM lp)
{
	switch (m)
	{
	case WM_COMMAND:
		switch (game.getBackground())
		{
		default:case Game::Background::FIGHTING_ARENA:
		{
			if ((HWND)lp == hItems[BUT_END_FIGHT])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				game.setDisplayState(DisplayState::MENU);
				game.setBackground(Game::Background::CITY_MENU);

				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_HIDE);

				// Story progression
				Game::Progression currentStage = game.getProgressionStage();
				switch (currentStage)
				{
				case Game::Progression::START: // If it's player's first victory - display story screen
					if (isPlayerWon)
					{
						game.setProgressionStage(Game::Progression::FIRST_VICTORY);
						game.getStoryScreen().displayScreen(hWnd, StoryScreen::Screen::FIRST_VICTORY);
					}
					break;

				case Game::Progression::FIRST_VICTORY: // If current story stage is that player's saving up money for freedom
					if (game.getPlayer().getInventory()->getItemQuantity(0) >= MONEY_NEEDED_FOR_FREEDOM) // If player has enough money
					{
						game.setProgressionStage(Game::Progression::ENOUGH_MONEY_FOR_FREEDOM);
						game.getPlayer().getInventory()->removeItem(0, MONEY_NEEDED_FOR_FREEDOM);
						game.getStoryScreen().displayScreen(hWnd, StoryScreen::Screen::STOLEN_MONEY);
					}
					break;

				case Game::Progression::STAYED_AFTER_GLADIATOR_OFFER: // If player didn't escape after losing money
					if (isPlayerWon)
					{
						game.getStoryScreen().displayScreen(hWnd, StoryScreen::Screen::CENTURION_OFFER);
					}
					break;
				}
				updateWindow(hWnd);
			}
		}
		break;

		case Game::Background::FIGHTING_QUEST_PERUGIA_SHANDAR_STREET:
		{
			if ((HWND)lp == hItems[BUT_END_FIGHT])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				game.setDisplayState(DisplayState::STORY_SCREEN);
				game.setBackground(Game::Background::CITY_MENU);

				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_HIDE);

				updateWindow(hWnd);
			}
		}
		break;

		case Game::Background::FIGHTING_QUEST_PERUGIA_SHANDAR_HOUSE:
		{
			if ((HWND)lp == hItems[BUT_END_FIGHT])
			{
				playSound(SoundEnum::SOUND_BUTTON_CLICK);
				game.setDisplayState(DisplayState::STORY_SCREEN);
				game.setBackground(Game::Background::CITY_MENU);

				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_HIDE);

				updateWindow(hWnd);
			}
		}
		break;
		}
	break;
	}
}

bool Fighting::stylizeWindow(HWND hWnd, UINT m, WPARAM wp, LPARAM lp, LRESULT& result)
{
	switch (m)
	{
	case WM_DRAWITEM:
		{
		if (game.getBackground() == Game::Background::FIGHTING_ARENA)
		{
			LPDRAWITEMSTRUCT item = (LPDRAWITEMSTRUCT)lp;
			HDC hdc = item->hDC;

			GetClassName(item->hwndItem, str, sizeof(str) / sizeof(str[0]));

			// Set text font and background
			SelectObject(hdc, game.getFont(Game::FontSize::MEDIUM));
			SetBkMode(hdc, TRANSPARENT);

			// Get text
			int len = GetWindowTextLength(item->hwndItem);
			buf.resize(len + 1); // Resize buffer to contain button text
			GetWindowTextA(item->hwndItem, &buf[0], len + 1); // Write text into buffer

			SetTextColor(hdc, COLOR_WHITE); // Set basic text color

			FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_ROMAN_RED)); // Fill background
			DrawTextA(item->hDC, buf.c_str(), len, &item->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER); // Display text
			DrawEdge(hdc, &item->rcItem, EDGE_SUNKEN, BF_RECT); // Draw edge

			if (item->CtlType == ODT_BUTTON) // Button windows
			{
				if (item->itemState & ODS_SELECTED)
				{
					FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_ROMAN_RED_PUSHED)); // Fill background
					DrawTextA(item->hDC, buf.c_str(), len, &item->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER); // Display text
					DrawEdge(hdc, &item->rcItem, EDGE_RAISED, BF_RECT); // Draw edge
				}
				else
				{
					FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_ROMAN_RED)); // Fill background
					DrawTextA(item->hDC, buf.c_str(), len, &item->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER); // Display text
					DrawEdge(hdc, &item->rcItem, EDGE_RAISED, BF_RECT); // Draw edge
				}
			}
			return true;
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
