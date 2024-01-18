#include <boost/make_shared.hpp>

#include "GameGlobal.h"
#include "GameRoomManager.h"
#include "pch.h"
#include "GameSerivce.h"
#include "ThreadManager.h"

int main()
{
    auto count = std::thread::hardware_concurrency() * 2;
    std::cout << "core : " << count << std::endl;
    boost::asio::io_context io_context;

    uint16 port = 12128;

    // 일반몹 맵 = 0
    GRoomManger->CreateRoom(io_context, 0);
    // 보스몹 맵 = 1
    GRoomManger->CreateRoom(io_context, 1);

    GameServiceRef service = boost::make_shared<GameService>(io_context, port);

    GThreadManager->CreateThread([&io_context, &port]()
        {
            GameServiceRef service = boost::make_shared<GameService>(io_context, port);
            if (!service->Start())
            {
                // crash!!
                return;
            }
            service->run();
        }
    );

    if (!service->Start())
    {
        // crash!!
        return 0;
    }
    service->run();

    return 0;
}
