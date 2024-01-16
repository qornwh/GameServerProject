#pragma once
#include "pch.h"

struct FVector
{
    FVector(int32 X, int32 Y, int32 Z, float Yew) : X(X), Y(Y), Z(Z), Yew(Yew)
    {
    };

    int32 X;
    int32 Y;
    int32 Z;
    float Yew;
};

class GameObject
{
    
};

class GameMonster
{
    
};

class GameObjectInfo : public boost::enable_shared_from_this<GameObjectInfo>
{
    
public:
    GameObjectInfo(int32 uuid, int32 type, int32 hp);
    ~GameObjectInfo();

    void SetPosition(FVector& position);
    void SetPosition(int32 X, int32 Y, int32 Z);

    std::string& GetName() { return _name; }
    int32 GetCode() { return _uuid; }
    BYTE GetType() { return _type; }
    BYTE GetHp() { return _hp; }
    FVector& GetPosition() { return _position; }

    void SetName(const string& name);

private:
    std::string _name;
    int32 _uuid;

    int32 _hp;
    int32 _type;

    FVector _position{0, 0, 0, 0};
};

class GamePlayerInfo : public GameObjectInfo
{
public:
    GamePlayerInfo(GameSessionRef gameSession, int32 uuid, int32 type, int32 hp);
    ~GamePlayerInfo();

    GameSessionRef GetGameSession() { return _gameSession.lock(); }

private:

    // 혹시나 해서 들고 있는다.
    boost::weak_ptr<GameSession> _gameSession;
};
