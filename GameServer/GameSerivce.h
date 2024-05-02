#pragma once
#include "Service.h"
#include "Session.h"

class GameService : public Service
{
#ifdef IOCPMODE
public:
    GameService(uint16 port, int32 _maxSessionCount = 200);
    GameService(std::string host, uint16 port, int32 _maxSessionCount = 200);
    void RegistAccept(OverlappedSocket* overlappedPtr) override;
#else
public:
    GameService(boost::asio::io_context& io_context, uint16 port, int32 _maxSessionCount = 200);
    GameService(boost::asio::io_context& io_context, std::string host, uint16 port, int32 _maxSessionCount = 200);
    void RegistAccept() override;
private:
    boost::asio::ip::tcp::acceptor _acceptor;
#endif
    
public:
    ~GameService();

    bool Start();
    void BroadCast(SendBufferRef sendBuffer) override;
    void ReleaseSessionMesssage(SessionRef session) override;
    SessionRef CreateSession() override;
};
