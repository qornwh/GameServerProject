#include "Inventory.h"
#include "DBConnectPool.h"

Inventory::Inventory()
{
}

Inventory::~Inventory()
{
}

void Inventory::Init()
{
    DBConnRef ref = GDBPool->Pop();
    CrashFunc(ref == nullptr);
}

void Inventory::GetItemInfo()
{
}

bool Inventory::CheckGold(int32 gold)
{
    return false;
}

bool Inventory::CheckItem(int32 code, int32 count)
{
    return false;
}

void Inventory::UseGold(int32 gold)
{
}

void Inventory::UseItem(int32 code, int32 count)
{
}

void Inventory::SaveDB()
{
}

Item::Item(int32 code, int32 type, int32 count) : _code(code), _type(type), _count(count)
{
}

Item::Item()
{
}

Item::~Item()
{
}

void Item::Use(int32 count)
{
}

void Item::SetCount(int32 count)
{
}

ItemEquipment::ItemEquipment(int32 code, int32 type, int32 attack, int32 count)
{
}

ItemEquipment::~ItemEquipment()
{
}

void ItemEquipment::Use(int32 count)
{
}

void ItemEquipment::SetCount(int32 count)
{
}
