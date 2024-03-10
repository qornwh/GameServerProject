#pragma once
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "Session.h"
#include "pch.h"

class DummyPlayerInfo;
using DummyPlayerInfoRef = boost::shared_ptr<class DummyPlayerInfo>;

class DummySession : public Session
{
public:
    DummySession(boost::asio::io_context& io_context, const boost::asio::ip::tcp::endpoint& ep);
    ~DummySession();

    void SetId(int32 id);
    void SetUUID(boost::uuids::uuid uuid);

    int32 GetId() { return _id; }

    void OnWait();

    void AsyncLoad();

    void OnConnect(const boost::system::error_code& error) override;

    DummyPlayerInfoRef GetInfo() { return _playerInfo; }

private:
    int32 _id;
    boost::uuids::uuid _uuid;
    boost::uuids::random_generator generator;

    DummyPlayerInfoRef _playerInfo;

    int32 _roomIdx = 0;
};
