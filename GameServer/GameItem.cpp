#include "GameItem.h"

GameItem::GameItem()
{
    // �ΰ� ����
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
        // ������ �ߺ� db üũ
        return;
    }
    _itemMap.emplace(code, Item(code, type, maxSize));
}

Item& GameItem::GetItem(int code)
{
    // TODO: ���⿡ return ���� �����մϴ�.
    if (_itemMap.find(code) == _itemMap.end())
    {
        // �׷� �������� ����.
        return _itemMap[-1];
    }
    return _itemMap[code];
}

void GameItem::ClearItem()
{
    _itemMap.clear();
}
