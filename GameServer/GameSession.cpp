#include "GameSession.h"

#include "GameGlobal.h"
#include "GameService.pb.h"
#include "PacketHeader.h"
#include "GamePacketHandler.h"
#include "GameRoomManager.h"
#include "GameObjectInfo.h"
#include "IRoom.h"

GameSession::GameSession(boost::asio::io_context& io_context, const boost::asio::ip::tcp::endpoint& ep) : Session(
    io_context, ep)
{
    static Atomic<uint16> GameSessionId(0);
    GameSessionId.fetch_add(1);
    // 멀티스레드 상에서 이거 문제없는지 체크되야됨.. strand에서 하면 상관없긴한데...
    _sessionId = GameSessionId;
}

int32 GameSession::OnRecv(BYTE* buffer, int32 len)
{
    boost::asio::mutable_buffer _buffer = boost::asio::buffer(buffer, len);
    int32 offset = 0;

    while (true)
    {
        PacketHeader* header = reinterpret_cast<PacketHeader*>(&buffer[offset]);
        if (!GamePacketHandler::CheckPacketHeader(_buffer, header, offset, len))
            break;

        // 패킷 메시지
        HandlePacket(_buffer, offset, header);

        offset += header->size;
    }

    return offset;
}

void GameSession::CreatePlayerInfo(int32 type, int32 hp)
{
    _player = boost::make_shared<GamePlayerInfo>(reinterpret_pointer_cast<GameSession>(shared_from_this()), _sessionId,
                                             type, hp);
}

boost::shared_ptr<GamePlayerInfo> GameSession::GetPlayer()
{
    return _player;
}

void GameSession::HandlePacket(const boost::asio::mutable_buffer& buffer, int32 offset, PacketHeader* header)
{
    uint16 id = header->id;

    switch (id)
    {
    case protocol::MessageCode::LOGIN:
        {
            LoginHandler(buffer, header, offset + static_cast<int32>(sizeof(PacketHeader)));

            if (GRoomManger->getRoom(0) != nullptr)
            {
                GRoomManger->getRoom(0)->EnterSession(static_pointer_cast<GameSession>(shared_from_this()));
            }
        }
        break;
    case protocol::MessageCode::S_LOAD:
        break;
    case protocol::MessageCode::S_INSERTPLAYER:
        break;
    case protocol::MessageCode::S_MOVE:
        {
            MoveHandler(buffer, header, offset + static_cast<int32>(sizeof(PacketHeader)));
        }
        break;
    case protocol::MessageCode::S_CHAT:
        {
            ChatHandler(buffer, header, offset + static_cast<int32>(sizeof(PacketHeader)));
        }
        break;
    case protocol::MessageCode::S_PLAYERDATA:
        break;
    }
}

void GameSession::MoveHandler(const boost::asio::mutable_buffer& buffer, PacketHeader* header, int32 offset)
{
    protocol::SMove pkt;

    if (GamePacketHandler::ParsePacketHandler(pkt, buffer, header->size - offset, offset))
    {
        auto& position = pkt.position();
        // cout << position.x() << " : " << position.y() << " : " << position.z() << endl;

        GetPlayer()->SetPosition(position.x(), position.y(), position.z());
        pkt.set_code(_sessionId);
        protocol::Position* playerPosition = new protocol::Position;
        playerPosition->set_x(position.x());
        playerPosition->set_y(position.y());
        playerPosition->set_z(position.z());
        playerPosition->set_yew(position.yew());
        pkt.set_allocated_position(playerPosition);
        
        SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(pkt, protocol::MessageCode::S_MOVE);
        GetService()->BroadCast(sendBuffer);
        // AsyncWrite(sendBuffer);

        // 해당 함수는 보류다 바로바로 가야 되므로 strand처리를 제외한다.
        // if (GRoomManger->getRoom(_roomId) != nullptr)
        // {
        //     GRoomManger->getRoom(_roomId)->BroadCast(sendBuffer);
        // }
    }
}

void GameSession::LoginHandler(const boost::asio::mutable_buffer& buffer, PacketHeader* header, int32 offset)
{
    protocol::Login readPkt;
    if (GamePacketHandler::ParsePacketHandler(readPkt, buffer, header->size - offset, offset))
    {
        cout << readPkt.name() << endl;
        cout << readPkt.type() << endl;
        cout << readPkt.hp() << endl;

        CreatePlayerInfo(readPkt.type(), readPkt.hp());
        GetPlayer()->SetName(readPkt.name());

        // 나를 확인용 메시지 전달.
        if (GetService() != nullptr)
        {
            protocol::SPlayerData sendPkt;
            protocol::Player* player = new protocol::Player;
            player->set_name(_player->GetName());
            player->set_code(_player->GetCode());
            player->set_type(_player->GetType());
            player->set_hp(_player->GetHp());
            sendPkt.set_allocated_player(player);

            SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(
                sendPkt, protocol::MessageCode::S_PLAYERDATA);
            AsyncWrite(sendBuffer);
        }

        // 모든 유저 정보 전달. strand에 꽃자
        if (GetService() != nullptr)
        {
            protocol::SLoad sendPkt;
            for (auto& session : GetService()->GetSession())
            {
                GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

                if (gameSession->GetSessionId() == _sessionId)
                    continue;

                if (gameSession->GetPlayer() != nullptr)
                {
                    boost::shared_ptr<GamePlayerInfo> info = gameSession->GetPlayer();
                    // 설마 안지우지는 않겠지. pkt 소멸되면 메모리도 같이 날리겠지. 그냥 믿겠다!!!
                    protocol::Player* player = sendPkt.add_player();
                    player->set_name(info->GetName());
                    player->set_code(info->GetCode());
                    player->set_type(info->GetType());
                    player->set_hp(info->GetHp());
                    protocol::Position* position = new protocol::Position;
                    position->set_x(info->GetPosition().X);
                    position->set_y(info->GetPosition().Y);
                    position->set_z(info->GetPosition().Z);
                    position->set_yew(info->GetPosition().Yew);
                    // 메모리 할당이 아니라 스택메모리에 position 있어서 바로 보내야된다.
                    player->set_allocated_position(position);
                }
            }
            SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(sendPkt, protocol::MessageCode::S_LOAD);
            AsyncWrite(sendBuffer);
        }
    }
}

void GameSession::ChatHandler(const boost::asio::mutable_buffer& buffer, PacketHeader* header, int32 offset)
{
    protocol::SChat pkt;
    if (GamePacketHandler::ParsePacketHandler(pkt, buffer, header->size - offset, offset))
    {
        pkt.set_code(GetPlayer()->GetCode());
        SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(pkt, protocol::MessageCode::S_CHAT);
        // 해당 함수는 보류다 바로바로 가야 되므로 strand처리를 제외한다.
        if (GRoomManger->getRoom(_roomId) != nullptr)
        {
            GRoomManger->getRoom(_roomId)->BroadCast(sendBuffer);
        }
    }
}
