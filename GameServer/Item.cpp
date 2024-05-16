#include "Item.h"

Item::Item(int32 code, int32 type, int32 count, int32 attack) : _code(code), _type(type), _count(count), _attack(attack)
{
}

Item::~Item()
{
}

void Item::SetCode(int32 code)
{
    _code = code;
}

void Item::SetType(int32 type)
{
    _type = type;
}

void Item::SetAttack(int32 attack)
{
    _attack = attack;
}

void Item::SetCount(int32 count)
{
    _count = count;
}
