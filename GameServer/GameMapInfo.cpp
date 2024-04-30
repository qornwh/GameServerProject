#include "GameMapInfo.h"

MapInfo::MapInfo(MapType type)
{
}

MapInfo::MapInfo(int32 x, int32 y, int32 centerX, int32 centerY, MapType type) : _mapInfo(x, y, centerX, centerY),
    _type(type)
{
}

MapInfo::~MapInfo()
{
}

bool MapInfo::InRect(float x, float y)
{
    if (_mapInfo.StartX() < x && _mapInfo.EndX() > x && _mapInfo.StartY() < y && _mapInfo.EndY() > y)
        return true;

    return false;
}

bool MapInfo::InSetRect(float& x, float& y)
{
    bool flag = false;
    // 몬스터 영역 튀어나오는 값이면 제일 끝값으로 교체한다.
    if (_mapInfo.StartX() > x)
    {
        x = _mapInfo.StartX();
        flag = true;
    }
    else if (_mapInfo.EndX() < x)
    {
        x = _mapInfo.EndX();
        flag = true;
    }

    if (_mapInfo.StartY() > y)
    {
        y = _mapInfo.StartY();
        flag = true;
    }
    else if (_mapInfo.EndY() < y)
    {
        y = _mapInfo.EndY();
        flag = true;
    }

    return flag;
}

Rect& MapInfo::GetRect()
{
    return _mapInfo;
}

GameMapInfo::GameMapInfo(int32 x, int32 y, int32 centerX, int32 centerY) : _mapInfo(
    x, y, centerX, centerY, MapType::COMMON)
{
}

GameMapInfo::~GameMapInfo()
{
}

void GameMapInfo::CreateMonsterMapInfo(int32 x, int32 y, int32 centerX, int32 centerY, MapType type)
{
    _monsterMapInfo = std::make_shared<MapInfo>(x, y, centerX, centerY, type);
}

MapInfo& GameMapInfo::GetMapInfo()
{
    return _mapInfo;
}

MapInfoRef GameMapInfo::GetMonsterMapInfo()
{
    return _monsterMapInfo;
}

void GameMapInfo::SetMapCode(int32 mapCode)
{
    _mapCode = mapCode;
}
