#pragma once
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include "GameRoomManager.h"
#include "GameUtils.h"
#include "pch.h"
#include "Collider.h"

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

class GameObjectInfo : public boost::enable_shared_from_this<GameObjectInfo>
{
public:
    GameObjectInfo(int32 uuid, int32 type, int32 hp);
    ~GameObjectInfo();

    void SetPosition(Vector2& position);
    void SetPosition(float x, float y);
    void SetRotate(float yaw);

    std::string& GetName() { return _name; }
    int32 GetCode() { return _uuid; }
    int32 GetType() { return _type; }
    int32 GetHp() { return _hp; }
    Vector2& GetPosition() { return _collider.GetPosition(); }
    float GetRotate() { return _collider.GetRotate(); }
    Collider& GetCollider() { return _collider; }

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
    int32 _maxHp;
    int32 _damage;
    int32 _heal;
    int32 _type;

    ObjectState _state = ObjectState::IDLE;

    boost::weak_ptr<GameRoom> _gameRoomRef;

    Collider _collider;
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

    void SetStartPosition(int32 x, int32 y);
    void GetStartPosition(int32& x, int32& y);

    void SetTarget(int32 uuid);
    int32 GetTarget() { return _targetCode; }

    void Move();
    void updatePrePosition();
    void MoveTarget(GamePlayerInfoRef target);
    bool CheckAttackTarget(GamePlayerInfoRef target);

    void UpdateYaw();
    void UpdateYaw(float theta);

    Vector2& GetPrePosition() { return _prePosition; }
    float GetRangeX() { return _increaseX; }
    float GetRangeZ() { return _increaseY; }

    int32 AddAttackCounter(int count = 1);
    int32 AddIdleCounter(int count = 1);
    int32 AddHitCounter(int count = 1);
    int32 AddMoveCounter(int count = 1);
    int32 AddDieCounter(int count = 1);

    void IdlePosition();

private:
    int32 _startX;
    int32 _startY;
    int32 _targetCode;
    float _speed = 3.f;

    float _increaseX = 0;
    float _increaseY = 0;

    // 이전 위치
    Vector2 _prePosition{0, 0};

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
    bool AttackRect(Vector2 position, GameMosterInfoRef target);
    bool AttackCircle(Vector2 position, GameMosterInfoRef target);

    GameSessionRef GetGameSession() { return _gameSession.lock(); }

    void SetTarget(int32 uuid);
    int32 GetTarget() { return _targetCode; }

private:
    // 혹시나 해서 들고 있는다.
    boost::weak_ptr<GameSession> _gameSession;
    int32 _targetCode;

    // boost::json::value& _skillJson;
};
