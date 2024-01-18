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

void GameObjectInfo::SetPosition(int32 X, int32 Y, int32 Z)
{
    _position.X = X;
    _position.Y = Y;
    _position.Z = Z;
}

void GameObjectInfo::SetName(const string& name)
{
    _name = name;
}

GameMosterInfo::GameMosterInfo(int32 uuid, int32 type, int32 hp): GameObjectInfo(uuid, type, hp), _startX(0),
                                                                  _startZ(0), _targetUUid(-1), genYaw(-20, 20)
{
}

GameMosterInfo::~GameMosterInfo()
{
}

void GameMosterInfo::SetStartPosition(int32 x, int32 z, float Yaw)
{
    _position.X = x;
    _position.Z = z;
    _position.Yaw = Yaw;
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

void GameMosterInfo::Move(int32 x, int32 z)
{
    _position.X = x;
    _position.Z = z;
}

void GameMosterInfo::TargetMove(int32 x, int32 z)
{
}

void GameMosterInfo::UpdateYaw()
{
    _position.Yaw += genYaw(rng);
    if (_position.Yaw < 0)
        _position.Yaw = 360 - _position.Yaw;
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
