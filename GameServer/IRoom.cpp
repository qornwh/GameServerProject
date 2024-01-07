#include "IRoom.h"

#include "GamePacketHandler.h"
#include "GameService.pb.h"

void GameRoom::EnterSession(GameSessionRef session)
{
    IRoom<boost::shared_ptr<GameSession>, boost::shared_ptr<Session>>::EnterSession(session);
    session->SetRoomId(_id);
    
    protocol::SInsertplayer sendPkt;
    protocol::Player* player = new protocol::Player;
    player->set_name(session->GetPlayer()->GetName());
    player->set_code(session->GetPlayer()->GetCode());
    player->set_type(session->GetPlayer()->GetType());
    player->set_hp(session->GetPlayer()->GetHp());
    sendPkt.set_allocated_player(player);
            
    SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(sendPkt, protocol::MessageCode::S_INSERTPLAYER);
    BroadCastAnother(sendBuffer, session->GetPlayer()->GetCode());
}

void GameRoom::OutSession(boost::shared_ptr<GameSession> session)
{
    IRoom<boost::shared_ptr<GameSession>, boost::shared_ptr<Session>>::OutSession(session);
    session->SetRoomId(-1);

    protocol::SClosePlayer sendPkt;
    sendPkt.set_code(session->GetPlayer()->GetCode());

    SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(sendPkt, protocol::MessageCode::S_CLOSEPLAYER);
    BroadCastAnother(sendBuffer, session->GetPlayer()->GetCode());
}
