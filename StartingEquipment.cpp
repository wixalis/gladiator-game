#include "stdafx.h"
#include "StartingEquipment.h"

StartingEquipment::StartingEquipment()
{
	// Generating random weapons
	int itemTier = 1;
	rightHand = generateWeapon(itemTier);
	if (rightHand->getWeaponType() != Weapon::WeaponType::AXE && rightHand->getWeaponType() != Weapon::WeaponType::SPEAR)
	{
		leftHand = generateWeapon(itemTier);
		if (!rightHand->isCompatibleWith(leftHand->getWeaponType()))
		{
			if (rand() % 100 < 75)
			{
				if (leftHand)
					leftHand.release();
				leftHand = generateWeapon(itemTier, Weapon::SHIELD);
			}
			else if (rand() % 100 < 75)
				do
				{
					if (leftHand)
						leftHand.release();
					leftHand = generateWeapon(itemTier);
				} while (!rightHand->isCompatibleWith(leftHand->getWeaponType()));
			else if (leftHand)
			{
				leftHand.release();
				leftHand = nullptr;
			}
		}
	}
	else
		leftHand = nullptr;

	// Generating random armour
	armour = generateArmour(itemTier);
}

StartingEquipment::StartingEquipment(const StartingEquipment& SE)
{
	// Right hand weapon
	if (SE.rightHand)
		rightHand = make_unique<Weapon>(*SE.rightHand);
	else
		rightHand = nullptr;

	// Left hand weapon
	if (SE.leftHand)
		leftHand = make_unique<Weapon>(*SE.leftHand);
	else
		leftHand = nullptr;

	// Armour
	if (SE.armour)
		armour = make_unique<Armour>(*SE.armour);
	else
		armour = nullptr;
}

StartingEquipment& StartingEquipment::operator=(const StartingEquipment& SE)
{
	if (&SE == this) return *this;

	// Right hand weapon
	if (SE.rightHand)
	{
		if (!rightHand)
			rightHand = make_unique<Weapon>(*SE.rightHand);
		else
			*rightHand = *SE.rightHand;
	}
	else
		rightHand = nullptr;

	// Left hand weapon
	if (SE.leftHand)
	{
		if (!leftHand)
			leftHand = make_unique<Weapon>(*SE.leftHand);
		else
			*leftHand = *SE.leftHand;
	}
	else
		leftHand = nullptr;

	// Armour
	if (SE.armour)
	{
		if (!armour)
			armour = make_unique<Armour>(*SE.armour);
		else
			*armour = *SE.armour;
	}
	else
		armour = nullptr;

	return *this;
}

StartingEquipment::~StartingEquipment() { }
