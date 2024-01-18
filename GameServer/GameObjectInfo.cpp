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

void GameObjectInfo::SetObjecteState(ObjectState state)
{
    _state = state;
}

GameMosterInfo::GameMosterInfo(int32 uuid, int32 type, int32 hp): GameObjectInfo(uuid, type, hp), _startX(0),
                                                                  _startZ(0), _targetUUid(-1), genYaw(0, 7)
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
    const int32 val = static_cast<int32>(_position.Yaw) / 30;
    if (val == 7 || val <= 1)
    {
        // z 축 증가
        _position.Z++;
    }
    else if (3 <= val && val <= 5)
    {
        // z 축 감소
        _position.Z--;
    }
    if (1 <= val && val <= 3)
    {
        // x 축 증가
        _position.X++;
    }
    else if (5 <= val && val <= 7)
    {
        // x 축 감소 
        _position.X--;
    }
}

void GameMosterInfo::TargetMove(int32 x, int32 z)
{
}

void GameMosterInfo::UpdateYaw()
{
    _position.Yaw = genYaw(rng) * 45;
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
