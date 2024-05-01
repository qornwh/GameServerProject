#pragma once
#include "Session.h"
#include "PacketHeader.h"

class GameSession : public Session
{
public:
    GameSession(boost::asio::io_context& io_context, const boost::asio::ip::tcp::endpoint& ep);

    void LoginHandler(BYTE* buffer, PacketHeader* header, int32 offset);
    void CreateCharacterHandler(BYTE* buffer, PacketHeader* header, int32 offset);
    void LoadHandler(BYTE* buffer, PacketHeader* header, int32 offset);
    void MoveHandler(BYTE* buffer, PacketHeader* header, int32 offset);
    void ChatHandler(BYTE* buffer, PacketHeader* header, int32 offset);
    void AttackHandler(BYTE* buffer, PacketHeader* header, int32 offset);
    void ChangeRoomHandler(BYTE* buffer, PacketHeader* header, int32 offset);
    void HandlePacket(BYTE* buffer, int32 offset, PacketHeader* header);
    int32 OnRecv(BYTE* buffer, int32 len) override;
    void CreatePlayerInfo(int32 type, int32 hp);
    std::shared_ptr<class GamePlayerInfo> GetPlayer();

    uint16 GetSessionId() { return _sessionId; }

    void SetRoomId(uint32 id) { _roomId = id; }
    int32 GetRoomId() { return _roomId; }

private:
    std::shared_ptr<class GamePlayerInfo> _player;
    int32 _roomId = -1;
    uint16 _sessionId = -1;
    int32 _accountCode = -1;
};
