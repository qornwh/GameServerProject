#include "InventoryDB.h"

InventoryDB::InventoryDB() : _dbOrm(3)
{
    conn = GDBPool->Pop();
    _dbOrm.SetDBConn(conn);
}

InventoryDB::~InventoryDB()
{
    conn->EndTran();
    conn->CloseCursor();
    conn->FreeStmt();
}

void InventoryDB::LoadDB(int32 playerCode)
{
    const wchar_t* query = L"SELECT itemCode, itemType, itemCount FROM Inventory WHERE playerCode = ?";
    bool result = conn->Prepare(query);
    _dbOrm.BindParamInt(&playerCode);
    conn->Exec(query);

    _dbOrm.BindColInt(sizeof(_itemCode), &_itemCode);
    _dbOrm.BindColInt(sizeof(_type), &_type);
    _dbOrm.BindColInt(sizeof(_count), &_count);
}

bool InventoryDB::GetItem(int32& itemCode, int32& type, int32& count)
{
    if(conn->Fetch())
    {
        itemCode = _itemCode;
        type = _type;
        count = _count;
        return true;
    }
    else
    {
        return false;
    }
}

void InventoryDB::SaveInsertDB(int32 playerCode, int32 itemCode, int32 type, int32 count)
{
    conn->Prepare(insertQuery);
    _dbOrm.BindParamInt(&playerCode);
    _dbOrm.BindParamInt(&itemCode);
    _dbOrm.BindParamInt(&type);
    _dbOrm.BindParamInt(&count);
    conn->Exec(insertQuery);
    _dbOrm.ParamInit();
}

void InventoryDB::SaveUpdateDB(int32 playerCode, int32 itemCode, int32 type, int32 count)
{
    conn->Prepare(updateQuery);
    _dbOrm.BindParamInt(&count);
    _dbOrm.BindParamInt(&playerCode);
    _dbOrm.BindParamInt(&itemCode);
    conn->Exec(updateQuery);
    _dbOrm.ParamInit();
}
