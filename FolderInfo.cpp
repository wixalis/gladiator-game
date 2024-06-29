#include "stdafx.h"

bool FolderInfo::operator<(const FolderInfo& FI) const
{
	return mktime(const_cast<tm*>(&dateTime)) < mktime(const_cast<tm*>(&FI.dateTime));
}
