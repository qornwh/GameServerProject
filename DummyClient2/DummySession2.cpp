#include "DummySession2.h"

#include "UUIDGen.h"
#include <boost/make_shared.hpp>

#include "DummyPlayerInfo.h"
#include "DummyProto.pb.h"
#include "GamePacketHandler.h"

DummySession::DummySession(boost::asio::io_context& io_context, const boost::asio::ip::tcp::endpoint& ep)
    : Session(io_context, ep)
{
    static atomic<int32> id = 0;
    _playerInfo = boost::make_shared<DummyPlayerInfo>();
    _id = id.fetch_add(1);
}

DummySession::~DummySession()
{
    std::cout << "~DummySession" << std::endl;
}

void DummySession::SetId(int32 id)
{
    _id = id;
}

void DummySession::SetUUID(boost::uuids::uuid uuid)
{
    _uuid = uuid;
}

void DummySession::OnWait()
{
}

void DummySession::AsyncLoad()
{
    protocol::Login pkt;
    string wsId = "dummy";
    wsId += _id;
    pkt.set_text(wsId);
    pkt.set_type(_id % 2);

    SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(pkt, protocol::MessageCode::LOGIN);
    AsyncWrite(sendBuffer);
}

void DummySession::OnConnect(const boost::system::error_code& error)
{
    if (error)
    {
        // TODO : error 처리 세션 종료
    }
    else
    {
        std::cout << "OnConnect !!!" << std::endl;
        _uuid = generator();
        AsyncRead();
        AsyncLoad();
    }
}
