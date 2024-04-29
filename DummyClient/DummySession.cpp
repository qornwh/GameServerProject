#include "DummySession.h"
#include "PacketHandler.h"
#include "DummyPlayerInfo.h"
#include "DummyProto.pb.h"
#include "GamePacketHandler.h"

DummySession::DummySession(boost::asio::io_context& io_context, const boost::asio::ip::tcp::endpoint& ep)
    : Session(io_context, ep)
{
    static atomic<int32> id = 0;
    _playerInfo = boost::make_shared<DummyPlayerInfo>();
    _id = id.fetch_add(1);

    _playerInfo->Start();
}

DummySession::~DummySession()
{
    std::cout << "~DummySession" << std::endl;
}

void DummySession::SetId(int32 id)
{
    _id = id;
}

void DummySession::OnWait()
{
}

void DummySession::AsyncLoad()
{
#if AMODE == 0
    BS_Protocol::P_LOGIN_PAKCET pkt;
    std::wstring wsId = L"dummy";
    wsId += std::to_wstring(_id);

    memcpy(pkt.Id.data(), wsId.data(), wsId.size() * 2);
    pkt.IdLen = wsId.size() * 2;
    pkt.Type = _id % 3;
    cout << "id : " << _id % 3 << " type : " << pkt.Type << endl;
    SendBufferRef sendBuffer = PacketHandler::MakePacket(pkt);
#elif AMODE == 1
    protocol::Login pkt;
    string wsId = "dummy";
    wsId += std::to_string(_id);
    pkt.set_text(wsId);
    pkt.set_type(_id % 2);
    pkt.set_is_dummy(true);
    protocol::Position* position = new protocol::Position();
    position->set_x(_playerInfo->GetPostion().Y);
    position->set_z(_playerInfo->GetPostion().X);
    position->set_yaw(_playerInfo->GetPostion().Yaw);
    pkt.set_allocated_position(position);

    SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(pkt, protocol::MessageCode::LOGIN);
#endif
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
        AsyncRead();
        AsyncLoad();
    }
}
