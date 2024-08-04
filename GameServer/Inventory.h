#pragma once
#include "InventoryItem.h"
#include "pch.h"
#include "ReadWriteLock.h"

class Inventory
{
public:
    Inventory();
    ~Inventory();

    void Init(int32 playerCode, int32 gold); // 초기 DB 데이터 로드
    Map<int32, InventoryItem>& GetItemInfo(); // 아이템 정보 들고오기
    bool CheckGold(int32 gold); // 아이템, 머니 있는지 확인
    bool CheckItem(int32 code, int32 count = 1); // 아이템, 머니 있는지 확인
    void UseGold(int32 gold);
    void UseItem(int32 itemCode, int32 count = 1);
    void SaveDB();
    void AddGold(int32 gold);
    void AddItem(int32 itemCode, int32 type, int32 count);
    int32 GetGold() { return _gold; }

private:
    Lock lock;
    int32 _gold = 0;
    int32 _playerCode = -1;
    std::weak_ptr<GamePlayerInfo> _playerInfo;
    Map<int32, InventoryItem> _inventoryItemList;
};
