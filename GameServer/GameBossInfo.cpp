#include "GameBossInfo.h"

#include "GameGlobal.h"
#include "GameSkill.h"

GameBossInfo::GameBossInfo(int32 uuid, int32 type, int32 hp, int32 startX, int32 startZ):
    GameObjectInfo(uuid, type, hp),
    _startX(startX), _startY(startZ), _targetCode(-1), genYaw(0, 360)
{
    // 보스는 공격을 3초에 한번씩 한다고 친다. ( 일정 범위에 있으면 맞음 )
    // 보스는 항상 슈퍼아머상태이다.
    // 보스는 70, 40인 경우 특수 공격을 한다. ( 큐에 함수를 넣는 방범을 생각해 본다 )
    // 보스는 이동 x, 방향 x
    // 보스가 공격, 특수 공격 진행시에는 데미지가 들어가지 않는다. ( 사실상 무조건 패턴 발동 )
}

GameBossInfo::~GameBossInfo()
{
}

void GameBossInfo::SetObjecteState(ObjectState state)
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
        // _HitCounter.ResetTic();
        // 보스는 슈퍼아머상태.
        break;
    case ObjectState::MOVE:
        _MoveCounter.ResetTic();
        break;
    case ObjectState::ATTACK:
        _AttackCounter.ResetTic();
        break;
    }
}

void GameBossInfo::SetStartPosition(int32 x, int32 y)
{
    SetPosition(x, y);
    UpdateYaw();
}

void GameBossInfo::GetStartPosition(int32& x, int32& y)
{
    x = _startX;
    y = _startY;
}

void GameBossInfo::SetTarget(int32 uuid)
{
    _targetCode = uuid;
}

void GameBossInfo::Move()
{
    // 이동 x
}

void GameBossInfo::updatePrePosition()
{
    _prePosition.X += _increaseX;
    _prePosition.Y += _increaseY;
}

void GameBossInfo::MoveTarget(GamePlayerInfoRef target)
{
    // 이동 x
}

bool GameBossInfo::CheckAttackTarget(GamePlayerInfoRef target)
{
    // 임시로 하드코딩 해둠 나중에 함수화 해야됨 !!!
    unordered_map<int32, Skill>& skillMap = GSkill->GetMonsterSkill()[GetType()].GetSkillMap();

    int32 skillType = skillMap[ObjectState::ATTACK]._type;
    bool isTargeting = skillMap[ObjectState::ATTACK]._target;

    if (isTargeting)
    {
        if (skillType == Skill::CIRCLE)
        {
            int32 radius = skillMap[ObjectState::ATTACK]._radius;

            float targetX = abs(GetPosition().X - target->GetPosition().X);
            float targetY = abs(GetPosition().Y - target->GetPosition().Y);

            float targetRadius = sqrtf(powf(targetX, 2) + powf(targetY, 2));

            if (targetRadius < radius)
            {
                // 공격 성공
                target->TakeDamage(skillMap[ObjectState::ATTACK]._damage);
                return true;
            }

            return false;
        }
    }
    return false;
}

void GameBossInfo::UpdateYaw()
{
    // room 1 tick 당 이동거리 계산
    SetRotate(genYaw(rng));
    _increaseX = GameEngine::MathUtils::GetCos(GetRotate()) * (_speed / _MoveCounter.GetTickValue());
    _increaseY = GameEngine::MathUtils::GetSin(GetRotate()) * (_speed / _MoveCounter.GetTickValue());
}

void GameBossInfo::UpdateYaw(float theta)
{
    SetRotate(theta);
    _increaseX = GameEngine::MathUtils::GetCos(GetRotate()) * (_speed / _MoveCounter.GetTickValue());
    _increaseY = GameEngine::MathUtils::GetSin(GetRotate()) * (_speed / _MoveCounter.GetTickValue());
}

int32 GameBossInfo::AddAttackCounter(int count)
{
    int32 value = _AttackCounter.Add(count);
    if (value == _AttackCounter.GetTickValue() - 1)
        SetObjecteState(ObjectState::MOVE);
    return value;
}

int32 GameBossInfo::AddIdleCounter(int count)
{
    int32 value = _IdleCounter.Add(count);
    if (value == _IdleCounter.GetTickValue() - 1)
        SetObjecteState(ObjectState::MOVE);
    return value;
}

int32 GameBossInfo::AddHitCounter(int count)
{
    int32 value = _HitCounter.Add(count);
    if (value == _HitCounter.GetTickValue() - 1)
        SetObjecteState(ObjectState::MOVE);
    return value;
}

int32 GameBossInfo::AddMoveCounter(int count)
{
    return _MoveCounter.Add(count);
}

int32 GameBossInfo::AddDieCounter(int count)
{
    int32 value = _DieCounter.Add(count);
    if (value == _DieCounter.GetTickValue() - 1)
    {
        SetPosition(_startX, _startY);
        _prePosition = _collider.GetPosition();
        _increaseX = 0;
        _increaseY = 0;
        _hp = 100;
        _targetCode = -1;
        SetObjecteState(ObjectState::IDLE);
    }
    return value;
}

void GameBossInfo::IdlePosition()
{
    SetPosition(_prePosition.X, _prePosition.Y);
    _increaseX = 0;
    _increaseY = 0;
}