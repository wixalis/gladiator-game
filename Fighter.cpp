#include "stdafx.h"
#include "Fighter.h"

int Fighter::getDefense() const
{
	return isArmourEquipped() ? armour->getTotalDefense() : 0;
}

Fighter::Fighter() :
	Statistics(),
	hp(BASIC_HP),
	fullHP(BASIC_HP),
	inventory(make_unique<Inventory>()),
	rightHand(make_unique<Weapon>()),
	leftHand(make_unique<Weapon>()),
	armour(make_unique<Armour>())
{ }

Fighter::Fighter(
	const Statistics& rStats,
	int hhp,
	int ffullHP,
	unique_ptr<Inventory> pInventory,
	unique_ptr<Weapon> pRightHand,
	unique_ptr<Weapon> pLeftHand,
	unique_ptr<Armour> pArmour
) :
	Statistics(rStats),
	hp(hhp),
	fullHP(ffullHP),
	inventory(move(pInventory)),
	rightHand(move(pRightHand)),
	leftHand(move(pLeftHand)),
	armour(move(pArmour))
{		
	// TODO: think about processing exceptions
}

Fighter::Fighter(const Fighter& F) :
	Statistics(F),
	hp(F.hp),
	fullHP(F.fullHP)
{
	if (F.inventory)
		inventory = make_unique<Inventory>(*F.inventory);
	else
		inventory = nullptr;

	if (F.rightHand)
		rightHand = make_unique<Weapon>(*F.rightHand);
	else
		rightHand = nullptr;

	if (F.leftHand)
		leftHand = make_unique<Weapon>(*F.leftHand);
	else
		leftHand = nullptr;

	if (F.armour)
		armour = make_unique<Armour>(*F.armour);
	else
		armour = nullptr;
}

Fighter& Fighter::operator=(const Fighter& F)
{
	if (&F == this) return *this;

	Statistics::operator=(F);

	hp = F.hp;
	fullHP = F.fullHP;

	if (F.inventory)
	{
		if (!inventory)
			inventory = make_unique<Inventory>(*F.inventory);
		else
			*inventory = *F.inventory;
	}
	else
		inventory = nullptr;

	if (F.rightHand)
	{
		if (!rightHand)
			rightHand = make_unique<Weapon>(*F.rightHand);
		else
			*rightHand = *F.rightHand;
	}
	else
		rightHand = nullptr;

	if (F.leftHand)
	{
		if (!leftHand)
			leftHand = make_unique<Weapon>(*F.leftHand);
		else
			*leftHand = *F.leftHand;
	}
	else
		leftHand = nullptr;

	if (F.armour)
	{
		if (!armour)
			armour = make_unique<Armour>(*F.armour);
		else
			*armour = *F.armour;
	}
	else
		armour = nullptr;

	return *this;
}

Fighter::~Fighter() { }

void Fighter::setHP(int n) { hp = n; }

void Fighter::setFullHP(int n) { fullHP = n; }

void Fighter::updateMaxHP()
{
	double multiplier = (hp * 100 / fullHP) / 100;
	fullHP = 100 + (MAX_ADDITIONAL_HP_STRENGTH * strength / 100) + (MAX_ADDITIONAL_HP_CONSTITUTION * constitution / 100);
	hp = (int)(fullHP * multiplier);
}

void Fighter::unequipItem(int id)
{
	if (rightHand && rightHand->getID() == id) // Right hand weapon
	{
		inventory->addItem(move(rightHand));
	}
	else if (leftHand && leftHand->getID() == id) // Left hand weapon
	{
		inventory->addItem(move(leftHand));
	}
	else if (armour && armour->getID() == id) // Armour
	{
		inventory->addItem(move(armour));
	}
	else // Item not found
		throw exception("Cannot unequip item. It's not equipped.");
}

