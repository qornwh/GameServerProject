#include "GameBossInfo.h"

#include "GameGlobal.h"
#include "GameSession.h"
#include "GameSkill.h"
#include "IRoom.h"

GameBossInfo::GameBossInfo(GameRoomRef gameRoom, int32 uuid, int32 type, int32 hp, int32 startX, int32 startY):
    GameMosterInfo(gameRoom, uuid, type, hp, startX, startY),
    _targetCode(-1), genYaw(0, 360)
{
    // 보스는 공격을 3초에 한번씩 한다고 친다. ( 일정 범위에 있으면 맞음 )
    // 보스는 항상 슈퍼아머상태이다.
    // 보스는 70, 40인 경우 특수 공격을 한다. ( 큐에 함수를 넣는 방범을 생각해 본다 )
    // 보스는 이동 x, 방향 x
    // 보스가 공격, 특수 공격 진행시에는 데미지가 들어가지 않는다. ( 사실상 무조건 패턴 발동 )

    _SkillQueue.emplace_back([objRef = this](const int hp)
    {
        return objRef->ReadyAttackSkill1(hp);
    });

    _SkillQueue.emplace_back([objRef = this](const int hp)
    {
        return objRef->ReadyAttackSkill2(hp);
    });

    // 일단 이거 좀 예쁘게 만들자 !!!
    _AttackCounter = GameUtils::TickCounter(20);
    _collider.ResetCollider(0.9f);
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
    case ObjectState::READY_ATTACK:
        _ReadyAttackCounter.ResetTic();
        break;
    }
}

void GameBossInfo::Move()
{
    // 이동 x

    if (!_SkillQueue.empty())
    {
        auto& fn = _SkillQueue.back();
        if (fn(GetHp()))
        {
            _SkillQueue.pop_back();
        }
    }
    SetObjecteState(ObjectState::READY_ATTACK);
}

void GameBossInfo::MoveTarget(GamePlayerInfoRef target)
{
    // 이동 x
}

void GameBossInfo::UpdateYaw()
{
}

void GameBossInfo::UpdateYaw(float theta)
{
}

int32 GameBossInfo::AddReadyAttackCounter(int count)
{
    int32 value = _ReadyAttackCounter.Add(count);
    if (value == _ReadyAttackCounter.GetTickValue() - 1)
        SetObjecteState(ObjectState::ATTACK);
    return value;
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

bool GameBossInfo::ReadyAttackSkill1(int hp)
{
    cout << "ReadyAttackSkill1 !!!" << endl;
    if (hp <= 100)
    {
        SetObjecteState(ObjectState::SKILL1);
        return true;
    }
    return false;
}

bool GameBossInfo::ReadyAttackSkill2(int hp)
{
    cout << "ReadyAttackSkill2 !!!" << endl;
    if (hp <= 100)
    {
        SetObjecteState(ObjectState::SKILL2);
        return true;
    }
    return false;
}

void GameBossInfo::Attack(GamePlayerInfoRef target, vector<int32>& attackList)
{
    Skill& skill = GSkill->GetMonsterSkill()[GetType()].GetSkillMap()[GetObjectState()];
    int32 skillType = skill._type;

    if (target != nullptr)
    {
        // 타겟팅
        if (skillType == Skill::RECT)
        {
            if (AttackRect(target->GetPosition(), target))
            {
                attackList.push_back(target->GetCode());
                target->TakeDamage(skill._damage);
            }
        }
        else if (skillType == Skill::CIRCLE)
        {
            if (AttackCircle(target->GetPosition(), target))
            {
                attackList.push_back(target->GetCode());
                target->TakeDamage(skill._damage);
            }
        }
    }
    else
    {
        // 논타겟
        for (auto& it : GetGameRoom()->GetPlayerMap())
        {
            if (skillType == Skill::RECT)
            {
                if (AttackRect(it.second->GetPosition(), it.second))
                {
                    attackList.push_back(it.second->GetCode());
                    it.second->TakeDamage(skill._damage);
                }
            }
            else if (skillType == Skill::CIRCLE)
            {
                if (AttackCircle(it.second->GetPosition(), it.second))
                {
                    attackList.push_back(it.second->GetCode());
                    it.second->TakeDamage(skill._damage);
                }
            }
        }
    }
}

bool GameBossInfo::AttackRect(Vector2 position, GameObjectInfoRef target)
{
    float rangeX = GSkill->GetMonsterSkill()[GetType()].GetSkillMap()[GetObjectState()]._width;
    float rangeY = GSkill->GetMonsterSkill()[GetType()].GetSkillMap()[GetObjectState()]._height;

    Collider attackCollider(rangeX, rangeY);
    attackCollider.SetPosition(GetPosition().X, GetPosition().Y);
    attackCollider.SetRotate(_collider.GetRotate());
    if (attackCollider.IsTrigger(target->GetCollider()))
        return true;
    return false;
}

bool GameBossInfo::AttackCircle(Vector2 position, GameObjectInfoRef target)
{
    float radius = GSkill->GetMonsterSkill()[GetType()].GetSkillMap()[GetObjectState()]._radius;

    Collider attackCollider(radius);
    attackCollider.SetPosition(GetPosition().X, GetPosition().Y);
    attackCollider.SetRotate(_collider.GetRotate());

    if (attackCollider.IsTrigger(target->GetCollider()))
        return true;

    return false;
}
