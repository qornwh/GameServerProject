#include "DummyService.h"

#include "Session.h"
#include "DummySession.h"
#include "PacketHandler.h"
#include "DummyPlayerInfo.h"
#include "DummyProto.pb.h"
#include "GamePacketHandler.h"

DummyService::DummyService(boost::asio::io_context& io_context, std::string host, uint16 port)
    : Service(io_context, host, port),
      _strand(boost::asio::make_strand(io_context)),
      _timer(io_context, gTimerDelay)
{
}

DummyService::DummyService(boost::asio::io_context& io_context, std::string host, uint16 port, int32 _maxSessionCount)
    : Service(io_context, host, port, _maxSessionCount),
      _strand(boost::asio::make_strand(io_context)),
      _timer(io_context, gTimerDelay)
{
}

DummyService::~DummyService()
{
    std::cout << "~DummyService" << std::endl;
}

bool DummyService::Start()
{
    for (int32 i = 0; i < GetMaxSessionCount(); i++)
    {
        boost::core::detail::Sleep(300);
        SessionRef session = CreateSession();
        AddSessionRef(session);
        if (!session.get()->AsyncConnect())
        {
            return false;
        }
    }
    boost::core::detail::Sleep(300);
    StartTimer();
    return true;
}

SessionRef DummyService::CreateSession()
{
    SessionRef session = boost::make_shared<DummySession>(GetIoContext(), GetEndPoint());
    return session;
}

void DummyService::StartTimer()
{
    _timer.expires_at(_timer.expiry() + gTimerDelay);

    _timer.async_wait(boost::asio::bind_executor(_strand, [this](boost::system::error_code error)
    {
        AsyncSession();
    }));
}

void DummyService::AsyncSession()
{
    for (auto session : _sessions)
    {
        DummySessionRef dummySession = static_pointer_cast<DummySession>(session);
        DummyPlayerInfoRef info = dummySession->GetInfo();
        if (info != nullptr)
        {
            if (_tick % 3 == 0)
            {
                _tick = 0;
                if (info != nullptr)
                {
                    if (!info->IsUse())
                        info->Start();
                    info->UpdateRotate();
                }
            }

            // 좌표 이동 업데이트
            info->updatePosition();

            #if AMODE == 0
            // broadCast 계속 하기!!
            BS_Protocol::P_MOVE_PACKET pkt;
            pkt.Position.X = static_cast<int32>(info->GetPostion().X);
            pkt.Position.Y = static_cast<int32>(info->GetPostion().Y);
            pkt.Position.Z = static_cast<int32>(info->GetPostion().Z);
            pkt.Position.Yaw = static_cast<int32>(info->GetPostion().Yaw);
            SendBufferRef sendBuffer = PacketHandler::MakePacket(pkt);
#elif AMODE == 1
            protocol::SMove pkt;
            pkt.set_is_monster(false);
            protocol::Position *position = new protocol::Position();
            position->set_x(info->GetPostion().Y);
            position->set_z(info->GetPostion().X);
            position->set_yaw(info->GetPostion().Yaw);
            pkt.set_allocated_position(position);
            
            SendBufferRef sendBuffer = GamePacketHandler::MakePacketHandler(pkt, protocol::MessageCode::S_MOVE);
#endif
            session->AsyncWrite(sendBuffer);
        }
    }
    _tick++;
    StartTimer();
}
