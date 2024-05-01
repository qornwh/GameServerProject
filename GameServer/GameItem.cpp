#include "GameItem.h"

GameItem::GameItem()
{
    _itemMap.emplace(-1, Item(-1, -1, -1));
}

GameItem::~GameItem()
{
    _itemMap.clear();
}

void GameItem::AddItem(int32 code, int32 type, wchar_t* name, int32 maxSize)
{
    if (_itemMap.find(code) != _itemMap.end())
    {
        return;
    }
    _itemMap.emplace(code, Item(code, type, maxSize));
}

Item& GameItem::GetItem(int32 code)
{
    if (_itemMap.find(code) == _itemMap.end())
    {
        return _itemMap[-1];
    }
    return _itemMap[code];
}

void GameItem::ClearItem()
{
    _itemMap.clear();
}
