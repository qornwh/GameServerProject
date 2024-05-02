#include "pch.h"
#include "GameSerivce.h"
#include "GameGlobal.h"
#include "GameRoomManager.h"
#include "GameSession.h"
#include "IRoom.h"

#ifdef IOCPMODE
#include "SocketConfig.h"
#endif

#ifdef IOCPMODE
GameService::GameService(uint16 port, int32 _maxSessionCount) : Service(port, _maxSessionCount)
{
}

GameService::GameService(std::string host, uint16 port, int32 _maxSessionCount) : Service(host, port, _maxSessionCount)
{
}

void GameService::RegistAccept(OverlappedSocket* overlappedPtr)
{
    SessionRef session = std::make_shared<GameSession>(GetEndPoint());
    session->Init();
    session->SetService(shared_from_this());
    overlappedPtr->SetSession(session);
    SocketAcceptRegister(overlappedPtr);
    CrashFunc(SocketConfig::SetIoCompletionPort(session->GetSocket(), GetHandler()));
}
#else
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

#endif

GameService::~GameService()
{
}

SessionRef GameService::CreateSession()
{
#ifdef IOCPMODE
    GameSessionRef gameSession = std::make_shared<GameSession>(GetEndPoint());
#else
    GameSessionRef gameSession = std::make_shared<GameSession>(GetIoContext(), GetEndPoint());
#endif
    AddSessionRef(gameSession);
    return gameSession;
}

bool GameService::Start()
{
    std::cout << "Start Service !!!" << std::endl;
#ifdef IOCPMODE
    Init();

    for (int i = 0; i < 10; i++)
    {
        OverlappedSocket* overlappedPtr = new OverlappedSocket();
        overlappedPtr->SetType(0); // 초기 설정
        RegistAccept(overlappedPtr);
    }
#else
    RegistAccept();
#endif
    return true;
}

void GameService::BroadCast(SendBufferRef sendBuffer)
{
    ReadLockGuard read(lock, "read");
    for (const auto session : GetSession())
    {
        if (session->IsConnected())
        {
            GameSessionRef gameSession = std::static_pointer_cast<GameSession>(session);
            gameSession->AsyncWrite(sendBuffer);
        }
    }
}

void GameService::ReleaseSessionMesssage(SessionRef session)
{
    GameSessionRef gameSession = std::static_pointer_cast<GameSession>(session);
    if (gameSession->GetRoomId() > -1)
    {
        GRoomManger->getRoom(gameSession->GetRoomId())->OutSession(gameSession);
    }
}
