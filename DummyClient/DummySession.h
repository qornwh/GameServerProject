#pragma once
#include "Session.h"
#include "pch.h"

using DummyPlayerInfoRef = std::shared_ptr<class DummyPlayerInfo>;

class DummySession : public Session
{
public:
    DummySession(boost::asio::io_context& io_context, const boost::asio::ip::tcp::endpoint& ep);
    ~DummySession();

    void SetId(int32 id);
    int32 GetId() { return _id; }
    void OnWait();
    void AsyncLoad();
    void OnConnect(const boost::system::error_code& error) override;
    DummyPlayerInfoRef GetInfo() { return _playerInfo; }

private:
    int32 _id;
    DummyPlayerInfoRef _playerInfo;
    int32 _roomIdx = 0;
};
