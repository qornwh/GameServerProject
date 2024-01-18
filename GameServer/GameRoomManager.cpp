#include "GameRoomManager.h"
#include "IRoom.h"

GameRoomManager::GameRoomManager()
{
}

GameRoomManager::~GameRoomManager()
{
    cout << "~GameRoomManger" << endl;
}

void GameRoomManager::CreateRoom(boost::asio::io_context& io_context)
{
    // room생성
    int32 id = _id;
    
    if (_roomMap.find(id) == _roomMap.end())
    {
        GameRoomRef room = boost::make_shared<GameRoom>(io_context, id);
        _roomMap[_id] = room;
        _id.fetch_add(1);
    }
}

void GameRoomManager::CreateRoom(boost::asio::io_context& io_context, int32 id)
{
    // room생성
    if (_roomMap.find(id) == _roomMap.end())
    {
        GameRoomRef room = boost::make_shared<GameRoom>(io_context, id);
        _roomMap[id] = room;
    }
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
