#include "GameItem.h"

GameItem::GameItem()
{
    _itemMap.emplace(-1, std::make_shared<Item>(-1, -1, -1, -1));
}

GameItem::~GameItem()
{
    _itemMap.clear();
}

void GameItem::AddItem(int32 code, int32 type, int32 maxSize, int32 attack)
{
    if (_itemMap.find(code) != _itemMap.end())
    {
        return;
    }
    _itemMap.emplace(code, std::make_shared<Item>(code, type, maxSize, attack));
}

ItemRef GameItem::GetItem(int32 code)
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
