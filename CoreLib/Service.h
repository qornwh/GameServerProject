#pragma once
#include "pch.h"
#include <set>
#include "ReadWriteLock.h"

struct EndPointUtil
{
    EndPointUtil(boost::asio::ip::tcp::endpoint ep);
    EndPointUtil(string addr, uint16 port);
    EndPointUtil(EndPointUtil& ep);

    void SetAddress(boost::asio::ip::address addr);
    boost::asio::ip::address GetAddress();

    void SetPort(uint16 port);
    const uint16 GetPort();

    boost::asio::ip::tcp::endpoint* GetEndPoint() { return &_ep; }

private:
    boost::asio::ip::tcp::endpoint _ep;
};

class Service : public boost::enable_shared_from_this<Service>
{
public:
    Service(boost::asio::io_context& io_context, uint16 port, int32 _maxSessionCount = 200);
    Service(boost::asio::io_context& io_context, std::string host, uint16 port, int32 _maxSessionCount = 200);
    ~Service();

    int32 GetCurrentSessionCount() const { return _sessionCount; }
    int32 GetMaxSessionCount() const { return _maxSessionCount; }

    virtual SessionRef CreateSession();
    void AddSessionRef(SessionRef session);

    virtual void BroadCast(SendBufferRef sendBuffer);
    virtual void RegistAccept();
    virtual void ReleaseSessionMesssage(SessionRef session) {};

    void run() const;

    boost::asio::io_context& GetIoContext() const { return _ioContext; }
    boost::asio::ip::tcp::endpoint& GetEndPoint() { return _ep; }
    boost::system::error_code& GetErrorCode() { return _ec; }

    std::set<SessionRef> GetSession() { return _sessions; }
    void ReleaseSession(SessionRef session);

private:
    int32 _sessionCount = 0;
    int32 _maxSessionCount = 0;
    boost::asio::ip::tcp::endpoint _ep = {};

    boost::asio::io_context& _ioContext;
    boost::system::error_code _ec;

protected:
    std::set<SessionRef> _sessions;
    Lock lock;
};
