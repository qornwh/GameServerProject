﻿#pragma once
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include "GameUtils.h"
#include "pch.h"

static boost::random::mt19937_64 rng;

enum ObjectState
{
    IDLE = 0,
    DIE = 1,
    HITED = 2,
    MOVE = 3,
    ATTACK = 4,
    SKILL1 = 5,
    SKILL2 = 6,
};

struct FVector
{
    FVector(float X, float Y, float Z, float Yaw) : X(X), Y(Y), Z(Z), Yaw(Yaw)
    {
    }

    float X;
    float Y;
    float Z;
    float Yaw;

    FVector& operator=(const FVector& ref)
    {
        X = ref.X;
        Z = ref.Z;
        return *this;
    }
};

class GameObjectInfo : public boost::enable_shared_from_this<GameObjectInfo>
{
public:
    GameObjectInfo(int32 uuid, int32 type, int32 hp);
    ~GameObjectInfo();

    void SetPosition(FVector& position);
    void SetPosition(float X, float Y, float Z);

    std::string& GetName() { return _name; }
    int32 GetCode() { return _uuid; }
    int32 GetType() { return _type; }
    int32 GetHp() { return _hp; }
    FVector& GetPosition() { return _position; }

    void SetName(const string& name);

    void SetObjecteState(ObjectState state);
    ObjectState GetObjectState() { return _state; }

protected:
    std::string _name;
    int32 _uuid;
    int32 _hp;
    int32 _type;

    FVector _position{0, 0, 0, 0};
    ObjectState _state = ObjectState::IDLE;
};

class GameMosterInfo : public GameObjectInfo
{
public:
    // 초기위치
    // 이동
    // 타겟 이동
    // 공격 (시간) - tict필요
    // 스킬 (시간) - tict필요
    GameMosterInfo(int32 uuid, int32 type, int32 hp);
    ~GameMosterInfo();

    void SetStartPosition(int32 x, int32 z);

    void GetStartPosition(int32& x, int32& z);

    void SetTarget(int32 uuid);
    int32 GetTarget() { return _targetUUid; }

    void Move();
    void TargetMove(int32 x, int32 z);

    void UpdateYaw();

    // room 언제보낼찌 500ms 카운트 변수
    // 10프레임 이거에 대한 카운트 변수
    // 몬스터 방향 전환 2초마다하면 카운트 변수 4절대갑 카운트 2개
    // 공격 카운트 변수 왜 공격하는동안은 공격만보여줘야됨 

private:
    int32 _startX;
    int32 _startZ;
    int32 _targetUUid;
    int32 _speed = 3;

    // 이전 위치
    FVector _prePosition{0, 0, 0, 0};

    boost::random::uniform_int_distribution<> genYaw;

    GameUtils::TickCounter _YawCounter{4};
};

class GamePlayerInfo : public GameObjectInfo
{
public:
    GamePlayerInfo(GameSessionRef gameSession, int32 uuid, int32 type, int32 hp);
    ~GamePlayerInfo();

    GameSessionRef GetGameSession() { return _gameSession.lock(); }

private:
    // 혹시나 해서 들고 있는다.
    boost::weak_ptr<GameSession> _gameSession;
};
