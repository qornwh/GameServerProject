#include "GamePlayer.h"

GamePlayer::GamePlayer(GameSessionRef gameSession, int32 sessionId, int32 type, int32 hp) : _sessionId(sessionId)
{
    _uuid = _sessionId;
    _hp = hp;
    _type = type;
    _gameSession = gameSession;
}

GamePlayer::~GamePlayer()
{
    cout << "close player info" << endl;
}

void GamePlayer::SetPosition(FVector& position)
{
    _position.X = position.X;
    _position.Y = position.Y;
    _position.Z = position.Z;
}

void GamePlayer::SetPosition(int32 X, int32 Y, int32 Z)
{
    _position.X = X;
    _position.Y = Y;
    _position.Z = Z;
}

void GamePlayer::SetName(const string& name)
{
    _name = name;
}
