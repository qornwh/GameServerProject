#pragma once
#include "pch.h"
#include "ReadWriteLock.h"

struct EndPointUtil
{
    String host = "";
    uint16 port;
};

class OverlappedSocket;

class Service : public std::enable_shared_from_this<Service>
{
public:
    Service(uint16 port, int32 _maxSessionCount = 200);
    Service(String host, uint16 port, int32 _maxSessionCount = 200);
    ~Service();

    void Init();
    virtual bool Start();
    void run();
    void task();
    virtual void RegistAccept(OverlappedSocket* overlappedPtr);
    void Accept(OverlappedSocket* overlappedPtr);
    virtual SessionRef CreateSession();
    void AddSessionRef(SessionRef session);
    void ReleaseSession(SessionRef session);
    virtual void BroadCast(SendBufferRef sendBuffer);
    virtual void ReleaseSessionMesssage(SessionRef session) {};

    HANDLE GetHandler() { return _iocpHd; }
    SOCKET GetServerSocket() { return _serverSocket; }
    std::set<SessionRef> GetSession() { return _sessions; }
    EndPointUtil& GetEndPoint() { return _ep; }
    int32 GetCurrentSessionCount() const { return _sessionCount; }
    int32 GetMaxSessionCount() const { return _maxSessionCount; }
    void SocketAcceptRegister(OverlappedSocket* overlappedPtr);
    void ErrorCode(int32 errorCode);

private:
    int32 _sessionCount = 0;
    int32 _maxSessionCount = 0;
    HANDLE _iocpHd;
    SOCKET _serverSocket = INVALID_SOCKET;
    OVERLAPPED _overlapped;
    EndPointUtil _ep;

protected:
    std::set<SessionRef> _sessions;
    Lock lock;
};
