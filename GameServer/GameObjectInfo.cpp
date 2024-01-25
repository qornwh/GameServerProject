#include "GameObjectInfo.h"

#include "GameGlobal.h"
#include "GameSession.h"
#include "IRoom.h"

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

GameMosterInfo::GameMosterInfo(int32 uuid, int32 type, int32 hp, int32 roomUpdateTick): GameObjectInfo(uuid, type, hp),
    _startX(0), _startZ(0), _targetUUid(-1), _roomUpdateTick(roomUpdateTick), genYaw(0, 360)
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

int32 GameMosterInfo::AddAttackCounter(int count)
{
    return _AttackCounter.Add(count);
}

int32 GameMosterInfo::AddHitCounter(int count)
{
    return _HitCounter.Add(count);
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

vector<int32> GamePlayerInfo::Attack(GameMosterInfoRef target)
{
    vector<int32> attackList;

    if (GetGameSession())
        return attackList;

    if (target == nullptr)
    {
        // 타겟팅
        if (AttackRect(target->GetPrePosition(), target))
        {
            attackList.push_back(target->GetCode());
        }
    }
    else
    {
        GameRoomRef gameRoom = GRoomManger->getRoom(GetGameSession()->GetRoomId());
        for (auto& it : gameRoom->GetMonsterMap())
        {
            // 논타겟
            if (AttackRect(it.second->GetPrePosition(), it.second))
            {
                attackList.push_back(it.second->GetCode());
            }
        }
    }

    return attackList;
}

bool GamePlayerInfo::AttackRect(FVector& position, GameMosterInfoRef target)
{
    int32 rangeX = 5;
    int32 rangeZ = 10;

    float targetX = min(abs(_position.X - position.X), abs(_position.X - (position.X + target->GetRangeX())));
    float targetZ = min(abs(_position.Z - position.Z), abs(_position.Z - (position.Z + target->GetRangeZ())));

    if (0 < targetX && targetX < rangeX)
    {
        if (0 < targetZ && targetZ < rangeZ)
        {
            // 공격 성공
            return true;
        }
    }

    return false;
}

bool GamePlayerInfo::AttackCircle(FVector& position, GameMosterInfoRef target)
{
    int32 radius = 5;

    float targetX = min(abs(_position.X - position.X), abs(_position.X - (position.X + target->GetRangeX())));
    float targetZ = min(abs(_position.Z - position.Z), abs(_position.Z - (position.Z + target->GetRangeZ())));

    float targetRadius = sqrtf(powf(targetX, 2) + powf(targetZ, 2));

    if (radius > targetRadius)
        // 공격 성공
        return true;

    return false;
}
