#include "GameItem.h"

GameItem::GameItem()
{
    // 널값 셋팅
    _itemMap.emplace(-1, Item(-1, -1, -1));
}

GameItem::~GameItem()
{
    _itemMap.clear();
}

void GameItem::AddItem(int code, int type, wchar_t* name, int maxSize)
{
    if (_itemMap.find(code) != _itemMap.end())
    {
        // 아이템 중복 db 체크
        return;
    }
    _itemMap.emplace(code, Item(code, type, maxSize));
}

Item& GameItem::GetItem(int code)
{
    // TODO: 여기에 return 문을 삽입합니다.
    if (_itemMap.find(code) == _itemMap.end())
    {
        // 그런 아이템은 없다.
        return _itemMap[-1];
    }
    return _itemMap[code];
}

void GameItem::ClearItem()
{
    _itemMap.clear();
}
