#pragma once

class Fighting
{
private:
	vector<HWND> hItems;

	HBITMAP hBackgroundImage;
	HBRUSH hBackgroundBrush;

	bool isPlayerWon;
public:
	enum MenuItem
	{
		STATIC_START,
		STATIC_PLAYER_HP,
		STATIC_OPPONENT_HP,
		STATIC_PLAYER_DAMAGE,
		STATIC_OPPONENT_DAMAGE,
		STATIC_PLAYER_DEFENSE,
		STATIC_OPPONENT_DEFENSE,
		STATIC_FIGHT_RESULT,
		EDIT_LOG_MESSAGES,
		BUT_SPARE_OPPONENT,
		BUT_EXECUTE_OPPONENT,
		BUT_SURRENDER,
		BUT_CONTINUE,
		BUT_END_FIGHT,
		ITEM_NUMBER
	};
	enum Attacker
	{
		PLAYER,
		OPPONENT
	};

	Fighting();
	Fighting(HWND);
	Fighting(const Fighting&);
	Fighting& operator=(const Fighting&);
	~Fighting();

	FightStatus fight(HWND, Player&, NPC&);
	void getAttackResult(const NPC& opponent_, const Attacker, const AttackResult, const int damage_);
	FightStatus checkFightStatus(const Player&, const NPC&);
	void getFightResult(const FightStatus, const int playerHP_, const int opponentHP_);

	void drawWindow(HWND, HDC, int, int);
	void resizeWindow(int, int);
	void handleInput(HWND, UINT, WPARAM, LPARAM);
	bool stylizeWindow(HWND, UINT, WPARAM, LPARAM, LRESULT&);
};
