#include "GameSerivce.h"

#include "GameGlobal.h"
#include "GameRoomManager.h"
#include "GameSession.h"
#include "IRoom.h"


GameService::GameService(boost::asio::io_context& io_context, uint16 port,
                         int32 _maxSessionCount) : Service(io_context, port, _maxSessionCount),
                                                   _acceptor(io_context, GetEndPoint())
{
}

GameService::GameService(boost::asio::io_context& io_context, std::string host, uint16 port,
                         int32 _maxSessionCount) : Service(io_context, host, port, _maxSessionCount),
                                                   _acceptor(io_context, GetEndPoint())
{
}

GameService::~GameService()
{
}

void GameService::InitRoom()
{
    // 일반몹 맵 = 0
    GRoomManger->CreateRoom(static_pointer_cast<GameService>(shared_from_this()));
    // 보스몹 맵 = 1
    GRoomManger->CreateRoom(static_pointer_cast<GameService>(shared_from_this()));
}

SessionRef GameService::CreateSession()
{
    GameSessionRef gameSession = boost::make_shared<GameSession>(GetIoContext(), GetEndPoint());
    AddSessionRef(gameSession);
    return gameSession;
}

bool GameService::Start()
{
    cout << "Start Service !!!" << endl;
    InitRoom();
    RegistAccept();
    return true;
}

void GameService::RegistAccept()
{
    SessionRef session = CreateSession();
    auto ptr = shared_from_this();
    _acceptor.async_accept(session->GetSocket(),
                           [ptr, session](const boost::system::error_code& ec)
                           {
                               session->OnConnect(ec);
                               session->SetService(ptr);
                               ptr->AddSessionRef(session);
                               ptr->RegistAccept();
                           });
}

void GameService::BroadCast(SendBufferRef sendBuffer)
{
    ReadLockGuard read(lock, "read");
    for (const auto session : GetSession())
    {
        if (session->IsConnected())
        {
            GameSessionRef gameSession = static_pointer_cast<GameSession>(session);
            gameSession->AsyncWrite(sendBuffer);
        }
    }
}

void GameService::ReleaseSessionMesssage(SessionRef session)
{
    // 일단 같은 지역만 strand로 끊는다!!!
    GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

    if (gameSession->GetRoomId() > -1)
    {
        GRoomManger->getRoom(gameSession->GetRoomId())->OutSession(gameSession);
    }
}
