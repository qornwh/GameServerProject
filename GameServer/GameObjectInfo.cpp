﻿#include "GameObjectInfo.h"

#include "GameGlobal.h"
#include "GameSession.h"
#include "GameSkill.h"
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

void GameObjectInfo::TakeDamage(int32 Damage)
{
    _hp -= Damage;
    if (_hp < 0)
        _hp = 0;
    _damage += Damage;
}

void GameObjectInfo::ResetDamage()
{
    _damage = 0;
}

void GameObjectInfo::SetObjecteState(ObjectState state)
{
    _state = state;
}

GameMosterInfo::GameMosterInfo(int32 uuid, int32 type, int32 hp, int32 startX, int32 startZ):
    GameObjectInfo(uuid, type, hp),
    _startX(startX), _startZ(startZ), _targetCode(-1), genYaw(0, 360)
{
}

GameMosterInfo::~GameMosterInfo()
{
}

void GameMosterInfo::SetObjecteState(ObjectState state)
{
    GameObjectInfo::SetObjecteState(state);

    switch (_state)
    {
    case ObjectState::IDLE:
        _IdleCounter.ResetTic();
        break;
    case ObjectState::DIE:
        _DieCounter.ResetTic();
        break;
    case ObjectState::HITED:
        _HitCounter.ResetTic();
        break;
    case ObjectState::MOVE:
        _MoveCounter.ResetTic();
        break;
    case ObjectState::ATTACK:
        _AttackCounter.ResetTic();
        break;
    }
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
    _targetCode = uuid;
}

void GameMosterInfo::Move()
{
    _prePosition = _position;
    if (_YawCounter.Add() == 0)
    {
        UpdateYaw();
    }

    // 기본 거리 3
    _position.X += (_increaseX * _speed);
    _position.Z += (_increaseZ * _speed);
}

void GameMosterInfo::updatePrePosition()
{
    _prePosition.X += _increaseX;
    _prePosition.Z += _increaseZ;
}

void GameMosterInfo::MoveTarget(FVector targetPosition)
{
    float theta = GameUtils::MathUtils::CalculateAngle(_position.Z, _position.X, targetPosition.Z, targetPosition.X);
    UpdateYaw(theta);

    if (CheckAttackTarget(targetPosition))
    {
        // 여기서만 바로 공격
        SetObjecteState(ObjectState::ATTACK);
    }
    else
    {
        // 타겟지점 이동
        _prePosition = _position;

        // 기본 거리 3
        _position.X += (_increaseX * _speed);
        _position.Z += (_increaseZ * _speed);
    }
}

bool GameMosterInfo::CheckAttackTarget(FVector targetPosition)
{
    // 임시로 하드코딩 해둠 나중에 함수화 해야됨 !!!
    int32 skillType = GSkill->GetMonsterSkill()[GetType()].GetSkillMap()[ObjectState::ATTACK]._type;
    bool isTargeting = GSkill->GetMonsterSkill()[GetType()].GetSkillMap()[ObjectState::ATTACK]._target;

    if (isTargeting)
    {
        if (skillType == Skill::RECT)
        {
            int32 rangeX = GSkill->GetMonsterSkill()[GetType()].GetSkillMap()[ObjectState::ATTACK]._width;
            int32 rangeZ = GSkill->GetMonsterSkill()[GetType()].GetSkillMap()[ObjectState::ATTACK]._height;

            float targetX = targetPosition.X - _position.X;
            float targetZ = targetPosition.Z - _position.Z;

            targetX = targetX * GameUtils::MathUtils::GetSin(_position.Yaw);
            targetZ = targetZ * GameUtils::MathUtils::GetCos(_position.Yaw);

            if (0 <= targetX && targetX < rangeX)
            {
                if (0 <= targetZ && targetZ < rangeZ)
                {
                    // 공격 성공
                    return true;
                }
            }
            return false;
        }
    }
    return false;
}

void GameMosterInfo::UpdateYaw()
{
    // room 1 tick 당 이동거리 계산
    _position.Yaw = genYaw(rng);
    _increaseX = GameUtils::MathUtils::GetSin(_position.Yaw) * (_speed / _MoveCounter.GetTickValue());
    _increaseZ = GameUtils::MathUtils::GetCos(_position.Yaw) * (_speed / _MoveCounter.GetTickValue());
}

void GameMosterInfo::UpdateYaw(float theta)
{
    _position.Yaw = theta;
    _increaseX = GameUtils::MathUtils::GetSin(_position.Yaw) * (_speed / _MoveCounter.GetTickValue());
    _increaseZ = GameUtils::MathUtils::GetCos(_position.Yaw) * (_speed / _MoveCounter.GetTickValue());
}

