#pragma once
#include "Fighter.h"
#include "Leveling.h"

class Player :
	public Fighter,
	public Leveling
{
private:
	string name;
	int portraitIndex;
	bool isExhausted;
	bool isFree;
public:
	Player();
	Player(const Fighter&, const Leveling&, const string&);
	Player(const Player&);
	Player& operator=(const Player&);
	virtual ~Player();

	void setName(const string&);
	void setPortaitIndex(int);
	void setExhaustion(bool);
	void setFreedom(bool);

	const string& getName() const;
	const int getPortaitIndex() const;
	const bool getExhaustion() const;
	const bool getFreedom() const;

	// Save and load
	void saveToFile(const string&) const;
	void loadFromFile(const string&);
};