void Fighter::equipItemFromInventory(int id)
{
	// 1. Chech item validity
	if (!inventory->getItem(id))
		return; // If item is not found return

	// 2. Check item type
	const auto& rItem = inventory->getItem(id);
	Weapon* pWeapon;
	Armour* pArmour;
	switch (inventory->getItemType(id))
	{
	case Item::ItemType::WEAPON:
		pWeapon = dynamic_cast<Weapon*>(const_cast<Item*>(rItem));
		if (pWeapon) // If retrieved item is a weapon
		{
			if (!isRightHandOccupied() && pWeapon->getWeaponType() != Weapon::WeaponType::SHIELD) // If right hand is empty and retrieved weapon is not a shield
			{
				rightHand = make_unique<Weapon>(*pWeapon);
				inventory->removeItem(id);
			}
			else if (!isLeftHandOccupied() && rightHand->isCompatibleWith(pWeapon->getWeaponType())) // If left hand is empty and retrieved weapon is compatible with existing
			{
				leftHand = make_unique<Weapon>(*pWeapon);
				inventory->removeItem(id);
			}
			else
				throw exception("Weapon cannot be equipped. Slots are occupied or it's incompatible.");
		}
		break;

	case Item::ItemType::ARMOUR:
		pArmour = dynamic_cast<Armour*>(const_cast<Item*>(rItem));
		if (pArmour && !isArmourEquipped()) // If retrieved item is an armour and armour is not already equipped
		{
			armour = make_unique<Armour>(*pArmour);
			inventory->removeItem(id);
		}
		else
			throw exception("Armour cannot be equipped. Slot is occupied.");
		break;

	default: throw exception("Cannot equip item. The retrieved item is neither a weapon nor an armour.");
	}
}

