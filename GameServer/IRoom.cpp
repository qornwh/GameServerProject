#include "IRoom.h"

#include "GameMapInfo.h"
#include "GamePacketHandler.h"
#include "GameRoomQuest.h"
#include "GameService.pb.h"

void GameRoom::EnterSession(GameSessionRef session)
{
    _playerMap.emplace(session->GetPlayer()->GetCode(), session->GetPlayer());
    IRoom<boost::shared_ptr<GameSession>, boost::shared_ptr<Session>>::EnterSession(session);
    session->SetRoomId(_id);
    {
        protocol::SInsertplayer sendPkt;
        protocol::Player* player = new protocol::Player;
        protocol::Unit* unit = new protocol::Unit;
        unit->set_name(session->GetPlayer()->GetName());
        unit->set_code(session->GetPlayer()->GetCode());
        unit->set_type(session->GetPlayer()->GetType());
        unit->set_hp(session->GetPlayer()->GetHp());
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
                GameSessionRef gameSession = static_pointer_cast<GameSession>(it);

                if (gameSession->GetSessionId() == session->GetSessionId())
                    continue;

                if (gameSession->GetPlayer() != nullptr)
                {
                    boost::shared_ptr<GamePlayerInfo> info = gameSession->GetPlayer();
                    // 설마 안지우지는 않겠지. pkt 소멸되면 메모리도 같이 날리겠지. 그냥 믿겠다!!!
                    protocol::Player* player = sendPkt.add_player();
                    protocol::Unit* unit = new protocol::Unit;
                    unit->set_name(info->GetName());
                    unit->set_code(info->GetCode());
                    unit->set_type(info->GetType());
                    unit->set_hp(info->GetHp());
                    protocol::Position* position = new protocol::Position;
                    position->set_x(info->GetPosition().X);
                    position->set_y(info->GetPosition().Y);
                    position->set_z(info->GetPosition().Z);
                    position->set_yaw(info->GetPosition().Yaw);
                    // 메모리 할당이 아니라 스택메모리에 position 있어서 바로 보내야된다.
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
            position->set_x(info->GetPosition().X);
            position->set_y(info->GetPosition().Y);
            position->set_z(info->GetPosition().Z);
            position->set_yaw(info->GetPosition().Yaw);
            // 메모리 할당이 아니라 스택메모리에 position 있어서 바로 보내야된다.
            unit->set_allocated_position(position);
            monster->set_state(info->GetObjectState());
            monster->set_allocated_unit(unit);
        }
        SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(sendPkt, protocol::MessageCode::S_LOAD);
        session->AsyncWrite(sendBuffer);
    }
}

void GameRoom::OutSession(GameSessionRef session)
{
    // playerInfo 레퍼런스카운트 미리 제거해둔다.
    _playerMap.erase(session->GetPlayer()->GetCode());
    IRoom<boost::shared_ptr<GameSession>, boost::shared_ptr<Session>>::OutSession(session);
    session->SetRoomId(-1);

    protocol::SClosePlayer sendPkt;
    sendPkt.set_code(session->GetPlayer()->GetCode());

    SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(sendPkt, protocol::MessageCode::S_CLOSEPLAYER);
    BroadCastAnother(sendBuffer, session->GetPlayer()->GetCode());
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

        GameRoomQuestInfo& rqInfo =_gameRoomQuest->GetInfo();
        if (rqInfo.IsKilled())
        {
            rqInfo.SetDeadMonster();

            protocol::SRoomQuest pkt;
            pkt.set_is_clear(rqInfo.IsClear());
            pkt.set_kill_count(rqInfo.GetKillCount());
            pkt.set_sum_kill(rqInfo.GetSumCount());
            SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(pkt, protocol::MessageCode::S_ROOMQUEST);
            BroadCast(sendBuffer);
        }
    }));
}

