#pragma once
#include "pch.h"

enum MapType
{
    COMMON = 0,
    MONSTER = 1,
    BOS = 2
};

struct Rect
{
    Rect(int32 x, int32 y) : X(x), Y(y)
    {
    }

    Rect(int32 x, int32 y, int32 centerX, int32 centerY) : X(x), Y(y), CenterX(centerX), CenterY(centerY)
    {
    }

    int32 X = 0;
    int32 Y = 0;
    int32 CenterX = 0;
    int32 CenterY = 0;

    int32 StartX()
    {
        return CenterX - X;
    }

    int32 StartY()
    {
        return CenterY - Y;
    }

    int32 EndX()
    {
        return CenterX + X;
    }

    int32 EndY()
    {
        return CenterY + Y;
    }
};

class MapInfo : public std::enable_shared_from_this<MapInfo>
{
public:
    MapInfo(MapType type);
    MapInfo(int32 x, int32 y, int32 centerX, int32 centerY, MapType type);
    ~MapInfo();

    bool InRect(float x, float y);
    bool InSetRect(float& x, float& y);

    Rect& GetRect();
    MapType GetMapType() { return _type; }

private:
    Rect _mapInfo{0, 0, 0, 0};
    MapType _type = COMMON;
};

class GameMapInfo : public std::enable_shared_from_this<GameMapInfo>
{
public:
    GameMapInfo(int32 x, int32 y, int32 centerX, int32 centerY);
    ~GameMapInfo();

    void CreateMonsterMapInfo(int32 x, int32 y, int32 centerX, int32 centerY, MapType type);

    MapInfo& GetMapInfo();
    MapInfoRef GetMonsterMapInfo();

    void SetMapCode(int32 mapCode);
    int32 GetMapCode() { return _mapCode; }

private:
    MapInfo _mapInfo;
    MapInfoRef _monsterMapInfo;
    int32 _mapCode = 0;
};
