#include "stdafx.h"
#include "Leveling.h"

Leveling::Leveling() : level(MIN_LEVEL) { }

Leveling::Leveling(int l) : level(l) { }

Leveling::Leveling(const Leveling& L) : level(L.level) { }

Leveling& Leveling::operator=(const Leveling& L)
{
	if (this == &L) return *this;
	level = L.level;
	return *this;
}

Leveling::~Leveling() { }

int Leveling::getLevel() const { return level; }