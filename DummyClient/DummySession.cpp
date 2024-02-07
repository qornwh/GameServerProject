#include "DummySession.h"

#include "PacketHandler.h"
#include "UUIDGen.h"
#include <boost/make_shared.hpp>

#include "Pawn.h"

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

void DummySession::AsyncTestMessage(const boost::system::error_code& ec)
{
    // 메시지 만들고 async_write
    // Protocol::TestPlayerInfo info;
    // info.set_type(Protocol::Type::TESTPLAYER);
    // info.set_id(Protocol::Type::TESTPLAYER);
    // info.set_type(Protocol::Type::TESTPLAYER);
}

void DummySession::AsyncLoad()
{
    BS_Protocol::P_LOGIN_PAKCET pkt;
    std::wstring wsId = L"dummy";
    wsId += std::to_wstring(_id);

    memcpy(pkt.Id.data(), wsId.data(), wsId.size() * 2);
    pkt.IdLen = wsId.size() * 2;
    pkt.Type = _id % 3;
    cout << "id : " << _id % 3 << " type : " << pkt.Type << endl;
    SendBufferRef sendBuffer = PacketHandler::MakePacket(pkt);
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
