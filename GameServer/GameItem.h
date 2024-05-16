#pragma once
#include "Item.h"

class GameItem : public std::enable_shared_from_this<GameItem>
{
public:
    GameItem();
    ~GameItem();
    void AddItem(int32 code, int32 type, int32 maxSize, int32 attack);
    ItemRef GetItem(int32 code);
    void ClearItem();

private:
    Map<int32, ItemRef> _itemMap;
};

