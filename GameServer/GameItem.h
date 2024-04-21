#pragma once
#include "Inventory.h"

class GameItem : public boost::enable_shared_from_this<GameItem>
{
public:
    GameItem();
    ~GameItem();
    void AddItem(int code, int type, wchar_t* name, int maxSize);
    Item& GetItem(int code);
    void ClearItem();

private:
    unordered_map<int, Item> _itemMap;
};

