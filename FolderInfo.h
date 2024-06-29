#pragma once

// Struct used to store save folder's data (name and time of creation) to further sort them in vector

struct FolderInfo {
	string name;
	tm dateTime;

	bool operator<(const FolderInfo&) const;
};
