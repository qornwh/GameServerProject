#include "DummyPlayerInfo.h"

DummyPlayerInfo::DummyPlayerInfo()
{
}

DummyPlayerInfo::~DummyPlayerInfo()
{
}

Vector& DummyPlayerInfo::GetPostion()
{
    return _position;
}

void DummyPlayerInfo::SetPostion(Vector vec)
{
    _position = vec;
}

void DummyPlayerInfo::Start()
{
    _use = true;

    _position.X = dstX(rng);
    _position.Y = dstY(rng);
    std::cout << "x : " << _position.X << " y : " << _position.Y << std::endl;

    
}

void DummyPlayerInfo::UpdateMapRange()
{
}

bool DummyPlayerInfo::IsMapRange(int32 mapNum)
{
    MapRange* map = nullptr;
    if (mapNum == 1)
    {
        map = &_monsterMap;
    }
    else
    {
        map = &_bossmap;
    }
    if (map->StartX >= _position.X || map->EndX <= _position.X)
        return false;
    if (map->StartY >= _position.Y || map->EndY <= _position.Y)
        return false;
    return true;
}
