#include "IRoom.h"
#include "GameBossInfo.h"
#include "GameMapInfo.h"
#include "GamePacketHandler.h"
#include "GameRoomQuest.h"
#include "GameService.pb.h"

void GameRoom::EnterSession(GameSessionRef session)
{
    _playerMap.emplace(session->GetPlayer()->GetCode(), session->GetPlayer());
    session->SetRoomId(_id);
    {
        protocol::SInsertplayer sendPkt;
        protocol::Player* player = new protocol::Player;
        protocol::Unit* unit = new protocol::Unit;
        unit->set_name(session->GetPlayer()->GetName());
        unit->set_code(session->GetPlayer()->GetCode());
        unit->set_type(session->GetPlayer()->GetType());
        unit->set_hp(session->GetPlayer()->GetHp());
 
        protocol::Position* position = new protocol::Position;
        position->set_x(session->GetPlayer()->GetPosition().Y);
        position->set_y(0);
        position->set_z(session->GetPlayer()->GetPosition().X);
        position->set_yaw(session->GetPlayer()->GetRotate());
        unit->set_allocated_position(position);
        player->set_allocated_unit(unit);
        sendPkt.set_allocated_player(player);

        SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(sendPkt, protocol::MessageCode::S_INSERTPLAYER);
        BroadCastAnother(sendBuffer, session->GetPlayer()->GetCode());
    }

    {
        protocol::SLoad sendPkt;
        sendPkt.set_room_id(_id);
        for (auto it : _sessionList)
        {
            if (it != nullptr)
            {
                GameSessionRef gameSession = std::static_pointer_cast<GameSession>(it);

                if (gameSession->GetSessionId() == session->GetSessionId())
                    continue;

                if (gameSession->GetPlayer() != nullptr)
                {
                    std::shared_ptr<GamePlayerInfo> info = gameSession->GetPlayer();
                    protocol::Player* player = sendPkt.add_player();
                    protocol::Unit* unit = new protocol::Unit;
                    unit->set_name(info->GetName());
                    unit->set_code(info->GetCode());
                    unit->set_type(info->GetType());
                    unit->set_hp(info->GetHp());
                    protocol::Position* position = new protocol::Position;
                    position->set_x(info->GetPosition().Y);
                    position->set_y(0);
                    position->set_z(info->GetPosition().X);
                    position->set_yaw(info->GetRotate());
                    unit->set_allocated_position(position);
                    player->set_allocated_unit(unit);
                }
            }
        }

        for (auto& it : _monsterMap)
        {
            int32 uuid = it.first;
            GameMosterInfoRef info = it.second;
            protocol::Monster* monster = sendPkt.add_monster();
            protocol::Unit* unit = new protocol::Unit;
            unit->set_name(info->GetName());
            unit->set_code(info->GetCode());
            unit->set_type(info->GetType());
            unit->set_hp(info->GetHp());
            protocol::Position* position = new protocol::Position;
            position->set_x(info->GetPosition().Y);
            position->set_y(0);
            position->set_z(info->GetPosition().X);
            position->set_yaw(info->GetRotate());
            unit->set_allocated_position(position);
            monster->set_state(info->GetObjectState());
            monster->set_allocated_unit(unit);
        }

        SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(sendPkt, protocol::MessageCode::S_LOAD);
        session->AsyncWrite(sendBuffer);
    }
    IRoom<std::shared_ptr<GameSession>, std::shared_ptr<Session>>::EnterSession(session);
}

void GameRoom::OutSession(GameSessionRef session)
{
    // playerInfo 레퍼런스카운트 미리 제거해둔다.
    _playerMap.erase(session->GetPlayer()->GetCode());
    session->SetRoomId(-1);

    protocol::SClosePlayer sendPkt;
    sendPkt.set_code(session->GetPlayer()->GetCode());

    SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(sendPkt, protocol::MessageCode::S_CLOSEPLAYER);
    BroadCastAnother(sendBuffer, session->GetPlayer()->GetCode());
    IRoom<std::shared_ptr<GameSession>, std::shared_ptr<Session>>::OutSession(session);
}

void GameRoom::AttackSession(GameSessionRef session)
{
    boost::asio::post(boost::asio::bind_executor(_gameStrand, [this, session]
    {
        attackQueue.push(session->GetPlayer());
    }));
}

void GameRoom::BuffSession(GameSessionRef session)
{
    GamePlayerInfoRef info = session->GetPlayer();

    info->Healing();

    protocol::SUnitBuff sendPkt;
    protocol::Buff* buff = sendPkt.add_buff();
    buff->set_code(info->GetCode());
    buff->set_heal(info->GetHeal());
    buff->set_hp(info->GetHp());
    buff->set_skill_code(info->GetObjectState());
    buff->set_is_monster(false);
    info->ResetHeal();
    SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(sendPkt, protocol::MessageCode::S_UNITBUFF);
    BroadCast(sendBuffer);
}

void GameRoom::StartGameRoom()
{
    std::cout << "StartGameRoom !!!" << std::endl;
    Tick();
}

