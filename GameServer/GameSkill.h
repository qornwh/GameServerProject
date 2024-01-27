#pragma once
#include "Types.h"

class Skill
{
public:
    enum SkillType
    {
        RECT = 0,
        CIRCLE = 1
    };

    Skill(int32 type, bool target, int32 width, int32 heigth);
    Skill(int32 type, bool target, int32 radius);
    Skill();

    SkillType _type;
    bool _target;
    int32 _width;
    int32 _height;
    int32 _radius;
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

    void AddSkill(int32 skillCode, int32 type, bool target, int32 width, int32 height);
    void AddSkill(int32 skillCode, int32 type, bool target, int32 radius);
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
