#pragma once

class Localization
{
private:
	Language language;
	array<string, Localized::MESSAGE_NUMBER> messages;
	vector<string> npcFirstNames;
	vector<string> npcLastNames;
	vector<string> gladiatorNicknames;
	vector<string> itemTypes;
	vector<string> weaponTypes;
	vector<string> armourTypes;
	vector<string> cityNames;

	// TODO: errorMessages
public:
	Localization();
	Localization(const Localization&);
	Localization& operator=(const Localization&);
	~Localization();

	void setLanguage(Language = ENGLISH);

	const string& operator[](int) const;
	Language getLanguage() const;
	string getMessage(Localized::Message) const;
	string getNPCName(const NamedNPC&) const;
	const string& getNPCFirstName(const NamedNPC&) const;
	const string& getNPCLastName(const NamedNPC&) const;
	const string& getNPCNickname(const NamedNPC&) const;
	const string& getItemTypeName(const Item&) const;
	const string& getWeaponTypeName(const Weapon&) const;
	const string& getArmourTypeName(const Armour&) const;
	const string& getCityName(const City&) const;
	// TODO: getErrorMessage()
};
