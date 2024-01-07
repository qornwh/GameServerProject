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

class GamePlayer : public boost::enable_shared_from_this<GamePlayer>
{
public:
    GamePlayer(GameSessionRef gameSession, int32 sessionId, int32 type, int32 hp);
    ~GamePlayer();

public:
    void SetPosition(FVector& position);
    void SetPosition(int32 X, int32 Y, int32 Z);

    std::string& GetName() { return _name; }
    int32 GetCode() { return _uuid; }
    BYTE GetType() { return _type; }
    BYTE GetHp() { return _hp; }
    FVector& GetPosition() { return _position; }

    GameSessionRef GetGameSession() { return _gameSession.lock(); }
    void SetName(const string& name);

private:
    int _sessionId;

private:
    std::string _name;
    int32 _uuid;

    int32 _hp;
    int32 _type;

    FVector _position{0, 0, 0, 0};
    // uuid 필요

    // 혹시나 해서 들고 있는다.
    boost::weak_ptr<GameSession> _gameSession;
};
