#pragma once
#include "GameObjectInfo.h"

class GameBossInfo : public GameObjectInfo
{
public:
    // 초기위치
    // 이동
    // 타겟 이동
    // 공격 (시간) - tict필요
    // 스킬 (시간) - tict필요
    GameBossInfo(int32 uuid, int32 type, int32 hp, int32 startX, int32 startZ);
    ~GameBossInfo();

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
    GameUtils::TickCounter _AttackCounter{30};
};
