#pragma once
#include "DropItem.h"
#include "pch.h"

class GameDrop : public std::enable_shared_from_this<GameDrop>
{
public:
    GameDrop() {}
    ~GameDrop() {}

    void AddDropItem(int32 monsterCode, int32 itemCode, int32 count);
    Vector<DropItem> GetMonsterDropList(int32 monsterCode);
    void ClearItem();

private:
    Map<int32, Vector<DropItem>> _monsterDropList;
};
