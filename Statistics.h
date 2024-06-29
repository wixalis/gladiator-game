#pragma once

class Statistics
{
protected:
	int strength, constitution, dexterity, intelligence, wisdom, charisma;
	int age;
	int fame;
public:
	Statistics();
	Statistics(
		int strength_,
		int constitution_,
		int dexterity_,
		int intelligence_,
		int wisdom_,
		int charisma_,
		int age_,
		int fame_
	);
	Statistics(const Statistics&);
	Statistics& operator=(const Statistics&);
	virtual ~Statistics();

	void setFame(int);

	bool isValid() const;
	int getStrength() const;
	int getConstitution() const;
	int getDexterity() const;
	int getIntelligence() const;
	int getWisdom() const;
	int getCharisma() const;
	int getAge() const;
	int getFame() const;

	void setStrength(int);
	void setConstitution(int);
	void setDexterity(int);
	void setIntelligence(int);
	void setWisdom(int);
	void setCharisma(int);
};
