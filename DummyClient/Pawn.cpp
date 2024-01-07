#include "Pawn.h"

Pawn::Pawn()
{
}

Pawn::~Pawn()
{
}

Vector& Pawn::GetPostion()
{
    return _position;
}

void Pawn::SetPostion(Vector vec)
{
    _position = vec;
}

Vector& Pawn::GetRotate()
{
    return _rotate;
}

void Pawn::SetRotate(Vector vec)
{
    _rotate = vec;
}

void Pawn::SetRandRotate()
{
    _rotate.X = rot(rng);
    _rotate.Y = rot(rng);
}

void Pawn::Start()
{
    _use = true;

    _position.X = dstX(rng);
    _position.Y = dstY(rng);
    std::cout << "x : " << _position.X << " y : " << _position.Y << std::endl;

    // x축 이동 범위 지정
    if (_position.X < _del + _range / 2)
    {
        _map.StartX = _del;
        _map.EndX = _del + _range;
    }
    else if (_position.X + _range / 2 > 20000)
    {
        _map.StartX = static_cast<int32>(_position.X) - _del - _range;
        _map.EndX = static_cast<int32>(_position.X) - _del;
    }
    else
    {
        _map.StartX = static_cast<int32>(_position.X) - _range / 2;
        _map.EndX = static_cast<int32>(_position.X) + _range / 2;
    }

    // y축 이동 범위 지정
    if (_position.Y < _del + _range / 2)
    {
        _map.StartY = _del;
        _map.EndY = _del + _range;
    }
    else if (_position.Y + _range / 2 > 30000)
    {
        _map.StartY = static_cast<int32>(_position.Y) - _del - _range;
        _map.EndY = static_cast<int32>(_position.Y) - _del;
    }
    else
    {
        _map.StartY = static_cast<int32>(_position.Y) - _range / 2;
        _map.EndY = static_cast<int32>(_position.Y) + _range / 2;
    }
}

bool Pawn::isMapRange()
{
    if (_map.StartX >= _position.X || _map.EndX <= _position.X)
        return false;
    if (_map.StartY >= _position.Y || _map.EndY <= _position.Y)
        return false;
    return true;
}
