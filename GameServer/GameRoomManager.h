#pragma once
#include "pch.h"

using GameRoomRef = boost::shared_ptr<class GameRoom>;

class GameRoomManager : public enable_shared_from_this<GameRoomManager>
{
public:
    
    enum RoomType
    {
        space, // 지역
    };

    GameRoomManager();
    ~GameRoomManager();
    void CreateRoom(boost::asio::io_context& io_context);
    void CreateRoom(boost::asio::io_context& io_context, int32 id);
    void EreaseRoom(int32 id);
    GameRoomRef getRoom(int32 id);
    void ClearRoom();

private:
    Atomic<int32> _id{0};
    unordered_map<int32, GameRoomRef> _roomMap;
};

extern shared_ptr<GameRoomManager> GBSRoomManger;
