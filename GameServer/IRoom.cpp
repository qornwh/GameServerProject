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

void GameRoom::StartGameRoom()
{
    cout << "StartGameRoom !!!" << endl;
    // 처음 타이머를 시작시킨다.
    Tick();
}

void GameRoom::Tick()
{
    // 일단 스트랜드는 싱글처리해도 되기때문에 딜레이를 넣어둔다.
    // _gameStrand는 Tick 처리용도이다.
    // 먼저 딜레이후 일정 시간마다 _gameStrand에 Tick 함수를 계속 집어넣어서 시작한다.
    _timer.expires_at(_timer.expiry() + _timerDelay);

    _timer.async_wait(boost::asio::bind_executor(_gameStrand, [this](boost::system::error_code error)
    {
        // 룸의 일정 시간마다 작업 (일단 20프레임 해봄)
        Task();
        // 다시 Tick 등록
        Tick();
    }));
}

void GameRoom::Task()
{
    cout << "TASK : " << _id << " !!!" << endl;
    // 몬스터 이동.
    // -- 포지션, 방향 좌표 계산 필요
    // 몬스터 플레이어 충돌 및 타격
    // -- 몬스터 공격 판정 처리
    // -- 먼저 몬스터 공격 메시지 전달.

    // 순서
    // 케이스 1
    // 몬스터 일정 시간만다 이동 (방향전환도 함께)
    // 케이스 2
    // 플레이어의 공격명령(이때 현재 좌표도 함께 받음)을 받고 현재몬스터들의 좌표에 공격범위에 있으면 몬스터 Hit처리후 클라에게 메시지 전달
    // 케이스 3
    // 공격받은 몬스터들은 플레이어 쪽으로 이동 (방향전환 함께)
    // 케이스 4
    // 공격받은 몬스터들의 공격범위에 플레이어가 있으면 공격 및 데미지 계산 플레이어 Hit처리후 클라에게 공격 데미지 메시지 전달. 
}
