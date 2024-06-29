#include "stdafx.h"
#include "NewMenuStorage.h"

NewMenuStorage::NewMenuStorage() :
	age(MIN_AGE),
	unnassignedAttributes(STARTING_ADDITIONAL_ATTRIBUTES),
	strength(BASIC_ATTRIBUTES),
	constitution(BASIC_ATTRIBUTES),
	dexterity(BASIC_ATTRIBUTES),
	intelligence(BASIC_ATTRIBUTES),
	wisdom(BASIC_ATTRIBUTES),
	charisma(BASIC_ATTRIBUTES)
{ }

NewMenuStorage::NewMenuStorage(const NewMenuStorage& NMS) :
	age(NMS.age),
	unnassignedAttributes(NMS.unnassignedAttributes),
	strength(NMS.strength),
	constitution(NMS.constitution),
	dexterity(NMS.dexterity),
	intelligence(NMS.intelligence),
	wisdom(NMS.wisdom),
	charisma(NMS.charisma)
{ }

NewMenuStorage& NewMenuStorage::operator=(const NewMenuStorage& NMS)
{
	if (&NMS == this) return *this;

	age = NMS.age;
	unnassignedAttributes = NMS.unnassignedAttributes;
	strength = NMS.strength;
	constitution = NMS.constitution;
	dexterity = NMS.dexterity;
	intelligence = NMS.intelligence;
	wisdom = NMS.wisdom;
	charisma = NMS.charisma;
	return *this;
}
