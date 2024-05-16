#include "InventoryItem.h"

InventoryItem::InventoryItem(int32 itemCode, int32 type, int32 count, InventoryItemState state) : _itemCode(itemCode), _count(count), _type(type), _state(state)
{
}

InventoryItem::InventoryItem(): _itemCode(0), _count(0), _type(0), _state(InventoryItemState::NONE)
{
}

InventoryItem::~InventoryItem()
{
}

void InventoryItem::UpdateItem(int32 count)
{
    if (_state != InventoryItemState::INSERT)
        _state = InventoryItemState::UPDATE;
    _count = count;
}