void Fighter::attack(Fighter& rOpponent, AttackResult& rResult, int& rDamage)
{
	/*
	 * Determining whether the opponent has a shield:
	 * 0 -- no shield, 1 -- a left hand, 2 -- a right hand.
	 */
	int whereShield = 0;
	if (rOpponent.isLeftHandOccupied() && rOpponent.leftHand->getWeaponType() == Weapon::SHIELD)
		whereShield = 1;
	else if (rOpponent.isRightHandOccupied() && rOpponent.rightHand->getWeaponType() == Weapon::SHIELD)
		whereShield = 2;

	// Calculate total weapon damage
	int weaponDamage = 0;
	int statsDamage = 0;
	int totalDamage = 0;

	if (isLeftHandOccupied() && leftHand->getWeaponType() != Weapon::SHIELD)
		weaponDamage += leftHand->getTotalDamage();
	if (isRightHandOccupied() && rightHand->getWeaponType() != Weapon::SHIELD)
		weaponDamage += rightHand->getTotalDamage();

	// Increase damage by stats value
	statsDamage = (strength + dexterity) * 2;
	if (statsDamage > MAX_DAMAGE_FROM_STATS)
		statsDamage = MAX_DAMAGE_FROM_STATS;

	// If fighter doesn't have any weapon equipped - use hard stats
	if (weaponDamage == 0)
		totalDamage = (strength + dexterity) / 3;
	else
		totalDamage = weaponDamage + statsDamage;

	// Determining random damage spread - up to 10% less or more:
	int randomDamageSpread = rand() % WEAPON_MAX_RAND_DMG_SPREAD_PERCENT;
	if (rand() % 2)
		randomDamageSpread = -randomDamageSpread;

	// Is the attack evaded?
	if (
		rand() % 100 < rOpponent.dexterity * 3 / 10 +
		rOpponent.isArmourEquipped() ? rOpponent.armour->getEvasionProbAddition() : 0
		)
	{
		// Does the opponent counterattack?
		if (rand() % 100 < rOpponent.dexterity / 5)
		{
			rResult = AttackResult::WERE_COUNTERATTAKED;

			// Determining the opponent's damage
			int opponentsTotalDamage = 0;
			switch (whereShield)
			{
			case 0: // No shield
				if (rOpponent.isRightHandOccupied())
					opponentsTotalDamage += rOpponent.rightHand->getTotalDamage();
				// The left hand is checked when determining whether a shield
				opponentsTotalDamage += rOpponent.leftHand->getTotalDamage();
				break;
			case 1: // Left hand
				if (rOpponent.isRightHandOccupied())
					opponentsTotalDamage = rOpponent.rightHand->getTotalDamage();
				break;
			case 2: // Right hand
				// The left hand is checked when determining whether a shield
				opponentsTotalDamage = rOpponent.leftHand->getTotalDamage();
				break;
			}
			// Increase damage by stats value
			int opponentsStatsDamage = (strength + dexterity) * 2;
			if (opponentsStatsDamage > MAX_DAMAGE_FROM_STATS)
				opponentsStatsDamage = MAX_DAMAGE_FROM_STATS;
			// If fighter doesn't have any weapon equipped - use hard stats
			if (opponentsTotalDamage == 0)
				opponentsTotalDamage = (rOpponent.strength + rOpponent.dexterity) / 3;
			else
				opponentsTotalDamage += opponentsStatsDamage;

			// (Opponent damage - Player defense) is reduced to prolong a fight
			rDamage = opponentsTotalDamage * (100 - getDefense()) / 100 * // Basic damage formula
				(100 + randomDamageSpread) / 100 * // Random spread
				(ONE_HUNDRED_PERCENT - DAMAGE_REDUCTION_PERCENT) / ONE_HUNDRED_PERCENT; // Damage reduction to prolong a fight

			// Attack
			hp -= rDamage;
			if (rOpponent.hp < 0)
				rOpponent.hp = 0;
			return;
		}

		rResult = AttackResult::WERE_DODGED;
		rDamage = 0;
		return;
	}

	// Determining the opponent's block bonus if a shield is equipped
	int blockProbAddition = 0, blockDefPercentAddition = 0;
	switch (whereShield)
	{
	case 0: // No shield
		break;
	case 1: // Left hand
		blockProbAddition = rOpponent.leftHand->getShieldProbAddition();
		blockDefPercentAddition = rOpponent.leftHand->getShieldDefPercentAddition();
		break;
	case 2: // Right hand
		blockProbAddition = rOpponent.rightHand->getShieldProbAddition();
		blockDefPercentAddition = rOpponent.rightHand->getShieldDefPercentAddition();
		break;
	}

	/*
	 * Is the attack blocked?
	 *
	 * <= 35% probability + <= 30% reduction from the constitution and
	 * <= 15% probability + <= 60% reduction from a shield.
	 */
	if (rand() % 100 < rOpponent.constitution * 35 / 100 + blockProbAddition)
	{
		rResult = AttackResult::WERE_BLOCKED;
		// (Damage - Defense) is reduced by the block defense and by the percent to prolong fight
		rDamage = totalDamage * (100 - (rOpponent.constitution * 3 / 10 + blockDefPercentAddition)) / 100 * // Block defense
			(100 - rOpponent.getDefense()) / 100 * // Armour defense
			(100 + randomDamageSpread) / 100 *
			(ONE_HUNDRED_PERCENT - DAMAGE_REDUCTION_PERCENT) / ONE_HUNDRED_PERCENT;

		if (rDamage < 1)
			rDamage = 1;

		// Attack
		rOpponent.hp -= rDamage;

		if (rOpponent.hp < 0)
			rOpponent.hp = 0;

		return;
	}

	// Is the attack critical?
	if (rand() % 100 < dexterity * 3 / 10)
	{
		rResult = AttackResult::DEALT_CRIT_DAMAGE;
		/*
		 * (Player damage - Opponent defense) is increased by the crit (30% + <=40% from the dexterity) and
		 * reduced to prolong fight.
		 */
		rDamage = totalDamage * (100 - rOpponent.getDefense()) / 100 *
			(120 + dexterity * 3 / 10) / 100 *
			(100 + randomDamageSpread) / 100 *
			(ONE_HUNDRED_PERCENT - DAMAGE_REDUCTION_PERCENT) / ONE_HUNDRED_PERCENT;

		if (rDamage < 1)
			rDamage = 1;

		// Attack
		rOpponent.hp -= rDamage;

		if (rOpponent.hp < 0)
			rOpponent.hp = 0;

		return;
	}

	// Basic attack
	rResult = AttackResult::DEALT_DAMAGE;

	// Is the stun attack?
	if (
		rand() % 100 < strength * 15 / 100 -
		(rOpponent.isArmourEquipped() ? rOpponent.armour->getStunProbSubtraction() : 0)
		)
		rResult = AttackResult::STUNNED;

	// (Player damage - Opponent defense) is reduced to prolong a fight
	rDamage = totalDamage * (100 - rOpponent.getDefense()) / 100 *
		(100 + randomDamageSpread) / 100 *
		(ONE_HUNDRED_PERCENT - DAMAGE_REDUCTION_PERCENT) / ONE_HUNDRED_PERCENT;

	if (rDamage < 1)
		rDamage = 1;

	// Attack
	rOpponent.hp -= rDamage;

	if (rOpponent.hp < 0)
		rOpponent.hp = 0;
}

bool Fighter::isAlive() const { return hp > 0; }

bool Fighter::isRightHandOccupied() const { return rightHand != nullptr; }

bool Fighter::isLeftHandOccupied() const { return leftHand != nullptr; }

bool Fighter::isArmourEquipped() const { return armour != nullptr; }

int Fighter::getHP() const { return hp; }

int Fighter::getFullHP() const { return fullHP; }

const unique_ptr<Inventory>& Fighter::getInventory() const { return inventory; }

const unique_ptr<Weapon>& Fighter::getRightHand() const { return rightHand; }

const unique_ptr<Weapon>& Fighter::getLeftHand() const { return leftHand; }

const unique_ptr<Armour>& Fighter::getArmour() const { return armour; }
