// #include "GameBossInfo.h"
//
// #include "GameGlobal.h"
// #include "GameSkill.h"
//
// GameBossInfo::GameBossInfo(int32 uuid, int32 type, int32 hp, int32 startX, int32 startZ):
//     GameObjectInfo(uuid, type, hp),
//     _startX(startX), _startZ(startZ), _targetCode(-1), genYaw(0, 360)
// {
// }
//
// GameBossInfo::~GameBossInfo()
// {
// }
//
// void GameBossInfo::SetObjecteState(ObjectState state)
// {
//     GameObjectInfo::SetObjecteState(state);
//
//     switch (_state)
//     {
//     case ObjectState::IDLE:
//         _IdleCounter.ResetTic();
//         break;
//     case ObjectState::DIE:
//         _DieCounter.ResetTic();
//         break;
//     case ObjectState::HITED:
//         _HitCounter.ResetTic();
//         break;
//     case ObjectState::MOVE:
//         _MoveCounter.ResetTic();
//         break;
//     case ObjectState::ATTACK:
//         _AttackCounter.ResetTic();
//         break;
//     }
// }
//
// void GameBossInfo::SetStartPosition(int32 x, int32 z)
// {
//     UpdateYaw();
//     _position.X = x;
//     _position.Z = z;
// }
//
// void GameBossInfo::GetStartPosition(int32& x, int32& z)
// {
//     x = _startX;
//     z = _startZ;
// }
//
// void GameBossInfo::SetTarget(int32 uuid)
// {
//     _targetCode = uuid;
// }
//
// void GameBossInfo::Move()
// {
//     _prePosition = _position;
//     if (_YawCounter.Add() == 0)
//     {
//         UpdateYaw();
//     }
//
//     // 기본 거리 3
//     _position.X += (_increaseX * _speed);
//     _position.Z += (_increaseZ * _speed);
// }
//
// void GameBossInfo::updatePrePosition()
// {
//     _prePosition.X += _increaseX;
//     _prePosition.Z += _increaseZ;
// }
//
// void GameBossInfo::MoveTarget(GamePlayerInfoRef target)
// {
//     float theta = GameUtils::MathUtils::CalculateAngle(_position.Z, _position.X, target->GetPosition().Z,
//                                                        target->GetPosition().X);
//     UpdateYaw(theta);
//
//     if (CheckAttackTarget(target))
//     {
//         // 여기서만 바로 공격
//         SetObjecteState(ObjectState::ATTACK);
//     }
//     else
//     {
//         // 타겟지점 이동
//         _prePosition = _position;
//
//         // 기본 거리 3
//         _position.X += (_increaseX * _speed);
//         _position.Z += (_increaseZ * _speed);
//     }
// }
//
// bool GameBossInfo::CheckAttackTarget(GamePlayerInfoRef target)
// {
//     // 임시로 하드코딩 해둠 나중에 함수화 해야됨 !!!
//     unordered_map<int32, Skill>& skillMap = GSkill->GetMonsterSkill()[GetType()].GetSkillMap();
//
//     int32 skillType = skillMap[ObjectState::ATTACK]._type;
//     bool isTargeting = skillMap[ObjectState::ATTACK]._target;
//
//     if (isTargeting)
//     {
//         if (skillType == Skill::CIRCLE)
//         {
//             int32 radius = skillMap[ObjectState::ATTACK]._radius;
//
//             float targetX = abs(_position.X - target->GetPosition().X);
//             float targetZ = abs(_position.Z - target->GetPosition().Z);
//
//             float targetRadius = sqrtf(powf(targetX, 2) + powf(targetZ, 2));
//
//             if (targetRadius < radius)
//             {
//                 // 공격 성공
//                 target->TakeDamage(skillMap[ObjectState::ATTACK]._damage);
//                 return true;
//             }
//
//             return false;
//         }
//     }
//     return false;
// }
//
// void GameBossInfo::UpdateYaw()
// {
//     // room 1 tick 당 이동거리 계산
//     _position.Yaw = genYaw(rng);
//     _increaseX = GameUtils::MathUtils::GetSin(_position.Yaw) * (_speed / _MoveCounter.GetTickValue());
//     _increaseZ = GameUtils::MathUtils::GetCos(_position.Yaw) * (_speed / _MoveCounter.GetTickValue());
// }
//
// void GameBossInfo::UpdateYaw(float theta)
// {
//     _position.Yaw = theta;
//     _increaseX = GameUtils::MathUtils::GetSin(_position.Yaw) * (_speed / _MoveCounter.GetTickValue());
//     _increaseZ = GameUtils::MathUtils::GetCos(_position.Yaw) * (_speed / _MoveCounter.GetTickValue());
// }
//
// int32 GameBossInfo::AddAttackCounter(int count)
// {
//     int32 value = _AttackCounter.Add(count);
//     if (value == _AttackCounter.GetTickValue() - 1)
//         SetObjecteState(ObjectState::MOVE);
//     return value;
// }
//
// int32 GameBossInfo::AddIdleCounter(int count)
// {
//     int32 value = _IdleCounter.Add(count);
//     if (value == _IdleCounter.GetTickValue() - 1)
//         SetObjecteState(ObjectState::MOVE);
//     return value;
// }
//
// int32 GameBossInfo::AddHitCounter(int count)
// {
//     int32 value = _HitCounter.Add(count);
//     if (value == _HitCounter.GetTickValue() - 1)
//         SetObjecteState(ObjectState::MOVE);
//     return value;
// }
//
// int32 GameBossInfo::AddMoveCounter(int count)
// {
//     return _MoveCounter.Add(count);
// }
//
// int32 GameBossInfo::AddDieCounter(int count)
// {
//     int32 value = _DieCounter.Add(count);
//     if (value == _DieCounter.GetTickValue() - 1)
//     {
//         _position.X = _startX;
//         _position.Z = _startZ;
//         _prePosition.X = _startX;
//         _prePosition.Z = _startZ;
//         _increaseX = 0;
//         _increaseZ = 0;
//         _hp = 100;
//         _targetCode = -1;
//         SetObjecteState(ObjectState::IDLE);
//     }
//     return value;
// }
//
// void GameBossInfo::IdlePosition()
// {
//     _position.X = _prePosition.X;
//     _position.Y = _prePosition.Y;
//     _position.Z = _prePosition.Z;
//     _increaseX = 0;
//     _increaseZ = 0;
// }