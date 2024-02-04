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
    case protocol::MessageCode::C_PLAYERATTACK:
        {
            AttackHandler(buffer, header, offset + static_cast<int32>(sizeof(PacketHeader)));
        }
        break;
    case protocol::MessageCode::C_MOVEPOTAL:
        {
            ChangeRoomHandler(buffer, header, offset + static_cast<int32>(sizeof(PacketHeader)));
        }
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

        GetPlayer()->SetPosition(position.x(), position.y(), position.z(), position.yaw());
        pkt.set_code(_sessionId);
        protocol::Position* playerPosition = new protocol::Position;
        playerPosition->set_x(position.x());
        playerPosition->set_y(position.y());
        playerPosition->set_z(position.z());
        playerPosition->set_yaw(position.yaw());
        pkt.set_allocated_position(playerPosition);
        pkt.set_is_monster(false);

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
        CreatePlayerInfo(readPkt.type(), 1000);
        GetPlayer()->SetName(readPkt.name());

        // 나를 확인용 메시지 전달.
        if (GetService() != nullptr)
        {
            protocol::SPlayerData sendPkt;
            protocol::Player* player = new protocol::Player;
            protocol::Unit* unit = new protocol::Unit;
            unit->set_name(_player->GetName());
            unit->set_code(_player->GetCode());
            unit->set_type(_player->GetType());
            unit->set_hp(_player->GetHp());
            player->set_allocated_unit(unit);
            sendPkt.set_allocated_player(player);

            SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(
                sendPkt, protocol::MessageCode::S_PLAYERDATA);
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

void GameSession::AttackHandler(const boost::asio::mutable_buffer& buffer, PacketHeader* header, int32 offset)
{
    protocol::CPlayerAttack pkt;
    if (GRoomManger->getRoom(GetRoomId()) != nullptr)
    {
        if (GamePacketHandler::ParsePacketHandler(pkt, buffer, header->size - offset, offset))
        {
            int32 SkillCode = pkt.skill_code();
            int32 TargetCode = pkt.target_code();
            cout << "targetCode : " << TargetCode << " skillCode : " << SkillCode << endl;
            
            auto& position = pkt.position();
            GetPlayer()->SetPosition(position.x(), position.y(), position.z(), position.yaw());
            GetPlayer()->SetObjecteState(static_cast<ObjectState>(SkillCode));
            GetPlayer()->SetTarget(TargetCode);
            
            if (ObjectState::SKILL2 == SkillCode)
            {
                // 힐, 버프 등.
                GRoomManger->getRoom(GetRoomId())->BuffSession(static_pointer_cast<GameSession>(shared_from_this()));
            }
            else
            {
                // 공격
                GRoomManger->getRoom(GetRoomId())->AttackSession(static_pointer_cast<GameSession>(shared_from_this()));

                protocol::SUnitAttack sendPkt;
                protocol::Attack* attackPkt = sendPkt.add_attack();
                attackPkt->set_code(GetPlayer()->GetCode());
                attackPkt->set_is_monster(false);
                attackPkt->set_skill_code(SkillCode);

                SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(sendPkt, protocol::MessageCode::S_UNITATTACK);
                GRoomManger->getRoom(GetRoomId())->BroadCastAnother(sendBuffer, GetPlayer()->GetCode());
            }

        }
    }
}

void GameSession::ChangeRoomHandler(const boost::asio::mutable_buffer& buffer, PacketHeader* header, int32 offset)
{
    protocol::CMovePotal readPkt;

    int32 currentRoomId = readPkt.pre_room_id();
    int32 nextRoomId = readPkt.next_room_id();

    if (GRoomManger->getRoom(GetRoomId()) != nullptr)
    {
        GRoomManger->getRoom(GetRoomId())->OutSession(static_pointer_cast<GameSession>(shared_from_this()));
    }

    if (GRoomManger->getRoom(nextRoomId) != nullptr)
    {
        GRoomManger->getRoom(nextRoomId)->EnterSession(static_pointer_cast<GameSession>(shared_from_this()));
    }
}
