#pragma once
#include "Inventory.h"

class GameItem : public std::enable_shared_from_this<GameItem>
{
public:
    GameItem();
    ~GameItem();
    void AddItem(int32 code, int32 type, wchar_t* name, int32 maxSize);
    Item& GetItem(int32 code);
    void ClearItem();

private:
    Map<int, Item> _itemMap;
};

