#pragma once
#include <set>

#include "GameGlobal.h"
#include "GameRoomManager.h"
#include "GameSerivce.h"
#include "pch.h"
#include "GameSession.h"
#include "GamePlayer.h"

// 일단 이구간 템플릿 연구 필요 아직 템플릿 숙지 덜됨, 컴파일시 어떻게 돌아가는지??

template <typename T, typename = std::enable_if_t<std::is_base_of_v<SessionRef, T>>>
class IRoom : public enable_shared_from_this<IRoom<T, SessionRef>>
{
public:
    IRoom(GameServiceRef gameService, uint32 id) : _gameService(gameService), _id(id), _strand(boost::asio::make_strand(_gameService.lock()->GetIoContext()))
    {
    }

    virtual ~IRoom() {};

    virtual void EnterSession(T session)
    {
        // 스트랜드 lock 처리 필요 x
        // 1개씩 처리됨
        boost::asio::post(boost::asio::bind_executor(_strand, [this, session]
        {
            std::cout << "세션 추가 !!!" << std::endl;
            sessionList.insert(session);
        }));
    }

    virtual void OutSession(T session)
    {
        // 스트랜드 lock 처리 필요 x
        // 1개씩 처리됨
        boost::asio::post(boost::asio::bind_executor(_strand, [this, session]
        {
            std::cout << "세션 탈주 !!!" << std::endl;
            sessionList.erase(session);
        }));
    }

    virtual void BroadCast(SendBufferRef sendBuffer)
    {
        boost::asio::post(boost::asio::bind_executor(_strand, [this, sendBuffer]
        {
            for (const auto session : sessionList)
            {
                // 여기에 쓴다.
                session->AsyncWrite(sendBuffer);
            }
        }));
    }

    void SetTimer();

protected:
    set<T> sessionList;
    int32 _id;
    uint32 _type;

    boost::weak_ptr<GameService> _gameService;
    boost::asio::strand<boost::asio::io_context::executor_type> _strand;
};

class GameRoom : public IRoom<GameSessionRef, SessionRef>
{
public:
    GameRoom(GameServiceRef gameService, uint32 id) : IRoom<boost::shared_ptr<GameSession>, boost::shared_ptr<Session>>(gameService, id)
    {
        _type = GRoomManger->RoomType::space; // 일단 디폴트
    }

    ~GameRoom() override
    {
        std::cout << "~GameRoom !!!" << std::endl;
    }

    void EnterSession(GameSessionRef session) override;

    void OutSession(boost::shared_ptr<GameSession> session) override;
    
    void BroadCastAnother(SendBufferRef sendBuffer, int32 code) const
    {
        boost::asio::post(boost::asio::bind_executor(_strand, [this, sendBuffer, code]
        {
            for (const auto session : sessionList)
            {
                // 여기에 쓴다.
                if (session->GetPlayer()->GetCode() != code)
                    session->AsyncWrite(sendBuffer);
            }
        }));
    }
};
