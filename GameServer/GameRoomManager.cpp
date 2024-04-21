#include "GameRoomManager.h"
#include "IRoom.h"

GameRoomManager::GameRoomManager()
{
}

GameRoomManager::~GameRoomManager()
{
    cout << "~GameRoomManger" << endl;
}

GameRoomRef GameRoomManager::CreateRoom(boost::asio::io_context& io_context, int32 id)
{
    // room생성
    if (_roomMap.find(id) == _roomMap.end())
    {
        GameRoomRef room = boost::make_shared<GameRoom>(io_context, id);
        _roomMap[id] = room;
    }
    return _roomMap[id];
}

void GameRoomManager::EreaseRoom(int32 id)
{
    if (_roomMap[id])
        _roomMap.erase(id);
}

GameRoomRef GameRoomManager::getRoom(int32 id)
{
    if (_roomMap.find(id) != _roomMap.end())
        return _roomMap[id];
    else
        return nullptr;
}

void GameRoomManager::ClearRoom()
{
    _roomMap.clear();
}
