#pragma once
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "Session.h"

class Pawn;
using PawnRef = boost::shared_ptr<Pawn>;

class DummySession : public Session
{
public:
    DummySession(boost::asio::io_context& io_context, const boost::asio::ip::tcp::endpoint& ep);
    ~DummySession();

    void SetId(int32 id);
    void SetUUID(boost::uuids::uuid uuid);

    int32 GetId() { return _id; }

    void OnWait();
    void AsyncTestMessage(const boost::system::error_code& ec);

    void AsyncLoad();

    void OnConnect(const boost::system::error_code& error) override;

    PawnRef GetPawn() { return _pawn; }

private:
    int32 _id;
    boost::uuids::uuid _uuid;
    boost::uuids::random_generator generator;

    PawnRef _pawn;
};
