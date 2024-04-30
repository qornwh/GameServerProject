#include "DummyPlayerInfo.h"

DummyPlayerInfo::DummyPlayerInfo()
{
#if AMODE == 0
    _speed = 400.f;
    _map.StartX = -2400;
    _map.StartY = -2400;
    _map.EndX = 2400;
    _map.EndY = 2400;
#elif AMODE == 1
    _speed = 5.f;
    _map.StartX = -25;
    _map.StartY = -25;
    _map.EndX = 25;
    _map.EndY = 25;
#endif
}

DummyPlayerInfo::~DummyPlayerInfo()
{
}

Vector3d& DummyPlayerInfo::GetPostion()
{
    return _position;
}

void DummyPlayerInfo::SetPostion(Vector3d vec)
{
    _position = vec;
}

void DummyPlayerInfo::UpdateRotate()
{
    _position.Yaw = rot(rng);

    _increaseX = cosf(_position.Yaw * 3.14f / 180.f) * _speed;
    _increaseY = sinf(_position.Yaw * 3.14f / 180.f) * _speed;
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
    MapRange* map = &_map;
    
    if (map->StartX >= _position.X || map->EndX <= _position.X)
        return false;
    if (map->StartY >= _position.Y || map->EndY <= _position.Y)
        return false;
    return true;
}

void DummyPlayerInfo::updatePosition()
{
    _position.X += _increaseX;
    _position.Y += _increaseY;
    
    MapRange* map = &_map;
    
    if (map->StartX >= _position.X)
        _position.X = map->StartX;
    if (map->EndX <= _position.X)
        _position.X = map->EndX;
    if (map->StartY >= _position.Y)
        _position.Y = map->StartY;
    if (map->EndY <= _position.Y)
        _position.Y = map->EndY;
}
