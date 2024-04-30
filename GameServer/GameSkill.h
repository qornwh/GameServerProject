#pragma once
#include "Types.h"

class Skill
{
public:
    enum SkillType
    {
        RECT = 0,
        CIRCLE = 1,
        HEAL = 2
    };

    Skill(int32 type, bool target, int32 width, int32 height, int32 damage);
    Skill(int32 type, bool target, int32 radius, int32 damage);
    Skill(int32 type, bool target, int32 heal);
    Skill();

    SkillType _type;
    bool _target;
    int32 _width;
    int32 _height;
    int32 _radius;
    int32 _heal;
    int32 _damage;
};

class UnitSkill
{
public:
    UnitSkill(int32 type);
    UnitSkill();

    void AddSkill(int32 skillCode, int32 type, bool target, int32 width, int32 height, int32 damage);
    void AddSkill(int32 skillCode, int32 type, bool target, int32 radius, int32 damage);
    void AddSkill(int32 skillCode, int32 type, bool target, int32 heal);
    void ClearSkill();

    Map<int32, Skill>& GetSkillMap() { return _skillMap; }

private:
    int32 _type;
    Map<int32, Skill> _skillMap;
};

class GameSkill : public std::enable_shared_from_this<GameSkill>
{
public:
    GameSkill();

    void AddPlayerSkill(int32 type);
    void AddMonsterSkill(int32 type);
    void ClearSkill();

    Map<int32, UnitSkill>& GetPlayerSkill() { return _playerSkill; }
    Map<int32, UnitSkill>& GetMonsterSkill() { return _monsterSkill; }

private:
    Map<int32, UnitSkill> _playerSkill;
    Map<int32, UnitSkill> _monsterSkill;
};
