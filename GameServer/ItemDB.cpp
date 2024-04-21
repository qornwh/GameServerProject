#include "ItemDB.h"
#include "GameGlobal.h"
#include "GameItem.h"

ItemDB::ItemDB() : _dbOrm(4)
{
}

ItemDB::~ItemDB()
{
}

void ItemDB::LoadDB()
{
	const wchar_t* query = L"SELECT itemCode, type, name, maxSize FROM Item";
	DBConnRef conn = GDBPool->Pop();
	_dbOrm.SetDBConn(conn);
	conn->Exec(query);

	_dbOrm.BindColInt(sizeof(_itemCode), &_itemCode);
	_dbOrm.BindColInt(sizeof(_type), &_type);
	_dbOrm.BindColWchar(sizeof(_name), &_name);
	_dbOrm.BindColInt(sizeof(_maxSize), &_maxSize);

	while (conn->Fetch())
	{
		GItem->AddItem(_itemCode, _type, _name, _maxSize);
	}
	conn->FreeStmt();

	//GDBPool->Push(conn);
}
