#include "GameRoomManager.h"
#include "IRoom.h"

GameRoomManager::GameRoomManager()
{
}

GameRoomManager::~GameRoomManager()
{
    cout << "~GameRoomManger" << endl;
}

void GameRoomManager::CreateRoom(GameServiceRef gameService)
{
    // room생성
    uint32 id = _id;
    GameRoomRef room = boost::make_shared<GameRoom>(gameService, id);
    _roomMap[_id] = room;
    _id.fetch_add(1);
}

void GameRoomManager::EreaseRoom(int32 id)
{
    if (_roomMap[id])
        _roomMap.erase(id);
}

GameRoomRef GameRoomManager::getRoom(int32 id)
{
    if (_roomMap.find(0) != _roomMap.end())
        return _roomMap[id];
    else
        return nullptr;
}

void GameRoomManager::ClearRoom()
{
    _roomMap.clear();
}
