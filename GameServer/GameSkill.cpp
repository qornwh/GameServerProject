#include "GameSkill.h"

Skill::Skill(int32 type, bool target, int32 width, int32 heigth) : _type(static_cast<SkillType>(type)), _target(target),
                                                                   _width(width),
                                                                   _height(heigth), _radius(-1)
{
}

Skill::Skill(int32 type, bool target, int32 radius) : _type(static_cast<SkillType>(type)), _target(target), _width(-1),
                                                      _height(-1),
                                                      _radius(radius)
{
}

Skill::Skill() : _type(static_cast<SkillType>(0)), _target(false), _width(-1), _height(-1), _radius(-1)
{
    cout << "Skill !!!!" << endl;
}

UnitSkill::UnitSkill(int32 type) : _type(type)
{
}

UnitSkill::UnitSkill() : _type(-1)
{
    cout << "UnitSkill !!!!" << endl;
}

void UnitSkill::AddSkill(int32 skillCode, int32 type, bool target, int32 width, int32 height)
{
    _skillMap.emplace(skillCode, Skill(type, target, width, height));
}

void UnitSkill::AddSkill(int32 skillCode, int32 type, bool target, int32 radius)
{
    _skillMap.emplace(skillCode, Skill(type, target, radius));
}

void UnitSkill::ClearSkill()
{
    _skillMap.clear();
}

GameSkill::GameSkill()
{
}

void GameSkill::AddPlayerSkill(int32 type)
{
    _playerSkill.emplace(type, UnitSkill(type));
}

void GameSkill::AddMonsterSkill(int32 type)
{
    _monsterSkill.emplace(type, UnitSkill(type));
}

void GameSkill::ClearSkill()
{
    for (auto& player : _playerSkill)
    {
        player.second.ClearSkill();
    }

    for (auto& monster : _monsterSkill)
    {
        monster.second.ClearSkill();
    }

    _playerSkill.clear();
    _monsterSkill.clear();
}
