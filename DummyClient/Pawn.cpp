#include "Pawn.h"

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

Vector& DummyPlayerInfo::GetRotate()
{
    return _rotate;
}

void DummyPlayerInfo::SetRotate(Vector vec)
{
    _rotate = vec;
}

void DummyPlayerInfo::SetRandRotate()
{
    _rotate.Z = rot(rng);
}

void DummyPlayerInfo::Start()
{
    _use = true;

    _position.X = dstX(rng);
    _position.Y = dstY(rng);
    std::cout << "x : " << _position.X << " y : " << _position.Y << std::endl;
}

bool DummyPlayerInfo::IsMapRange()
{
    if (_map.StartX >= _position.X || _map.EndX <= _position.X)
        return false;
    if (_map.StartY >= _position.Y || _map.EndY <= _position.Y)
        return false;
    return true;
}
