#pragma once
#include "Fighter.h"
#include "NamedNPC.h"
#include "Leveling.h"

class NPC :
	public Fighter,
	public NamedNPC,
	public Leveling
{
private:
	int portraitIndex;

public:
	NPC();
	NPC(const Fighter&, const NamedNPC&, const Leveling&, int portraitIndex_);
	NPC(const NPC&);
	NPC& operator=(const NPC&);
	virtual ~NPC();

	unique_ptr<NPC> clone() const;

	void setPortraitIndex(int);

	const int getPortraitIndex() const;

	// Save and load
	void saveToFile(const string&) const;
	void loadFromFile(const string&);
};
