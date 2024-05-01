#include "Service.h"
#include "pch.h"
#include "SocketConfig.h"
#include "Session.h"
#include "OverlappedSocket.h"

Service::Service(uint16 port, int32 maxSessionCount) :
    _maxSessionCount(maxSessionCount)
{
    _ep.port = port;
}

Service::Service(String host, uint16 port, int32 maxSessionCount) :
    _maxSessionCount(maxSessionCount)
{
    _ep.host = host;
    _ep.port = port;
}

Service::~Service()
{
}

void Service::Init()
{// iocp 초기화
    _iocpHd = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, (u_long)0, 0);
    if (_iocpHd == INVALID_HANDLE_VALUE)
    {
        WSACleanup();
        assert(-1);
    }

    // 서버 소켓 생성(리슨용)
    _serverSocket = SocketConfig::CreateSocket();
    if (_serverSocket == INVALID_SOCKET)
    {
        WSACleanup();
        assert(-1);
    }

    // iocp 등록
    if (!SocketConfig::SetIoCompletionPort(_serverSocket, _iocpHd))
    {
        WSACleanup();
        assert(-1);
    }

    // bind
    if (!SocketConfig::SocketBind(_serverSocket))
    {
        WSACleanup();
        assert(-1);
    }

    // listen
    if (!SocketConfig::SocketListen(_serverSocket))
    {
        WSACleanup();
        assert(-1);
    }

    ZeroMemory(&_overlapped, sizeof(_overlapped));
}

void Service::Start()
{
    Init();

    for (int i = 0; i < 10; i++)
    {
        OverlappedSocket* overlappedPtr = new OverlappedSocket();
        overlappedPtr->Init();
        overlappedPtr->SetType(0); // 초기 설정
        AcceptRegister(overlappedPtr);
    }
}

SessionRef Service::CreateSession()
{
    SessionRef session = std::make_shared<Session>(_ep);
    return session;
}

void Service::AddSessionRef(SessionRef session)
{
    WriteLockGuard wl(lock, "writeLock");
    _sessions.insert(session);
    _sessionCount++;
}

void Service::BroadCast(SendBufferRef sendBuffer)
{
    ReadLockGuard rl(lock, "BroadCast");
    for (auto session : _sessions)
    {
        if (session->IsConnected())
            session->AsyncWrite(sendBuffer);
    }
}

void Service::run()
{
    while (true)
    {
        DWORD numOfBytes = 0;
        ULONG_PTR key = 0;
        OverlappedSocket* overlappedPtr = nullptr;

        if (GetQueuedCompletionStatus(_iocpHd, &numOfBytes, &key, reinterpret_cast<LPOVERLAPPED*>(&overlappedPtr), 10))
        {
            if (overlappedPtr->GetType() == OverlappedSocket::Type::ACCP)
            {
                // 서버랑 연결되는 시점
                Accept(overlappedPtr);
            }
            else if (overlappedPtr->GetType() == OverlappedSocket::Type::READ)
            {
                // read disconn, 등
                std::shared_ptr<Session> session = overlappedPtr->GetSession();
                if (session == nullptr)
                    assert(-1);

                session->OnRead(numOfBytes);
            }
            else if (overlappedPtr->GetType() == OverlappedSocket::Type::SEND)
            {
                std::shared_ptr<Session> session = overlappedPtr->GetSession();
                if (session == nullptr)
                    assert(-1);

                session->OnWrite(numOfBytes);
            }
        }
        else
        {
            int errCode = ::WSAGetLastError();
            if (errCode != WAIT_TIMEOUT)
            {
                // 비정상 종료 체크
                printf("GetQueuedCompletionStatus 에러 발생 : %d\n", errCode);
                assert(-1);
            }
        }
    }
}

void Service::AcceptRegister(OverlappedSocket* overlappedPtr)
{
    SessionRef session = std::make_shared<Session>(_ep);
    session->Init();
    session->SetService(shared_from_this());
    overlappedPtr->SetSession(session);
    SocketAcceptRegister(overlappedPtr);
    if (!SocketConfig::SetIoCompletionPort(session->GetSocket(), _iocpHd))
    {
        assert(-1);
    }
}

void Service::Accept(OverlappedSocket* overlappedPtr)
{
    // 여기서 실제로 클라 연결요청이 들어오면 등록하고
    // 다시 새로운 클라이언트를 iocp에 등록해두어야된다.
    // 세션 추가

    std::shared_ptr<Session> session = overlappedPtr->GetSession();
    if (session == nullptr)
        assert(-1);

    if (SocketConfig::SetUpdateAcceptSocket(session->GetSocket(), _serverSocket))
    {
        AcceptRegister(overlappedPtr);
        return;
    }

    SOCKADDR_IN sockAddress;
    int sizeOfSockAddr = sizeof(sockAddress);
    if (SOCKET_ERROR == ::getpeername(session->GetSocket(), OUT reinterpret_cast<SOCKADDR*>(&sockAddress), &sizeOfSockAddr))
    {
        AcceptRegister(overlappedPtr);
        return;
    }

    session->OnConnect();
    AcceptRegister(overlappedPtr);
}

void Service::ReleaseSession(SessionRef session)
{
    {
        WriteLockGuard wl(lock, "writeLock");
        _sessions.erase(session);
        _sessionCount--;
    }

    ReleaseSessionMesssage(session);
}

void Service::SocketAcceptRegister(OverlappedSocket* overlappedPtr)
{
    std::shared_ptr<Session> session = overlappedPtr->GetSession();
    if (session == nullptr)
        assert(-1);
    session->AsyncConnect(overlappedPtr);
}
