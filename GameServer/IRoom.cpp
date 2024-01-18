#include "IRoom.h"

#include "GameMapInfo.h"
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
    _tickCounter.Add();
    // while (_isTask)
    // {
    //     _timer.expires_at(_timer.expiry() + _timerDelay);
    //     _tickCounter.Add();
    // }

    _timer.async_wait(boost::asio::bind_executor(_gameStrand, [this](boost::system::error_code error)
    {
        // 룸의 일정 시간마다 작업 (일단 10프레임 해봄)
        Task();
    }));
}

void GameRoom::Task()
{
    _isTask.exchange(true);
    MapType mapType = _gameMapInfo->GetMonsterMapInfo()->GetMapType();
    Rect& rect = _gameMapInfo->GetMonsterMapInfo()->GetRect();

    bool IsUpdate = false;
    if (_tickCounter.GetTick() == 0)
    {
        IsUpdate = true;
    }
    if (mapType == MapType::MONSTER)
    {
        for (auto& it : _monsterMap)
        {
            int32 uuid = it.first;
            GameMosterInfoRef info = it.second;
            if (IsUpdate)
            {
                info->UpdateYaw();
            }
            FVector pos = info->GetPosition();
            info->Move(GameUtils::MathUtils::GetCos(pos.X, pos.Yaw, 1),
                       GameUtils::MathUtils::GetSin(pos.Z, pos.Yaw, 1));


            // 생각 1
            // 미리 이동될거리를 보내고
            // 이동될 거리를 계산한다.
            // 
        }
    }
    _isTask.exchange(false);
    // 다시 Tick 등록
    Tick();

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

void GameRoom::CreateMapInfo(int32 type)
{
    // 일단 하드코딩으로 생성해둔다.
    if (type == 0)
    {
        // 일반 몹 맵
        _gameMapInfo = boost::make_shared<GameMapInfo>(25, 25, 0, 0);
        _gameMapInfo->CreateMonsterMapInfo(25, 15, 0, 0, MapType::MONSTER);
        _monsterCount = 10;
    }
    else if (type == 1)
    {
        // 보스 몹 맵
        _gameMapInfo = boost::make_shared<GameMapInfo>(15, 15, 0, 0);
        _gameMapInfo->CreateMonsterMapInfo(10, 10, 0, 0, MapType::BOS);
        _bosMonsterCount = 1;
    }
}

void GameRoom::InitMonsters()
{
    MapType mapType = _gameMapInfo->GetMonsterMapInfo()->GetMapType();
    Rect& rect = _gameMapInfo->GetMonsterMapInfo()->GetRect();
    boost::random::uniform_int_distribution<> genX(rect.StartX(), rect.EndX());
    boost::random::uniform_int_distribution<> genY(rect.StartY(), rect.EndX());
    boost::random::uniform_int_distribution<> genYaw(0, 360);

    if (mapType == MapType::MONSTER)
    {
        for (int32 i = 0; i < _monsterCount; i++)
        {
            GameMosterInfoRef info = boost::make_shared<GameMosterInfo>(i, 0, 100);
            info->SetStartPosition(genX(rng), genY(rng), genYaw(rng));
        }
    }
    else if (mapType == MapType::BOS)
    {
    }
}

void GameRoom::SpawnMonsters()
{
}

void GameRoom::MoveMonsters()
{
}

void GameRoom::AttackMonster()
{
}
