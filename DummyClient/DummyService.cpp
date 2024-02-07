#include "DummyService.h"

#include "Session.h"
#include "DummySession.h"
#include "PacketHandler.h"
#include "Pawn.h"

DummyService::DummyService(boost::asio::io_context& io_context, std::string host, uint16 port)
    : Service(io_context, host, port),
      _strand(boost::asio::make_strand(io_context)),
      _timer(io_context, gTimerDelay),
      _connectTimer(io_context, gTimerDelay)
{
}

DummyService::DummyService(boost::asio::io_context& io_context, std::string host, uint16 port, int32 _maxSessionCount)
    : Service(io_context, host, port, _maxSessionCount),
      _strand(boost::asio::make_strand(io_context)),
      _timer(io_context, gTimerDelay),
      _connectTimer(io_context, gTimerDelay)
{
}

DummyService::~DummyService()
{
    std::cout << "~DummyService" << std::endl;
}

bool DummyService::Start()
{
    ConnectionTimer(0);
    return true;
}

SessionRef DummyService::CreateSession()
{
    SessionRef session = boost::make_shared<DummySession>(GetIoContext(), GetEndPoint());
    return session;
}

void DummyService::ConnectionTimer(int32 idx)
{
    if (idx < GetMaxSessionCount())
    {
        _timer.expires_at(_timer.expiry() + gTimerDelay);
        SessionRef session = CreateSession();
        AddSessionRef(session);
        session.get()->AsyncConnect();
        
        _timer.async_wait(boost::asio::bind_executor(_strand, [this, idx](boost::system::error_code error)
        {
            ConnectionTimer(idx + 1);
        }));
    }
    else
    {
        _timer.expires_at(_timer.expiry() + gTimerDelay);

        StartTimer();
    }
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
    // 위치 업데이트 초당 대략 500을 가는걸로 확인되었다. !!
    // 맵 20000 * 30000
    for (auto session : _sessions)
    {
        DummySessionRef dummySession = static_pointer_cast<DummySession>(session);
        PawnRef pawn = dummySession->GetPawn();
        if (pawn != nullptr)
        {
            if (_tick % 5 == 0)
            {
                // 5초 일때 좌표 1번 업데이트 모든 세션에 달린 pawn
                _tick = 0;
                if (pawn != nullptr)
                {
                    if (!pawn->IsUse())
                        pawn->Start();
                    pawn->SetRandRotate();
                }
            }

            // 좌표 이동 업데이트
            // rotate 방향으로 100씩 이동
            // 100이동된 좌표 구하기.

            // 튀어나가는지 체크
            if (!pawn->IsMapRange())
            {
                pawn->GetRotate().Z = static_cast<int32>(pawn->GetRotate().Z + 180) % 360;
                std::cout << "isMapRange !!!" << std::endl;
            }
            //
            pawn->GetPostion().X += (100 * cosf(pawn->GetRotate().Z * 3.14 / 180));
            pawn->GetPostion().Y += (100 * sinf(pawn->GetRotate().Z * 3.14 / 180));

            // std::cout << "id : " << dummySession->GetId() << " x : " << pawn->GetPostion().X << " y : " << pawn->GetPostion().Y << std::endl;

            // broadCast 계속 하기!!
            BS_Protocol::P_MOVE_PACKET pkt;
            pkt.Position.X = static_cast<int32>(pawn->GetPostion().X);
            pkt.Position.Y = static_cast<int32>(pawn->GetPostion().Y);
            pkt.Position.Z = static_cast<int32>(pawn->GetPostion().Z);
            pkt.Position.Yaw = static_cast<int32>(pawn->GetRotate().Z);
            SendBufferRef sendBuffer = PacketHandler::MakePacket(pkt);
            session->AsyncWrite(sendBuffer);
        }
    }
    _tick++;
    StartTimer();
}
