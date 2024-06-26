﻿#pragma once
#include "Service.h"

using DummySessionRef = std::shared_ptr<class DummySession>;

static boost::asio::chrono::milliseconds gTimerDelay = boost::asio::chrono::milliseconds(3000);

class DummyService : public Service
{
public:
    DummyService(boost::asio::io_context& io_context, std::string host, uint16 port);
    DummyService(boost::asio::io_context& io_context, std::string host, uint16 port, int32 _maxSessionCount);
    ~DummyService();

    bool Start();
    SessionRef CreateSession() override;

    void StartTimer();
    void AsyncSession();

    void RegistAccept() override;

private:
    boost::asio::strand<boost::asio::io_context::executor_type> _strand;
    boost::asio::steady_timer _timer;

    uint8 _tick = 0;
};
