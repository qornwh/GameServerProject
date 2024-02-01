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
    // enum PlayerSkillType
    // {
    //     SWORD = 0,
    //     ARCHER = 1
    // };
    //
    // enum MonserSkillType
    // {
    //     KNIGHT = 0,
    //     UD_SPEARMAN = 1
    // };

    UnitSkill(int32 type);
    UnitSkill();

    void AddSkill(int32 skillCode, int32 type, bool target, int32 width, int32 height, int32 damage);
    void AddSkill(int32 skillCode, int32 type, bool target, int32 radius, int32 damage);
    void AddSkill(int32 skillCode, int32 type, bool target, int32 heal);
    void ClearSkill();

    unordered_map<int32, Skill>& GetSkillMap() { return _skillMap; }

private:
    int32 _type;
    unordered_map<int32, Skill> _skillMap;
};

class GameSkill
{
public:
    GameSkill();

    void AddPlayerSkill(int32 type);
    void AddMonsterSkill(int32 type);
    void ClearSkill();

    unordered_map<int32, UnitSkill>& GetPlayerSkill() { return _playerSkill; }
    unordered_map<int32, UnitSkill>& GetMonsterSkill() { return _monsterSkill; }

private:
    unordered_map<int32, UnitSkill> _playerSkill;
    unordered_map<int32, UnitSkill> _monsterSkill;
};
