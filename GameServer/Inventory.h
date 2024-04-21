#pragma once
#include "pch.h"
#include "ReadWriteLock.h"

class Item;
class ItemEquipment;

class Inventory
{
public:
    Inventory();
    ~Inventory();

    void Init(); // 초기 DB 데이터 로드
    void GetItemInfo(); // 아이템 정보 들고오기
    bool CheckGold(int32 gold); // 아이템, 머니 있는지 확인
    bool CheckItem(int32 code, int count = 1); // 아이템, 머니 있는지 확인

    void UseGold(int32 gold);
    void UseItem(int32 code, int count = 1);

    void SaveDB();

private:
    Lock lock;
    int32 _gold = 0;
    boost::weak_ptr<GamePlayerInfo> _playerInfo;
};

class Item
{
public:
    Item(int32 code, int32 type, int32 count);
    Item();
    ~Item();

    void Use(int32 count);
    void SetCount(int32 count);

private:
    int32 _code = 0;
    int32 _type = 0;
    int32 _count = 0;
};

class ItemEquipment
{
public:
    ItemEquipment(int32 code, int32 type, int32 attack, int32 count = 1);
    ~ItemEquipment();

    void Use(int32 count);
    void SetCount(int32 count);

private:
    int32 _code = 0;
    int32 _type = 0;
    int32 _attack = 0;
    int32 _count = 1;
};