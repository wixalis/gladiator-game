#pragma once

// Forward declaration
class Weapon;
class Armour;

struct StartingEquipment
{
public:
	unique_ptr<Weapon> rightHand;
	unique_ptr<Weapon> leftHand;
	unique_ptr<Armour> armour;

	StartingEquipment();
	StartingEquipment(const StartingEquipment&);
	StartingEquipment& operator=(const StartingEquipment&);
	~StartingEquipment();
};
