#pragma once

class WorldMap
{
private:
	vector<HWND> hItems;

	vector<City> cities;
	int currentCity;
	int selectedCity;

	HBITMAP hBackgroundImage;
	HBRUSH hBackgroundBrush;

public:
	enum Item
	{
		BUT_ROME_MAP,
		BUT_NAPLES_MAP,
		BUT_METAPONTO_MAP,
		BUT_BOJANO_MAP,
		BUT_ANCONA_MAP,
		BUT_PERUGIA_MAP,
		BUT_FLORENCE_MAP,
		BUT_BOLOGNA_MAP,
		BUT_GENOA_MAP,
		BUT_AQUILEIA_MAP,
		BUT_MILAN_MAP,
		BUT_ROME_LIST,
		BUT_NAPLES_LIST,
		BUT_METAPONTO_LIST,
		BUT_BOJANO_LIST,
		BUT_ANCONA_LIST,
		BUT_PERUGIA_LIST,
		BUT_FLORENCE_LIST,
		BUT_BOLOGNA_LIST,
		BUT_GENOA_LIST,
		BUT_AQUILEIA_LIST,
		BUT_MILAN_LIST,
		BUT_PLAYER_ICON_MAP,
		BUT_PLAYER_ICON_LIST,
		STAT_MAP,
		BUT_TRAVEL_LIST,
		ITEM_NUMBER
	};
	enum Screen
	{
		SCREEN_ITEM_NUMBER
	};
	WorldMap();
	WorldMap(HWND);
	WorldMap(HWND, const vector<City>& cities_, int currentCity_ = 0);
	WorldMap(const WorldMap&);
	WorldMap& operator=(const WorldMap&);
	~WorldMap();

	City& getCurrentCity();
	int getCurrentCityIndex() const;
	City& getCity(int);

	void setCurrentCityIndex(int);

	void drawWindow(HWND, HDC, int, int);
	void resizeWindow(int, int);
	void handleInput(HWND, UINT, WPARAM, LPARAM);
	bool stylizeWindow(HWND, UINT, WPARAM, LPARAM, LRESULT&);

	void selectCity(HWND, Item city_);

	// Save and load
	void saveToFile(const string&) const;
	void loadFromFile(const string&);
};
