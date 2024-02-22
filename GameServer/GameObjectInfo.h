#pragma once
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include "pch.h"
#include "GameUtils.h"
#include "Collider.h"
#include "GameService.pb.h"

static boost::random::mt19937_64 rng;

enum ObjectState
{
    IDLE = 0,
    DIE = 1,
    HITED = 2,
    ATTACK = 4,
    MOVE = 3,
    SKILL1 = 5,
    SKILL2 = 6,
    READY_ATTACK = 14,
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
    GameObjectInfo(GameRoomRef gameRoom, int32 uuid, int32 type, int32 hp);
    ~GameObjectInfo();

    virtual void Update() {}

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

    bool _hitted = false;
};

class GameMosterInfo : public GameObjectInfo
{
public:
    // 초기위치
    // 이동
    // 타겟 이동
    // 공격 (시간) - tict필요
    // 스킬 (시간) - tict필요
    GameMosterInfo(GameRoomRef gameRoom, int32 uuid, int32 type, int32 hp, int32 startX, int32 startZ);
    ~GameMosterInfo();

    void Update() override;

    void SetObjecteState(ObjectState state) override;

    void SetStartPosition(int32 x, int32 y);
    void GetStartPosition(int32& x, int32& y);

    void SetTarget(int32 uuid);
    int32 GetTarget() { return _targetCode; }

    virtual void Move();
    void updatePrePosition();
    virtual void MoveTarget(GamePlayerInfoRef target);
    bool CheckAttackTarget(GamePlayerInfoRef target);

    virtual void UpdateYaw();
    virtual void UpdateYaw(float theta);

    Vector2& GetPrePosition() { return _prePosition; }
    float GetRangeX() { return _increaseX; }
    float GetRangeZ() { return _increaseY; }

    virtual int32 AddAttackCounter(int count = 1);
    virtual int32 AddIdleCounter(int count = 1);
    virtual int32 AddHitCounter(int count = 1);
    virtual int32 AddMoveCounter(int count = 1);
    virtual int32 AddDieCounter(int count = 1);

    virtual void IdlePosition();

private:
    int32 _startX;
    int32 _startY;
    int32 _targetCode;
    float _speed = 3.f;

    float _increaseX = 0;
    float _increaseY = 0;
    float _preIncreaseValue = 0;

    // 이전 위치
    Vector2 _prePosition{0, 0};

    boost::random::uniform_int_distribution<> genYaw;

protected:
    GameUtils::TickCounter _YawCounter{4};
    GameUtils::TickCounter _MoveCounter{10};
    GameUtils::TickCounter _IdleCounter{3};
    GameUtils::TickCounter _HitCounter{3};
    GameUtils::TickCounter _DieCounter{30};
    GameUtils::TickCounter _AttackCounter{10};
    GameUtils::TickCounter _ReadyAttackCounter{10};
};

class GamePlayerInfo : public GameObjectInfo
{
public:
    GamePlayerInfo(GameSessionRef gameSession, int32 uuid, int32 type, int32 hp);
    ~GamePlayerInfo();

    void Update() override;

    void Attack(GameObjectInfoRef target, vector<int32>& attackList);
    void Healing();
    bool AttackRect(Vector2 position, GameObjectInfoRef target);
    bool AttackCircle(Vector2 position, GameObjectInfoRef target);

    void SetTarget(int32 uuid);
    int32 GetTarget() { return _targetCode; }

    GameSessionRef GetGameSession() { return _gameSession.lock(); }

    void SetAttacked(bool attack);

private:
    int32 _targetCode;
    
    // 혹시나 해서 들고 있는다.
    boost::weak_ptr<GameSession> _gameSession;

    //
    bool _attacked = false;
};
