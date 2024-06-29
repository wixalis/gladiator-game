#pragma once

class City
{
private:
	int nameIndex;
	Arena arena;
	int level;
	HarmlessNPC trader;
	bool isPromotionAchieved;
	bool isQuestCompleted;

public:
	City();
	City(int nameIndex_, const Arena&, int level_, const HarmlessNPC& trader_);
	City(const City&);
	City& operator=(const City&);
	~City();

	void setName(int);
	void setLevel(int);
	void setPromotion(bool);
	void setQuestCompletion(bool);

	int getNameIndex() const;
	Arena& getArena();
	int getLevel() const;
	HarmlessNPC& getTrader();
	bool getPromotionStatus() const;
	bool getQuestCompletion() const;

	// Save and load
	void saveToFile(const string&) const;
	void loadFromFile(const string&);
};
