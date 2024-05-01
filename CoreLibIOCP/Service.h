#pragma once
#include "pch.h"
#include "ReadWriteLock.h"

struct EndPointUtil
{
    String host = "";
    uint16 port;
};

class Service : public std::enable_shared_from_this<Service>
{
public:
    Service(uint16 port, int32 _maxSessionCount = 200);
    Service(String host, uint16 port, int32 _maxSessionCount = 200);
    ~Service();

    void Init();
    void Start();

    SOCKET GetServerSocket()
    {
        return _serverSocket;
    }

    HANDLE GetHandler()
    {
        return _iocpHd;
    }

    void run();
    void AcceptRegister(class OverlappedSocket* overlappedPtr);
    void Accept(class OverlappedSocket* overlappedPtr);

    virtual SessionRef CreateSession();
    void AddSessionRef(SessionRef session);
    void ReleaseSession(SessionRef session);
    std::set<SessionRef> GetSession() { return _sessions; }

    EndPointUtil& GetEndPoint() { return _ep; }

    virtual void BroadCast(SendBufferRef sendBuffer);
    virtual void ReleaseSessionMesssage(SessionRef session) {};

    int32 GetCurrentSessionCount() const { return _sessionCount; }
    int32 GetMaxSessionCount() const { return _maxSessionCount; }

private:
    void SocketAcceptRegister(class OverlappedSocket* overlappedPtr);

private:
    int32 _sessionCount = 0;
    int32 _maxSessionCount = 0;
    EndPointUtil _ep;

    HANDLE _iocpHd;
    SOCKET _serverSocket = INVALID_SOCKET;
    OVERLAPPED _overlapped;

protected:
    std::set<SessionRef> _sessions;
    Lock lock;
};
