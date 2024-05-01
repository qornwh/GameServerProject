﻿#pragma once
#include "Service.h"
#include "Session.h"

class GameService : public Service
{
public:
    GameService(boost::asio::io_context& io_context, uint16 port, int32 _maxSessionCount = 200);
    GameService(boost::asio::io_context& io_context, std::string host, uint16 port, int32 _maxSessionCount = 200);
    ~GameService();

    bool Start();
    void RegistAccept() override;
    void BroadCast(SendBufferRef sendBuffer) override;
    void ReleaseSessionMesssage(SessionRef session) override;
    SessionRef CreateSession() override;

private:
    boost::asio::ip::tcp::acceptor _acceptor;
};
