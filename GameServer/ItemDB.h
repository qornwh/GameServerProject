#pragma once
#include <DBOrm.h>

class ItemDB
{
public:
	ItemDB();
	~ItemDB();

	void LoadDB();

private:
	DBOrm _dbOrm;
	int32 _itemCode = -1;
	int32 _type = -1;
	wchar_t _name[20] = {0,};
	int32 _maxSize = -1;
};
