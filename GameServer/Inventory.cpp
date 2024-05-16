#include "Inventory.h"
#include "DBConnectPool.h"
#include "InventoryDB.h"
#include "SessionDB.h"

Inventory::Inventory()
{
}

Inventory::~Inventory()
{
}

void Inventory::Init(int32 playerCode, int32 gold)
{
    _playerCode = playerCode;
    InventoryDB inventoryDB;
    inventoryDB.LoadDB(_playerCode);
    
    int32 itemCode = 0;
    int32 itemType = 0;
    int32 itemCount = 0;

    while(inventoryDB.GetItem(itemCode, itemType, itemCount))
    {
        _inventoryItemList.emplace(itemCode, InventoryItem(itemCode, itemType, itemCount, InventoryItemState::NONE));
    }
    _gold = gold;
}

void Inventory::GetItemInfo()
{
}

bool Inventory::CheckGold(int32 gold)
{
    if (_gold >= gold)
        return true;
    return false;
}

bool Inventory::CheckItem(int32 code, int32 count)
{
    if (_inventoryItemList.find(code) != _inventoryItemList.end())
    {
        if (_inventoryItemList[code].GetCount() >= count)
            return true;
    }
    return false;
}

void Inventory::UseGold(int32 gold)
{
    if (CheckGold(gold))
        gold += gold;
}

void Inventory::UseItem(int32 itemCode, int32 count)
{
    if (CheckItem(itemCode, count))
    {
        int curCount = _inventoryItemList[itemCode].GetCount();
        _inventoryItemList[itemCode].UpdateItem(curCount - count);
    }
}

void Inventory::SaveDB()
{
    InventoryDB inventoryDB;
    for (auto inventoryItem : _inventoryItemList)
    {
        InventoryItem& item = inventoryItem.second;

        switch (item.GetState())
        {
        case InventoryItemState::INSERT:
            {
                inventoryDB.SaveInsertDB(_playerCode, item.GetCode(), item.GetType(), item.GetCount());
            }
            break;
        case InventoryItemState::UPDATE:
            {
                inventoryDB.SaveUpdateDB(_playerCode, item.GetCode(), item.GetType(), item.GetCount());
            }
            break;
        default:
            break;
        }
    }

    // 골드 DB기록
    SessionDB sessionDB;
    sessionDB.SavePlayerDB(_playerCode, _gold);
}

void Inventory::AddGold(int32 gold)
{
    _gold += gold;
}

void Inventory::AddItem(int32 itemCode, int32 type, int32 count)
{
    if (_inventoryItemList.find(itemCode) != _inventoryItemList.end())
    {
        int curCount = _inventoryItemList[itemCode].GetCount();
        _inventoryItemList[itemCode].UpdateItem(curCount + count);
    }
    else
    {
        _inventoryItemList.emplace(itemCode, InventoryItem(itemCode, type, count, InventoryItemState::INSERT));
    }
}
