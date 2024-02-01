#pragma once
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include "GameRoomManager.h"
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

enum GameObjectType
{
    PLAYER = 0,
    MOMSTER = 1,
    OBJECT = 2,
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
    void SetPosition(float X, float Y, float Z, float Yaw);

    std::string& GetName() { return _name; }
    int32 GetCode() { return _uuid; }
    int32 GetType() { return _type; }
    int32 GetHp() { return _hp; }
    FVector& GetPosition() { return _position; }

    void SetName(const string& name);
    void TakeDamage(int32 Damage);
    void TakeHeal(int32 heal);
    void ResetDamage();
    void ResetHeal();
    int32 GetDamage() { return _damage; }
    int32 GetHeal() { return _heal; }

    virtual void SetObjecteState(ObjectState state);
    ObjectState GetObjectState() { return _state; }

    void SetGameRoom(GameRoomRef gameRoom) { _gameRoomRef = gameRoom; }
    GameRoomRef GetGameRoom() { return _gameRoomRef.lock(); }

protected:
    std::string _name;
    int32 _uuid;
    int32 _hp;
    int32 _damage;
    int32 _heal;
    int32 _type;

    FVector _position{0, 0, 0, 0};
    ObjectState _state = ObjectState::IDLE;

    boost::weak_ptr<GameRoom> _gameRoomRef;
};

class GameMosterInfo : public GameObjectInfo
{
public:
    // 초기위치
    // 이동
    // 타겟 이동
    // 공격 (시간) - tict필요
    // 스킬 (시간) - tict필요
    GameMosterInfo(int32 uuid, int32 type, int32 hp, int32 startX, int32 startZ);
    ~GameMosterInfo();

    void SetObjecteState(ObjectState state) override;

    void SetStartPosition(int32 x, int32 z);
    void GetStartPosition(int32& x, int32& z);

    void SetTarget(int32 uuid);
    int32 GetTarget() { return _targetCode; }

    void Move();
    void updatePrePosition();
    void MoveTarget(FVector targetPosition);
    bool CheckAttackTarget(FVector targetPosition);

    void UpdateYaw();
    void UpdateYaw(float theta);

    FVector& GetPrePosition() { return _prePosition; }
    float GetRangeX() { return _increaseX; }
    float GetRangeZ() { return _increaseZ; }

    int32 AddAttackCounter(int count = 1);
    int32 AddIdleCounter(int count = 1);
    int32 AddHitCounter(int count = 1);
    int32 AddMoveCounter(int count = 1);
    int32 AddDieCounter(int count = 1);

    void IdlePosition();

private:
    int32 _startX;
    int32 _startZ;
    int32 _targetCode;
    float _speed = 3.f;

    float _increaseX = 0;
    float _increaseZ = 0;

    // 이전 위치
    FVector _prePosition{0, 0, 0, 0};

    boost::random::uniform_int_distribution<> genYaw;

    GameUtils::TickCounter _YawCounter{4};
    GameUtils::TickCounter _MoveCounter{10};
    GameUtils::TickCounter _IdleCounter{3};
    GameUtils::TickCounter _HitCounter{3};
    GameUtils::TickCounter _DieCounter{30};
    GameUtils::TickCounter _AttackCounter{10};
};

class GamePlayerInfo : public GameObjectInfo
{
public:
    GamePlayerInfo(GameSessionRef gameSession, int32 uuid, int32 type, int32 hp);
    ~GamePlayerInfo();

    void Attack(GameMosterInfoRef target, vector<int32>& attackList);
    void Healing();
    bool AttackRect(FVector position, GameMosterInfoRef target);
    bool AttackCircle(FVector position, GameMosterInfoRef target);

    GameSessionRef GetGameSession() { return _gameSession.lock(); }

    void SetTarget(int32 uuid);
    int32 GetTarget() { return _targetCode; }

private:
    // 혹시나 해서 들고 있는다.
    boost::weak_ptr<GameSession> _gameSession;
    int32 _targetCode;

    // boost::json::value& _skillJson;
};
