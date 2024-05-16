#pragma once
#include "pch.h"

class DropItem
{
public:
    DropItem(int32 itemCode, int32 count) : _itemCode(itemCode), _count(count)
    {
    }
    ~DropItem() {}

    int32 GetItemCode() { return _itemCode; }
    int32 GetCount() { return _count; }

private:
    int32 _itemCode;
    int32 _count;
};
