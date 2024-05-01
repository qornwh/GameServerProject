#pragma once
#include <random>
#include "pch.h"
#include "GameGlobal.h"
#include "GameRoomManager.h"
#include "GameSession.h"
#include "GameObjectInfo.h"
#include "GameUtils.h"

template <typename T, typename = std::enable_if_t<std::is_base_of_v<SessionRef, T>>>
class IRoom : public std::enable_shared_from_this<IRoom<T, SessionRef>>
{
#ifdef IOCPMODE
public:
    IRoom(uint32 id) : _id(id)
    {
    }

    virtual void EnterSession(T session)
    {
        // boost::asio::post(boost::asio::bind_executor(_strand, [this, session]
        // {
        //     std::cout << "Session Add !!!" << std::endl;
        //     _sessionList.insert(session);
        // }));
    }

    virtual void OutSession(T session)
    {
        // boost::asio::post(boost::asio::bind_executor(_strand, [this, session]
        // {
        //     std::cout << "Session Out !!!" << std::endl;
        //     _sessionList.erase(session);
        // }));
    }

    virtual void BroadCast(SendBufferRef sendBuffer)
    {
        // boost::asio::post(boost::asio::bind_executor(_strand, [this, sendBuffer]
        // {
        //     for (const auto session : _sessionList)
        //     {
        //         session->AsyncWrite(sendBuffer);
        //     }
        // }));
    }

    void BroadCastAnother(SendBufferRef sendBuffer, int32 code) const
    {
        // boost::asio::post(boost::asio::bind_executor(_strand, [this, sendBuffer, code]
        // {
        //     for (const auto session : _sessionList)
        //     {
        //         // 여기에 쓴다.
        //         if (session->GetPlayer()->GetCode() != code)
        //             session->AsyncWrite(sendBuffer);
        //     }
        // }));
    }
protected:
    
#else
    
public:
    IRoom(boost::asio::io_context& io_context, uint32 id) : _id(id), _strand(boost::asio::make_strand(io_context))
    {
    }

    virtual void EnterSession(T session)
    {
        // 스트랜드 lock 처리 필요 x
        // 1개씩 처리됨
        boost::asio::post(boost::asio::bind_executor(_strand, [this, session]
        {
            std::cout << "Session Add !!!" << std::endl;
            _sessionList.insert(session);
        }));
    }

    virtual void OutSession(T session)
    {
        boost::asio::post(boost::asio::bind_executor(_strand, [this, session]
        {
            std::cout << "Session Out !!!" << std::endl;
            _sessionList.erase(session);
        }));
    }

    virtual void BroadCast(SendBufferRef sendBuffer)
    {
        boost::asio::post(boost::asio::bind_executor(_strand, [this, sendBuffer]
        {
            for (const auto session : _sessionList)
            {
                session->AsyncWrite(sendBuffer);
            }
        }));
    }

    void BroadCastAnother(SendBufferRef sendBuffer, int32 code)
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
protected:
    boost::asio::strand<boost::asio::io_context::executor_type> _strand;
#endif

public:
    virtual ~IRoom()
    {
    }
    
    virtual void SetTimer()
    {
    }

    virtual void Tick()
    {
    }

protected:
    Set<T> _sessionList;
    int32 _id;
    uint32 _type;
    int32 _frameTime = 50; // 50ms, 20프레임
};

class GameRoom : public IRoom<GameSessionRef, SessionRef>
{
#ifdef IOCPMODE
public:
    GameRoom(uint32 id) :
        IRoom<std::shared_ptr<GameSession>, std::shared_ptr<Session>>(id)
    {
        _type = GRoomManger->RoomType::space;
    }
#else
public:
    GameRoom(boost::asio::io_context& io_context, uint32 id) :
        IRoom<std::shared_ptr<GameSession>, std::shared_ptr<Session>>(io_context, id),
        _timer(io_context, boost::asio::chrono::milliseconds(100)),
        _gameStrand(boost::asio::make_strand(io_context))
    {
        _type = GRoomManger->RoomType::space;
    }
private:
    boost::asio::steady_timer _timer;
    boost::asio::strand<boost::asio::io_context::executor_type> _gameStrand;
    boost::asio::chrono::milliseconds _timerDelay{100};
#endif

public:
    ~GameRoom() override
    {
    }

    void EnterSession(GameSessionRef session) override;
    void OutSession(GameSessionRef session) override;
    void AttackSession(GameSessionRef session);
    void BuffSession(GameSessionRef session);
    void StartGameRoom();
    void Tick() override;
    void Task();
    GameMapInfoRef CreateMapInfo(int32 type);
    void InitMonsters();

    GameMosterInfoRef GetMonster(int32 Code)
    {
        if (_monsterMap.find(Code) != _monsterMap.end())
            return _monsterMap[Code];
        return nullptr;
    }
    
    GamePlayerInfoRef GetPlayer(int32 Code)
    {
        if (_playerMap.find(Code) != _playerMap.end())
            return _playerMap[Code];
        return nullptr;
    }

    Map<int32, GameMosterInfoRef> GetMonsterMap() { return _monsterMap; }
    Map<int32, GamePlayerInfoRef> GetPlayerMap() { return _playerMap; }
    GameMapInfoRef GetGameMap() { return _gameMapInfo; }
    GameRoomQuestRef GetQuest() { return _gameRoomQuest; }
    protocol::SUnitStates& GetUnitPacket() { return _unitPkt; }

private:
    int32 _monsterCount = -1;
    int32 _bosMonsterCount = -1;
    Queue<GamePlayerInfoRef> attackQueue;
    GameMapInfoRef _gameMapInfo;
    GameRoomQuestRef _gameRoomQuest;
    Map<int32, GameMosterInfoRef> _monsterMap;
    Map<int32, GamePlayerInfoRef> _playerMap;
    Atomic<bool> _isTask{false};
    GameUtils::TickCounter _tickCounter{10};
    protocol::SUnitStates _unitPkt;
    std::mt19937_64 rng;
};
