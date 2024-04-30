#pragma once
#include "pch.h"

class GameRoomQuestInfo
{
public:
    GameRoomQuestInfo(int32 deadMonsterSize)
    {
        _deadMonsterSize = deadMonsterSize;
    }

    bool IsClear()
    {
        if (_deadMonsterSize <= _killMonsterCount)
            return true;
        return false;
    }

    void SetDeadMonster()
    {
        _killMonsterCount += _tempkillCount;
        _tempkillCount = 0;
    }

    void AddDeadMonster(int32 value = 1)
    {
        _tempkillCount += value;
    }

    bool IsKilled()
    {
        if (_tempkillCount > 0)
            return true;
        return false;
    }

    int32 GetKillCount()
    {
        return _killMonsterCount;
    }

    int32 GetSumCount()
    {
        return _deadMonsterSize;
    }

private:
    int32 _tempkillCount = 0;
    int32 _killMonsterCount = 0;
    int32 _deadMonsterSize = 0;
};

class GameRoomQuest : public std::enable_shared_from_this<GameRoomQuest>
{
public:
    GameRoomQuest(int32 killMonsterCount);
    GameRoomQuestInfo& GetInfo() { return _info; }

private:
    GameRoomQuestInfo _info;
};
