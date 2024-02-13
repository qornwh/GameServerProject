#include "GameObjectInfo.h"

#include "GameGlobal.h"
#include "GameSession.h"
#include "GameSkill.h"
#include "IRoom.h"

GameObjectInfo::GameObjectInfo(int32 uuid, int32 type, int32 hp) : _uuid(uuid), _type(type), _hp(hp), _maxHp(hp),
                                                                   _collider(0.35f)
{
}

GameObjectInfo::~GameObjectInfo()
{
}

void GameObjectInfo::SetPosition(Vector2& position)
{
    _collider.SetPosition(position.X, position.Y);
}

void GameObjectInfo::SetPosition(float x, float y)
{
    _collider.SetPosition(x, y);
}

void GameObjectInfo::SetRotate(float yaw)
{
    _collider.SetRotate(yaw);
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

void GameObjectInfo::TakeHeal(int32 heal)
{
    _hp += heal;
    if (_hp > _maxHp)
        _hp = _maxHp;
    _heal += heal;
}

void GameObjectInfo::ResetDamage()
{
    _damage = 0;
}

void GameObjectInfo::ResetHeal()
{
    _heal = 0;
}

void GameObjectInfo::SetObjecteState(ObjectState state)
{
    _state = state;
}

GameMosterInfo::GameMosterInfo(int32 uuid, int32 type, int32 hp, int32 startX, int32 startZ):
    GameObjectInfo(uuid, type, hp),
    _startX(startX), _startY(startZ), _targetCode(-1), genYaw(0, 360)
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

void GameMosterInfo::SetStartPosition(int32 x, int32 y)
{
    SetPosition(x, y);
    UpdateYaw();
}

void GameMosterInfo::GetStartPosition(int32& x, int32& y)
{
    x = _startX;
    y = _startY;
}

void GameMosterInfo::SetTarget(int32 uuid)
{
    _targetCode = uuid;
}

void GameMosterInfo::Move()
{
    _prePosition = GetPosition();
    if (_YawCounter.Add() == 0)
    {
        UpdateYaw();
    }

    // 기본 거리 3
    SetPosition(GetPosition().X + (_increaseX * _speed), GetPosition().Y + (_increaseY * _speed));
}

void GameMosterInfo::updatePrePosition()
{
    _prePosition.X += _increaseX;
    _prePosition.Y += _increaseY;
}

void GameMosterInfo::MoveTarget(GamePlayerInfoRef target)
{
    float theta = Vector2::CalculateAngle(GetPosition(), target->GetPosition());
    UpdateYaw(theta);

    if (CheckAttackTarget(target))
    {
        // 여기서만 바로 공격
        SetObjecteState(ObjectState::ATTACK);
    }
    else
    {
        // 타겟지점 이동
        _prePosition = GetPosition();

        // 기본 거리 3
        SetPosition(GetPosition().X + (_increaseX * _speed), GetPosition().Y + (_increaseY * _speed));
    }
}

bool GameMosterInfo::CheckAttackTarget(GamePlayerInfoRef target)
{
    // 임시로 하드코딩 해둠 나중에 함수화 해야됨 !!!
    unordered_map<int32, Skill>& skillMap = GSkill->GetMonsterSkill()[GetType()].GetSkillMap();

    int32 skillType = skillMap[ObjectState::ATTACK]._type;
    bool isTargeting = skillMap[ObjectState::ATTACK]._target;

    if (isTargeting)
    {
        if (skillType == Skill::CIRCLE)
        {
            float radius = skillMap[ObjectState::ATTACK]._radius;

            Collider attackCollider(radius);
            attackCollider.SetPosition(GetPosition().X, GetPosition().Y);
            attackCollider.SetRotate(_collider.GetRotate());

            if (attackCollider.IsTrigger(target->GetCollider()))
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

void GameMosterInfo::UpdateYaw()
{
    // room 1 tick 당 이동거리 계산
    SetRotate(genYaw(rng));
    _increaseX = GameEngine::MathUtils::GetCos(GetRotate()) * (_speed / _MoveCounter.GetTickValue());
    _increaseY = GameEngine::MathUtils::GetSin(GetRotate()) * (_speed / _MoveCounter.GetTickValue());
}

void GameMosterInfo::UpdateYaw(float theta)
{
    SetRotate(theta);
    _increaseX = GameEngine::MathUtils::GetCos(GetRotate()) * (_speed / _MoveCounter.GetTickValue());
    _increaseY = GameEngine::MathUtils::GetSin(GetRotate()) * (_speed / _MoveCounter.GetTickValue());
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

void GameMosterInfo::IdlePosition()
{
    // SetPosition(_prePosition.X, _prePosition.Y);
    _increaseX = 0;
    _increaseY = 0;
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

    Skill& skill = GSkill->GetPlayerSkill()[GetType()].GetSkillMap()[GetObjectState()];
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
        GameRoomRef gameRoom = GRoomManger->getRoom(GetGameSession()->GetRoomId());
        for (auto& it : gameRoom->GetMonsterMap())
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

void GamePlayerInfo::Healing()
{
    Skill& skill = GSkill->GetPlayerSkill()[GetType()].GetSkillMap()[GetObjectState()];
    int32 skillType = skill._type;

    if (Skill::HEAL == skillType)
    {
        int32 heal = skill._heal;
        TakeHeal(heal);
    }
}

bool GamePlayerInfo::AttackRect(Vector2 position, GameMosterInfoRef target)
{
    float rangeX = GSkill->GetPlayerSkill()[GetType()].GetSkillMap()[GetObjectState()]._width;
    float rangeY = GSkill->GetPlayerSkill()[GetType()].GetSkillMap()[GetObjectState()]._height;

    Collider attackCollider(rangeX, rangeY);
    attackCollider.SetPosition(GetPosition().X + rangeY * GameEngine::MathUtils::GetCos(GetRotate()),
                               GetPosition().Y + rangeY / 2 * GameEngine::MathUtils::GetSin(GetRotate()));
    attackCollider.SetRotate(_collider.GetRotate());
    if (attackCollider.IsTrigger(target->GetCollider()))
        return true;
    return false;
}

bool GamePlayerInfo::AttackCircle(Vector2 position, GameMosterInfoRef target)
{
    float radius = GSkill->GetPlayerSkill()[GetType()].GetSkillMap()[GetObjectState()]._radius;

    Collider attackCollider(radius);
    attackCollider.SetPosition(GetPosition().X, GetPosition().Y);
    attackCollider.SetRotate(_collider.GetRotate());

    if (attackCollider.IsTrigger(target->GetCollider()))
        return true;

    return false;
}

void GamePlayerInfo::SetTarget(int32 uuid)
{
    _targetCode = uuid;
}
