#pragma once

class Leveling
{
protected:
	int level;
	int experience;
	int unnassignedAttributes;
public:
	Leveling();
	Leveling(int level_, int experience_ = 0, int unnassignedAttributes_ = 0);
	Leveling(const Leveling&);
	Leveling& operator=(const Leveling&);
	virtual ~Leveling();

	int getLevel() const;
	int getExperience() const;
	int getUnnassignedAttributes();

	void setLevel(int);
	void setExperience(int);
	void setUnnassignedAttributes(int);

	void gainExperience(int);
	int calculateExperienceForOneLevel(int) const;
	int calculateExperienceForLevel(int) const;

private:
	int calculateLevelForExperience(int);
};
