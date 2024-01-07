#include "DummyService.h"

#include "Session.h"
#include "DummySession.h"
#include "PacketHandler.h"
#include "Pawn.h"

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
    // 위치 업데이트 초당 대략 500을 가는걸로 확인되었다. !!
    // 맵 20000 * 30000
    for (auto session : _sessions)
    {
        DummySessionRef dummySession = reinterpret_pointer_cast<DummySession>(session);
        PawnRef pawn = dummySession->GetPawn();
        if (pawn != nullptr)
        {
            if (_tick % 12 == 0)
            {
                // 3 초 일때 좌표 1번 업데이트 모든 세션에 달린 pawn
                _tick = 0;
                if (pawn != nullptr)
                {
                    if (!pawn->isUse())
                        pawn->Start();
                    pawn->SetRandRotate();
                }
            }

            // 좌표 이동 업데이트
            // rotate 방향으로 100씩 이동
            // 100이동된 좌표 구하기.
            {
                // 튀어나가는지 체크
                if (!pawn->isMapRange())
                {
                    pawn->GetRotate().X = -1 * pawn->GetRotate().X;
                    pawn->GetRotate().Y = -1 * pawn->GetRotate().Y;
                    std::cout << "isMapRange !!!" << std::endl;
                }
                //
                float _theta = atan2f(pawn->GetRotate().Y, pawn->GetRotate().X);
                pawn->GetPostion().X += (100 * cosf(_theta));
                pawn->GetPostion().Y += (100 * sinf(_theta));
            }

            std::cout << "id : " << dummySession->GetId() << " x : " << pawn->GetPostion().X << " y : " << pawn->GetPostion().Y << std::endl;
            if (pawn->GetPostion().X < 0 || pawn->GetPostion().Y < 0)
                std::cout << "nonono!!!" << std::endl;

            // broadCast 계속 하기!!
            BS_Protocol::P_MOVE_PACKET pkt;
            pkt.Position.X = static_cast<int32>(pawn->GetPostion().X);
            pkt.Position.Y = static_cast<int32>(pawn->GetPostion().Y);
            pkt.Position.Z = static_cast<int32>(pawn->GetPostion().Z);
            SendBufferRef sendBuffer = PacketHandler::MakePacket(pkt);
            session->AsyncWrite(sendBuffer);
        }
    }
    _tick++;
    StartTimer();
}
