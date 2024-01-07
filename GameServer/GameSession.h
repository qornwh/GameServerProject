#pragma once
#include "Session.h"
#include "PacketHeader.h"

class GameSession : public Session
{
public:
    GameSession(boost::asio::io_context& io_context, const boost::asio::ip::tcp::endpoint& ep);

    ~GameSession()
    {
    }

    void LoginHandler(const boost::asio::mutable_buffer& buffer, PacketHeader* header, int32 offset);
    void MoveHandler(const boost::asio::mutable_buffer& buffer, PacketHeader* header, int32 offset);
    void ChatHandler(const boost::asio::mutable_buffer& buffer, PacketHeader* header, int32 offset);
    void HandlePacket(const boost::asio::mutable_buffer& buffer, int32 offset, PacketHeader* header);
    int32 OnRecv(BYTE* buffer, int32 len) override;
    void CreatePlayerInfo(int32 type, int32 hp);
    boost::shared_ptr<class GamePlayer> GetPlayer();

    uint16 GetSessionId() { return _sessionId; }

    void SetRoomId(uint32 id) { _roomId = id; }
    int32 GetRoomId() { return _roomId; }

private:
    boost::shared_ptr<class GamePlayer> _player;
    int32 _roomId = -1;

    uint16 _sessionId = -1;
};
