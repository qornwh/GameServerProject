#include "GameSkill.h"

Skill::Skill(int32 type, bool target, int32 width, int32 height, int32 damage) : _type(static_cast<SkillType>(type)),
    _target(target),
    _width(width),
    _height(height), _radius(-1), _heal(-1), _damage(damage)
{
}

Skill::Skill(int32 type, bool target, int32 radius, int32 damage) : _type(static_cast<SkillType>(type)),
                                                                    _target(target), _width(-1),
                                                                    _height(-1), _heal(-1),
                                                                    _radius(radius), _damage(damage)
{
}

Skill::Skill(int32 type, bool target, int32 heal) : _type(static_cast<SkillType>(type)), _target(target), _heal(heal),
                                                    _width(-1),
                                                    _height(-1), _radius(-1)
{
}

Skill::Skill() : _type(static_cast<SkillType>(0)), _target(false), _width(-1), _height(-1), _radius(-1), _heal(-1),
                 _damage(-1)
{
}

UnitSkill::UnitSkill(int32 type) : _type(type)
{
}

UnitSkill::UnitSkill() : _type(-1)
{
}

void UnitSkill::AddSkill(int32 skillCode, int32 type, bool target, int32 width, int32 height, int32 damage)
{
    _skillMap.emplace(skillCode, Skill(type, target, width, height, damage));
}

void UnitSkill::AddSkill(int32 skillCode, int32 type, bool target, int32 radius, int32 damage)
{
    _skillMap.emplace(skillCode, Skill(type, target, radius, damage));
}

void UnitSkill::AddSkill(int32 skillCode, int32 type, bool target, int32 heal)
{
    _skillMap.emplace(skillCode, Skill(type, target, heal));
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
