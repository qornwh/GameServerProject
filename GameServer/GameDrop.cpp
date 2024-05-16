#include "GameDrop.h"

void GameDrop::AddDropItem(int32 monsterCode, int32 itemCode, int32 count)
{
    if (_monsterDropList.find(monsterCode) == _monsterDropList.end())
    {
        _monsterDropList.emplace(monsterCode, Vector<DropItem>());
    }
    _monsterDropList[monsterCode].emplace_back(DropItem(itemCode, count));
}

Vector<DropItem> GameDrop::GetMonsterDropList(int32 monsterCode)
{
    if (_monsterDropList.find(monsterCode) == _monsterDropList.end())
        return Vector<DropItem>();
    return _monsterDropList[monsterCode];
}

void GameDrop::ClearItem()
{
    _monsterDropList.clear();
}