void GameRoom::Task()
{
    _isTask.exchange(true);
    protocol::SUnitStates pkt;
    const MapType mapType = _gameMapInfo->GetMonsterMapInfo()->GetMapType();
    const MapInfoRef monsterMap = _gameMapInfo->GetMonsterMapInfo();

    // _gameStrand에 넣었기 때문에 동시에 발생되는 문제는 없다 !!!
    while (!attackQueue.empty())
    {
        GamePlayerInfoRef attackInfo = attackQueue.front();
        attackQueue.pop();

        int32 skillCode = attackInfo->GetObjectState();
        int32 targetCode = attackInfo->GetTarget();
        GameMosterInfoRef info = GetMonster(targetCode);
        vector<int32> attackList;

        if (targetCode < 0)
        {
            // 논타겟
            attackInfo->Attack(nullptr, attackList);
        }
        else
        {
            // 타겟
            if (info != nullptr)
            {
                attackInfo->Attack(info, attackList);
            }
        }

        // 공격 판정된 몬스터 리스트들
        for (auto monsterCode : attackList)
        {
            GameMosterInfoRef info = GetMonster(monsterCode);
            if (info != nullptr && info->GetObjectState() != ObjectState::DIE)
            {
                cout << "공격 성공 : " << info->GetCode() << endl;
                info->SetTarget(attackInfo->GetCode());
                info->IdlePosition();
                if (info->GetHp() > 0)
                {
                    info->SetObjecteState(ObjectState::HITED);
                    info->SetTarget(attackInfo->GetCode());
                }
                else
                {
                    info->SetObjecteState(ObjectState::DIE);
                    _gameRoomQuest->GetInfo().AddDeadMonster();
                }
            }
        }
    }

    if (mapType == MapType::MONSTER)
    {
        for (auto& it : _monsterMap)
        {
            GameMosterInfoRef info = it.second;
            switch (info->GetObjectState())
            {
            case ObjectState::IDLE:
                {
                    if (info->AddIdleCounter() == 0)
                    {
                        info->IdlePosition();
                        protocol::UnitState* childPkt = pkt.add_unit_state();
                        childPkt->set_is_monster(true);
                        protocol::Monster* monster = new protocol::Monster();
                        monster->set_state(info->GetObjectState());
                        protocol::Unit* unit = new protocol::Unit();
                        unit->set_code(info->GetCode());
                        unit->set_hp(info->GetHp());
                        unit->set_type(info->GetType());
                        unit->set_name(info->GetName());
                        protocol::Position* position = new protocol::Position();
                        position->set_x(info->GetPosition().X);
                        position->set_y(info->GetPosition().Y);
                        position->set_z(info->GetPosition().Z);
                        position->set_yaw(info->GetPosition().Yaw);
                        unit->set_allocated_position(position);
                        monster->set_allocated_unit(unit);
                        childPkt->set_allocated_monster(monster);
                    }
                }
                break;
            case ObjectState::MOVE:
                {
                    if (info->AddMoveCounter() == 0)
                    {
                        protocol::UnitState* childPkt = pkt.add_unit_state();
                        childPkt->set_is_monster(true);
                        protocol::Monster* monster = new protocol::Monster();
                        monster->set_state(info->GetObjectState());
                        protocol::Unit* unit = new protocol::Unit();
                        unit->set_code(info->GetCode());
                        protocol::Position* position = new protocol::Position();
                        if (info->GetTarget() >= 0 && _playerMap.find(info->GetTarget()) != _playerMap.end())
                        {
                            FVector& pos = _playerMap[info->GetTarget()]->GetPosition();
                                
                            if (_gameMapInfo->GetMonsterMapInfo()->InRect(pos.X, pos.Z))
                            {
                                info->MoveTarget(pos);
                            }
                            else
                            {
                                info->Move();
                            }
                        }
                        else
                        {
                            info->Move();
                        }
                        monsterMap->InSetRect(info->GetPosition().X, info->GetPosition().Z);
                        position->set_x(info->GetPosition().X);
                        position->set_y(info->GetPosition().Y);
                        position->set_z(info->GetPosition().Z);
                        position->set_yaw(info->GetPosition().Yaw);
                        unit->set_allocated_position(position);
                        monster->set_allocated_unit(unit);
                        childPkt->set_allocated_monster(monster);
                    }
                    info->updatePrePosition();
                }
                break;
            case ObjectState::ATTACK:
                {
                    if (info->AddAttackCounter() == 0)
                    {
                        protocol::UnitState* childPkt = pkt.add_unit_state();
                        childPkt->set_is_monster(true);
                        childPkt->set_demage(10);
                        protocol::Monster* monster = new protocol::Monster();
                        monster->set_state(info->GetObjectState());
                        protocol::Unit* unit = new protocol::Unit();
                        unit->set_code(info->GetCode());
                        monster->set_allocated_unit(unit);
                        childPkt->set_allocated_monster(monster);
                    }
                }
                break;
            case ObjectState::HITED:
                {
                    if (info->AddHitCounter() == 0)
                    {
                        // 이순간 동시에 여러번 맞기 가능하기 때문에 패킷 데미지 처리 필요!!
                        protocol::UnitState* childPkt = pkt.add_unit_state();
                        childPkt->set_is_monster(true);
                        childPkt->set_demage(info->GetDamage());
                        info->ResetDamage();
                        protocol::Monster* monster = new protocol::Monster();
                        monster->set_state(info->GetObjectState());
                        protocol::Unit* unit = new protocol::Unit();
                        unit->set_code(info->GetCode());
                        unit->set_hp(info->GetHp());
                        protocol::Position* position = new protocol::Position();
                        position->set_x(info->GetPosition().X);
                        position->set_y(info->GetPosition().Y);
                        position->set_z(info->GetPosition().Z);
                        position->set_yaw(info->GetPosition().Yaw);
                        unit->set_allocated_position(position);
                        monster->set_allocated_unit(unit);
                        childPkt->set_allocated_monster(monster);
                    }
                }
                break;
            case ObjectState::DIE:
                {
                    // 리스폰 필요
                    // 이순간 동시에 여러번 맞기 가능하기 때문에 패킷 데미지 처리 필요!!
                    if (info->AddDieCounter() == 0)
                    {
                        protocol::UnitState* childPkt = pkt.add_unit_state();
                        childPkt->set_is_monster(true);
                        childPkt->set_demage(info->GetDamage());
                        info->ResetDamage();
                        protocol::Monster* monster = new protocol::Monster();
                        protocol::Unit* unit = new protocol::Unit();
                        monster->set_state(ObjectState::DIE);
                        unit->set_code(info->GetCode());
                        monster->set_allocated_unit(unit);
                        childPkt->set_allocated_monster(monster);
                    }
                }
                break;
            }
        }
    }
    
    if (pkt.unit_state_size() > 0)
    {
        SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(pkt, protocol::MessageCode::S_UNITSTATES);
        BroadCast(sendBuffer);
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
    // - 일단 완
    // 케이스 2
    // 플레이어의 공격명령(이때 현재 좌표도 함께 받음)을 받고 현재몬스터들의 좌표에 공격범위에 있으면 몬스터 Hit처리후 클라에게 메시지 전달
    // - 일단 완
    // 케이스 3
    // 공격받은 몬스터들은 플레이어 쪽으로 이동 (방향전환 함께)
    // - 일단 완
    // 케이스 4
    // 공격받은 몬스터들의 공격범위에 플레이어가 있으면 공격 및 데미지 계산 플레이어 Hit처리후 클라에게 공격 데미지 메시지 전달.
    // - 하는중
}

void GameRoom::CreateMapInfo(int32 type)
{
    // 일단 하드코딩으로 생성해둔다.
    if (type == 0)
    {
        // 일반 몹 맵
        _gameMapInfo = boost::make_shared<GameMapInfo>(25, 25, 0, 0);
        _gameMapInfo->CreateMonsterMapInfo(22, 15, 0, 0, MapType::MONSTER);
        _gameRoomQuest = boost::make_shared<GameRoomQuest>(5);
        _monsterCount = 10;
    }
    else if (type == 1)
    {
        // 보스 몹 맵
        _gameMapInfo = boost::make_shared<GameMapInfo>(15, 15, 0, 0);
        _gameMapInfo->CreateMonsterMapInfo(10, 10, 0, 0, MapType::BOS);
        _gameRoomQuest = boost::make_shared<GameRoomQuest>(1);
        _bosMonsterCount = 1;
    }

    InitMonsters();
}

void GameRoom::InitMonsters()
{
    MapType mapType = _gameMapInfo->GetMonsterMapInfo()->GetMapType();
    Rect& rect = _gameMapInfo->GetMonsterMapInfo()->GetRect();
    boost::random::uniform_int_distribution<> genX(rect.StartX(), rect.EndX());
    boost::random::uniform_int_distribution<> genY(rect.StartY(), rect.EndY());

    if (mapType == MapType::MONSTER)
    {
        for (int32 i = 0; i < _monsterCount; i++)
        {
            int32 startX = genX(rng);
            int32 startZ = genY(rng);
            GameMosterInfoRef info = boost::make_shared<GameMosterInfo>(i, 0, 100, startX, startZ);
            info->SetStartPosition(startX, startZ);
            info->SetObjecteState(ObjectState::MOVE);

            _monsterMap[i] = info;
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
