#include "ItemDB.h"
#include "GameGlobal.h"
#include "GameItem.h"

ItemDB::ItemDB() : _dbOrm(5)
{
}

ItemDB::~ItemDB()
{
}

void ItemDB::LoadDB()
{
	const wchar_t* query = L"SELECT itemCode, type, name, maxSize, attack FROM Item";
	conn = GDBPool->Pop();
	_dbOrm.SetDBConn(conn);
	conn->Exec(query);

	_dbOrm.BindColInt(sizeof(_itemCode), &_itemCode);
	_dbOrm.BindColInt(sizeof(_type), &_type);
	_dbOrm.BindColWchar(sizeof(_name), &_name);
	_dbOrm.BindColInt(sizeof(_maxSize), &_maxSize);
	_dbOrm.BindColInt(sizeof(_attack), &_attack);
}

bool ItemDB::GetItem(int32& itemCode, int32& type, wchar_t* name, int32& maxSize, int32& attack)
{
	if(conn->Fetch())
	{
		itemCode = _itemCode;
		type = _type;
		name = &_name[0];
		maxSize = _maxSize;
		attack = _attack;
		return true;
	}
	else
	{
		conn->CloseCursor();
		conn->FreeStmt();
		return false;
	}
}
