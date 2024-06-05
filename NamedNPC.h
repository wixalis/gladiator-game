#pragma once

class NamedNPC
{
protected:
	int firstNameIndex, lastNameIndex;
public:
	NamedNPC();
	NamedNPC(int firstNameIndex_, int lastNameIndex_);
	NamedNPC(const NamedNPC&);
	NamedNPC& operator=(const NamedNPC&);
	virtual ~NamedNPC();

	friend ostream& operator<<(ostream&, const NamedNPC&);
	friend istream& operator>>(istream&, NamedNPC&);

	void rename(int, int);

	int getFirstNameIndex() const;
	int getLastNameIndex() const;
};
