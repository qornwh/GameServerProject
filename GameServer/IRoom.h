#pragma once
#include <queue>
#include <set>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include "GameGlobal.h"
#include "GameRoomManager.h"
#include "pch.h"
#include "GameSession.h"
#include "GameObjectInfo.h"
#include "GameUtils.h"

// 일단 이구간 템플릿 연구 필요 아직 템플릿 숙지 덜됨, 컴파일시 어떻게 돌아가는지??

template <typename T, typename = std::enable_if_t<std::is_base_of_v<SessionRef, T>>>
class IRoom : public enable_shared_from_this<IRoom<T, SessionRef>>
{
public:
    IRoom(boost::asio::io_context& io_context, uint32 id) : _id(id), _strand(boost::asio::make_strand(io_context))
    {
    }

    virtual ~IRoom()
    {
    }

    virtual void EnterSession(T session)
    {
        // 스트랜드 lock 처리 필요 x
        // 1개씩 처리됨
        boost::asio::post(boost::asio::bind_executor(_strand, [this, session]
        {
            std::cout << "세션 추가 !!!" << std::endl;
            _sessionList.insert(session);
        }));
    }

    virtual void OutSession(T session)
    {
        // 스트랜드 lock 처리 필요 x
        // 1개씩 처리됨
        boost::asio::post(boost::asio::bind_executor(_strand, [this, session]
        {
            std::cout << "세션 탈주 !!!" << std::endl;
            _sessionList.erase(session);
        }));
    }

    virtual void BroadCast(SendBufferRef sendBuffer)
    {
        boost::asio::post(boost::asio::bind_executor(_strand, [this, sendBuffer]
        {
            for (const auto session : _sessionList)
            {
                // 여기에 쓴다.
                session->AsyncWrite(sendBuffer);
            }
        }));
    }

    virtual void SetTimer()
    {
    }

    virtual void Tick()
    {
    }

protected:
    set<T> _sessionList;
    int32 _id;
    uint32 _type;

    int32 _frameTime = 50; // 50ms, 20프레임

    boost::asio::strand<boost::asio::io_context::executor_type> _strand;
};

class GameRoom : public IRoom<GameSessionRef, SessionRef>
{
public:
    GameRoom(boost::asio::io_context& io_context, uint32 id) :
        IRoom<boost::shared_ptr<GameSession>, boost::shared_ptr<Session>>(io_context, id),
        _timer(io_context, _timerDelay),
        _gameStrand(boost::asio::make_strand(io_context))
    {
        _type = GRoomManger->RoomType::space; // 일단 디폴트
        CreateMapInfo(id);
        StartGameRoom();
    }

    ~GameRoom() override
    {
        std::cout << "~GameRoom !!!" << std::endl;
    }

    void EnterSession(GameSessionRef session) override;
    void OutSession(GameSessionRef session) override;
    void AttackSession(GameSessionRef session);
    void BuffSession(GameSessionRef session);

    void BroadCastAnother(SendBufferRef sendBuffer, int32 code) const
    {
        boost::asio::post(boost::asio::bind_executor(_strand, [this, sendBuffer, code]
        {
            for (const auto session : _sessionList)
            {
                // 여기에 쓴다.
                if (session->GetPlayer()->GetCode() != code)
                    session->AsyncWrite(sendBuffer);
            }
        }));
    }

    void StartGameRoom();
    void Tick() override;
    void Task();

    void CreateMapInfo(int32 type);

    void InitMonsters();
    void SpawnMonsters();
    void MoveMonsters();
    void AttackMonster();

    unordered_map<int32, GameMosterInfoRef> GetMonsterMap() { return _monsterMap; }
    GameMosterInfoRef GetMonster(int32 Code)
    {
        if (_monsterMap.find(Code) != _monsterMap.end())
            return _monsterMap[Code];
        return nullptr;
    }

private:
    boost::asio::steady_timer _timer;
    boost::asio::strand<boost::asio::io_context::executor_type> _gameStrand;
    boost::asio::chrono::milliseconds _timerDelay = boost::asio::chrono::milliseconds(100);
    boost::random::mt19937_64 rng;

    int32 _monsterCount = -1;
    int32 _bosMonsterCount = -1;
    queue<GamePlayerInfoRef> attackQueue;

    GameMapInfoRef _gameMapInfo;
    GameRoomQuestRef _gameRoomQuest;
    unordered_map<int32, GameMosterInfoRef> _monsterMap;
    unordered_map<int32, GamePlayerInfoRef> _playerMap;
    Atomic<bool> _isTask{false};
    GameUtils::TickCounter _tickCounter{10};
};
