#include "GameObjectInfo.h"

GameObjectInfo::GameObjectInfo(int32 uuid, int32 type, int32 hp) : _uuid(uuid), _type(type), _hp(hp)
{
}

GameObjectInfo::~GameObjectInfo()
{
}

void GameObjectInfo::SetPosition(FVector& position)
{
    _position.X = position.X;
    _position.Y = position.Y;
    _position.Z = position.Z;
}

void GameObjectInfo::SetPosition(float X, float Y, float Z)
{
    _position.X = X;
    _position.Y = Y;
    _position.Z = Z;
}

void GameObjectInfo::SetName(const string& name)
{
    _name = name;
}

void GameObjectInfo::SetObjecteState(ObjectState state)
{
    _state = state;
}

GameMosterInfo::GameMosterInfo(int32 uuid, int32 type, int32 hp, int32 roomUpdateTick): GameObjectInfo(uuid, type, hp), _startX(0),
                                                                  _startZ(0), _targetUUid(-1), _roomUpdateTick(roomUpdateTick), genYaw(0, 360)
{
}

GameMosterInfo::~GameMosterInfo()
{
}

void GameMosterInfo::SetStartPosition(int32 x, int32 z)
{
    UpdateYaw();
    _position.X = x;
    _position.Z = z;
}

void GameMosterInfo::GetStartPosition(int32& x, int32& z)
{
    x = _startX;
    z = _startZ;
}

void GameMosterInfo::SetTarget(int32 uuid)
{
    _targetUUid = uuid;
}

void GameMosterInfo::Move()
{
    _prePosition = _position;
    if (_YawCounter.Add() == 0)
    {
        UpdateYaw();
    }
    
    // 기본 거리 5
    _position.X += (_increaseX * _speed);
    _position.Z += (_increaseZ * _speed);
}

void GameMosterInfo::updatePrePosition()
{
    _prePosition.X += _increaseX;
    _prePosition.Z += _increaseZ;
}

void GameMosterInfo::TargetMove(int32 x, int32 z)
{
}

void GameMosterInfo::UpdateYaw()
{
    // room 1 tick 당 이동거리 계산
    _position.Yaw = genYaw(rng);
    _increaseX = GameUtils::MathUtils::GetSin(_position.Yaw) * (_speed / _roomUpdateTick);
    _increaseZ = GameUtils::MathUtils::GetCos(_position.Yaw) * (_speed / _roomUpdateTick);
}

GamePlayerInfo::GamePlayerInfo(GameSessionRef gameSession, int32 uuid, int32 type, int32 hp) : GameObjectInfo(
    uuid, type, hp)
{
    _gameSession = gameSession;
}

GamePlayerInfo::~GamePlayerInfo()
{
    cout << "close player info" << endl;
}
