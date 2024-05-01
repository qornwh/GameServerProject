#include "pch.h"
#include "Service.h"
#include "Session.h"

EndPointUtil::EndPointUtil(boost::asio::ip::tcp::endpoint ep) : _ep(std::move(ep))
{
}

EndPointUtil::EndPointUtil(String addr, uint16 port) : _ep(
    boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(addr), port))
{
}

EndPointUtil::EndPointUtil(EndPointUtil& ep)
{
    SetAddress(ep.GetAddress());
    SetPort(ep.GetPort());
}

void EndPointUtil::SetAddress(boost::asio::ip::address addr)
{
    _ep.address(addr);
}

boost::asio::ip::address EndPointUtil::GetAddress()
{
    return _ep.address();
}

void EndPointUtil::SetPort(uint16 port)
{
    _ep.port(port);
}

const uint16 EndPointUtil::GetPort()
{
    return _ep.port();
}

Service::Service(boost::asio::io_context& ioContext, uint16 port, int32 maxSessionCount) :
    _maxSessionCount(maxSessionCount),
    _ep(boost::asio::ip::tcp::v4(), port),
    _ioContext(ioContext)
{
}

Service::Service(boost::asio::io_context& ioContext, std::string host, uint16 port, int32 maxSessionCount) :
    _maxSessionCount(maxSessionCount),
    _ep(boost::asio::ip::make_address(host), port),
    _ioContext(ioContext)
{
}

Service::~Service()
{
}

SessionRef Service::CreateSession()
{
    SessionRef session = std::make_shared<Session>(_ioContext, _ep);
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

void Service::RegistAccept()
{
}

void Service::run()
{
    _ioContext.run();
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
