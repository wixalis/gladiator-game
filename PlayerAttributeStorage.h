#pragma once
class PlayerAttributeStorage
{
public:
	int unnassignedAttributes,
		strength, constitution, dexterity, intelligence, wisdom, charisma,
		hp, fullHP;

	PlayerAttributeStorage();
	PlayerAttributeStorage(const PlayerAttributeStorage&);
	PlayerAttributeStorage& operator=(const PlayerAttributeStorage&);

	void updateMaxHP();
	double calculateAverage();
};
