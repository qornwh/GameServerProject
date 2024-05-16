#pragma once
#include "pch.h"

enum InventoryItemState
{
    NONE,
    UPDATE,
    INSERT
};

class InventoryItem
{
public:
    InventoryItem(int32 itemCode, int32 type, int32 count, InventoryItemState state = InventoryItemState::NONE);
    InventoryItem();
    ~InventoryItem();

    void UpdateItem(int32 count = 1);
    InventoryItemState GetState() { return _state; }
    int32 GetCode() { return _itemCode; }
    int32 GetCount() { return _count; }
    int32 GetType() { return _type; }

private:
    int32 _itemCode;
    int32 _count;
    int32 _type;
    InventoryItemState _state;
};

