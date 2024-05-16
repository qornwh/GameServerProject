#pragma once
#include "pch.h"

class Item : public std::enable_shared_from_this<Item>
{
public:
    Item(int32 code, int32 type, int32 count, int32 attack);
    ~Item();

    int32 GetCode() { return _code; }
    void SetCode(int32 code);
    int32 GetType() { return _type; }
    void SetType(int32 type);
    int32 GetAttack() { return _attack; }
    void SetAttack(int32 attack);
    int32 GetCount() { return _count; }
    void SetCount(int32 count);

private:
    int32 _code = 0;
    int32 _type = 0;
    int32 _attack = 0;
    int32 _count = 0;
};
