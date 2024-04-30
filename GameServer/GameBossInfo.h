#pragma once
#include "GameObjectInfo.h"

class GameBossInfo : public GameMosterInfo
{
public:
    // 초기위치
    // 이동
    // 타겟 이동
    // 공격 (시간) - tict필요
    // 스킬 (시간) - tict필요
    GameBossInfo(GameRoomRef gameRoom, int32 uuid, int32 type, int32 hp, int32 startX, int32 startY);
    ~GameBossInfo();

    void SetObjecteState(ObjectState state) override;

    void Move() override;
    void MoveTarget(GamePlayerInfoRef target) override;

    void UpdateYaw() override;
    void UpdateYaw(float theta) override;

    int32 AddReadyAttackCounter(int count = 1);
    int32 AddDieCounter(int count = 1) override;

    // 특수 스킬들
    bool ReadyAttackSkill1(int hp);
    bool ReadyAttackSkill2(int hp);
    void Attack(GamePlayerInfoRef target, Vector<int32>& attackList);

    bool AttackRect(Vector2 position, GameObjectInfoRef target);
    bool AttackCircle(Vector2 position, GameObjectInfoRef target);

    void Update() override;

private:
    int32 _startX = 0;
    int32 _startY = 0;
    int32 _targetCode;
    float _speed = 3.f;

    float _increaseX = 0;
    float _increaseY = 0;

    // 이전 위치
    Vector2 _prePosition{0, 0};
    boost::random::uniform_int_distribution<> genYaw;
    Vector<std::function<bool(int)>> _SkillQueue;
};

class GameBossArrow : public GameObjectInfo
{
public:
    GameBossArrow(GameRoomRef gameRoom, int32 uuid, int32 type, int32 hp, int32 startX, int32 startZ);
    ~GameBossArrow();

    void Update() override;

private:
    Map<int32, int32> _players; // 플레이어코드, 상태(0: 안맞음, 1: 처음 충돌시작, 2: 충돌중)

    int32 skillDamage = 20; // 일단 고정 시킨다.
    
    float _speed = 5.f;
    int32 _startX = 0;
    int32 _startY = 0;
    
    float _increaseX = 0;
    float _increaseY = 0;
    Vector2 _prePosition{0, 0};
    
    GameUtils::TickCounter _MoveCounter{10};
};