void GameRoom::Tick()
{
    // _gameStrand는 Tick 처리용도이다.
    // 일단 스트랜드는 싱글처리해도 되기때문에 딜레이를 넣어둔다.
    // 먼저 딜레이후 일정 시간마다 _gameStrand에 Tick 함수를 계속 집어넣어서 시작한다.
    _timer.expires_at(_timer.expiry() + _timerDelay);
    _tickCounter.Add();
    _timer.async_wait(boost::asio::bind_executor(_gameStrand, [this](boost::system::error_code error)
    {
        Task();

        // GameRoomQuestInfo& rqInfo = _gameRoomQuest->GetInfo();
        // if (rqInfo.IsKilled())
        // {
        //     rqInfo.SetDeadMonster();
        //
        //     protocol::SRoomQuest pkt;
        //     pkt.set_is_clear(rqInfo.IsClear());
        //     pkt.set_kill_count(rqInfo.GetKillCount());
        //     pkt.set_sum_kill(rqInfo.GetSumCount());
        //     SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(pkt, protocol::MessageCode::S_ROOMQUEST);
        //     BroadCast(sendBuffer);
        //     
        //     MapType mapType = _gameMapInfo->GetMonsterMapInfo()->GetMapType();
        //     if (mapType == MapType::BOS)
        //     {
        //         for (auto session : _sessionList)
        //         {
        //             OutSession(session);
        //         }
        //     }
        // }
    }));
}

void GameRoom::Task()
{
    _isTask.exchange(true);
    const MapType mapType = _gameMapInfo->GetMonsterMapInfo()->GetMapType();
    const MapInfoRef monsterMap = _gameMapInfo->GetMonsterMapInfo();

    while (!attackQueue.empty())
    {
        GamePlayerInfoRef attackInfo = attackQueue.front();
        attackInfo->SetAttacked(true);
        attackQueue.pop();
    }
    
    for (auto& it : _playerMap)
    {
        GamePlayerInfoRef info = it.second;
        info->Update();
    }

    for (auto& it : _monsterMap)
    {
        GameMosterInfoRef info = it.second;
        info->Update();
    }

    if (_unitPkt.unit_state_size() > 0)
    {
        SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(_unitPkt, protocol::MessageCode::S_UNITSTATES);
        BroadCast(sendBuffer);
        _unitPkt.clear_unit_state();
    }
    _isTask.exchange(false);
    Tick();

    // 몬스터 이동.
    // -- 포지션, 방향 좌표 계산 필요
    // 몬스터 플레이어 충돌 및 타격
    // -- 몬스터 공격 판정 처리
    // -- 먼저 몬스터 공격 메시지 전달.

    // 순서
    // 케이스 1
    // 몬스터 일정 시간만다 이동 (방향전환도 함께)
    // - 일단 완
    // 케이스 2
    // 플레이어의 공격명령(이때 현재 좌표도 함께 받음)을 받고 현재몬스터들의 좌표에 공격범위에 있으면 몬스터 Hit처리후 클라에게 메시지 전달
    // - 완
    // 케이스 3
    // 공격받은 몬스터들은 플레이어 쪽으로 이동 (방향전환 함께)
    // - 완
    // 케이스 4
    // 공격받은 몬스터들의 공격범위에 플레이어가 있으면 공격 및 데미지 계산 플레이어 Hit처리후 클라에게 공격 데미지 메시지 전달.
    // - 완

    // 보스 정책 
    // 보스는 공격을 3초에 한번씩 한다고 친다. ( 일정 범위에 있으면 )
    // 보스는 항상 슈퍼아머상태이다.
    // 보스는 70, 40인 경우 특수 공격을 한다. ( 큐에 함수를 넣는 방범을 생각해 본다 )
    // 보스는 이동 x, 방향 x
    // 보스가 공격, 특수 공격 진행시에는 데미지가 들어가지 않는다. ( 사실상 무조건 패턴 발동 )
}

GameMapInfoRef GameRoom::CreateMapInfo(int32 type)
{
    // 일단 하드코딩으로 생성해둔다.
    if (type == 0)
    {
        // 일반 몹 맵
        _gameMapInfo = std::make_shared<GameMapInfo>(0, 0, 0, 0);
        _gameMapInfo->CreateMonsterMapInfo(0, 0, 0, 0, MapType::MONSTER);
        _gameRoomQuest = std::make_shared<GameRoomQuest>(5);
        _monsterCount = 10;
    }
    else if (type == 1)
    {
        // 보스 몹 맵
        _gameMapInfo = std::make_shared<GameMapInfo>(0, 0, 0, 0);
        _gameMapInfo->CreateMonsterMapInfo(0, 0, 0, 0, MapType::BOS);
        _gameRoomQuest = std::make_shared<GameRoomQuest>(1);
        _bosMonsterCount = 1;
    }

    return _gameMapInfo;
}

void GameRoom::InitMonsters()
{
    StartGameRoom();
    MapType mapType = _gameMapInfo->GetMonsterMapInfo()->GetMapType();
    Rect& rect = _gameMapInfo->GetMonsterMapInfo()->GetRect();
    std::uniform_int_distribution<> genX(rect.StartX(), rect.EndX());
    std::uniform_int_distribution<> genY(rect.StartY(), rect.EndY());

    if (mapType == MapType::MONSTER)
    {
        for (int32 i = 0; i < _monsterCount; i++)
        {
            int32 startX = genX(rng);
            int32 startZ = genY(rng);
            GameMosterInfoRef info = std::make_shared<GameMosterInfo>(
                std::static_pointer_cast<GameRoom>(shared_from_this()), i, i % 2, 100, startX, startZ);
            info->SetStartPosition(startX, startZ);
            info->SetObjecteState(ObjectState::MOVE);

            _monsterMap[i] = info;
        }
    }
    else if (mapType == MapType::BOS)
    {
        for (int32 i = 0; i < _bosMonsterCount; i++)
        {
            GameBossInfoRef info = std::make_shared<GameBossInfo>(std::static_pointer_cast<GameRoom>(shared_from_this()),
                                                                    i, 2, 300, 0, 0);
            info->SetStartPosition(0, 0);
            info->SetObjecteState(ObjectState::MOVE);

            _monsterMap[i] = info;
        }
    }
}
