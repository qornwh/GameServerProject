#include "DropItemDB.h"

DropItemDB::DropItemDB() : _dbOrm(2)
{
}

DropItemDB::~DropItemDB()
{
}

void DropItemDB::DropDB()
{
    const wchar_t* query = L"SELECT itemCode, monsterCode FROM ItemDrop";
    conn = GDBPool->Pop();
    _dbOrm.SetDBConn(conn);
    conn->Exec(query);

    _dbOrm.BindColInt(sizeof(_itemCode), &_itemCode);
    _dbOrm.BindColInt(sizeof(_monsterCode), &_monsterCode);
}

bool DropItemDB::GetDropItem(int32& itemCode, int32& monsterCode)
{
    if(conn->Fetch())
    {
        itemCode = _itemCode;
        monsterCode = _monsterCode;
        return true;
    }
    else
    {
        conn->CloseCursor();
        conn->FreeStmt();
        return false;
    }
}