int32 GameMosterInfo::AddAttackCounter(int count)
{
    int32 value = _AttackCounter.Add(count);
    if (value == _AttackCounter.GetTickValue() - 1)
        SetObjecteState(ObjectState::MOVE);
    return value;
}

int32 GameMosterInfo::AddIdleCounter(int count)
{
    int32 value = _IdleCounter.Add(count);
    if (value == _IdleCounter.GetTickValue() - 1)
        SetObjecteState(ObjectState::MOVE);
    return value;
}

int32 GameMosterInfo::AddHitCounter(int count)
{
    int32 value = _HitCounter.Add(count);
    if (value == _HitCounter.GetTickValue() - 1)
        SetObjecteState(ObjectState::MOVE);
    return value;
}

int32 GameMosterInfo::AddMoveCounter(int count)
{
    return _MoveCounter.Add(count);
}

int32 GameMosterInfo::AddDieCounter(int count)
{
    int32 value = _DieCounter.Add(count);
    if (value == _DieCounter.GetTickValue() - 1)
    {
        _position.X = _startX;
        _position.Z = _startZ;
        _prePosition.X = _startX;
        _prePosition.Z = _startZ;
        _increaseX = 0;
        _increaseZ = 0;
        _hp = 100;
        _targetCode = -1;
        SetObjecteState(ObjectState::IDLE);
    }
    return value;
}

void GameMosterInfo::IdlePosition()
{
    _position.X = _prePosition.X;
    _position.Y = _prePosition.Y;
    _position.Z = _prePosition.Z;
    _increaseX = 0;
    _increaseZ = 0;
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

void GamePlayerInfo::Attack(GameMosterInfoRef target, vector<int32>& attackList)
{
    if (GetGameSession() == nullptr)
        return;

    int32 skillType = GSkill->GetPlayerSkill()[GetType()].GetSkillMap()[GetObjectState()]._type;
    if (target != nullptr)
    {
        // 타겟팅
        if (skillType == Skill::RECT)
        {
            if (AttackRect(target->GetPrePosition(), target))
            {
                attackList.push_back(target->GetCode());
            }
        }
        else if (skillType == Skill::CIRCLE)
        {
            if (AttackCircle(target->GetPrePosition(), target))
            {
                attackList.push_back(target->GetCode());
            }
        }
    }
    else
    {
        GameRoomRef gameRoom = GRoomManger->getRoom(GetGameSession()->GetRoomId());
        for (auto& it : gameRoom->GetMonsterMap())
        {
            // 논타겟
            if (skillType == Skill::RECT)
            {
                if (AttackRect(it.second->GetPrePosition(), it.second))
                {
                    attackList.push_back(it.second->GetCode());
                }
            }
            else if (skillType == Skill::CIRCLE)
            {
                if (AttackCircle(it.second->GetPrePosition(), it.second))
                {
                    attackList.push_back(it.second->GetCode());
                }
            }
        }
    }
}

bool GamePlayerInfo::AttackRect(FVector position, GameMosterInfoRef target)
{
    int32 rangeX = GSkill->GetPlayerSkill()[GetType()].GetSkillMap()[GetObjectState()]._width;
    int32 rangeZ = GSkill->GetPlayerSkill()[GetType()].GetSkillMap()[GetObjectState()]._height;

    float targetX = _position.X - (position.X + target->GetRangeX());
    float targetZ = _position.Z - (position.Z + target->GetRangeZ());

    targetX = targetX * GameUtils::MathUtils::GetSin(_position.Yaw);
    targetZ = targetZ * GameUtils::MathUtils::GetCos(_position.Yaw);

    if (targetX < rangeX)
    {
        if (targetZ < rangeZ)
        {
            // 공격 성공
            return true;
        }
    }

    return false;
}

bool GamePlayerInfo::AttackCircle(FVector position, GameMosterInfoRef target)
{
    int32 radius = GSkill->GetPlayerSkill()[GetType()].GetSkillMap()[GetObjectState()]._radius;

    float targetX = min(abs(_position.X - position.X), abs(_position.X - (position.X + target->GetRangeX())));
    float targetZ = min(abs(_position.Z - position.Z), abs(_position.Z - (position.Z + target->GetRangeZ())));

    float targetRadius = sqrtf(powf(targetX, 2) + powf(targetZ, 2));

    if (targetRadius < radius)
        // 공격 성공
        return true;

    return false;
}

void GamePlayerInfo::SetTarget(int32 uuid)
{
    _targetCode = uuid;
}
