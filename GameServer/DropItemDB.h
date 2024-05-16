#pragma once
#include "DBOrm.h"

class DropItemDB
{
public:
    DropItemDB();
    ~DropItemDB();

    void DropDB();
    bool GetDropItem(int32& itemCode, int32& monsterCode);

private:
    DBOrm _dbOrm;
    int32 _itemCode = -1;
    int32 _monsterCode = -1;

    DBConnRef conn;
};
