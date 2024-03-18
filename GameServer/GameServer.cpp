﻿#include <boost/make_shared.hpp>

#include "GameGlobal.h"
#include "GameInit.h"
#include "GameRoomManager.h"
#include "pch.h"
#include "GameSerivce.h"
#include "IRoom.h"
#include "SendBuffer.h"
#include "ThreadManager.h"

int main()
{
    auto count = std::thread::hardware_concurrency() * 2;
    std::cout << "core : " << count << std::endl;
    boost::asio::io_context io_context;
    uint16 port = 12128;

    // 스킬 초기화
    GameInit();
    // 일반몹 맵 = 0
    GRoomManger->CreateRoom(io_context, 0);
    GRoomManger->getRoom(0)->InitMonsters();
    // 보스몹 맵 = 1
    GRoomManger->CreateRoom(io_context, 1);
    GRoomManger->getRoom(1)->InitMonsters();

    GameServiceRef service = boost::make_shared<GameService>(io_context, port);

    if (!service->Start())
    {
        // crash!!
        return 0;
    }

    GThreadManager->CreateThread([&service]()
        {
            service->run();
        }
    );

    GThreadManager->CreateThread([&service]()
        {
            service->run();
        }
    );

    GThreadManager->CreateThread([&service]()
        {
            service->run();
        }
    );

    // 메인스레드 초기화
    TLS_ThreadId = 0;
    TLS_SendBufferManager = new SendBufferManager();
    service->run();
    if (TLS_SendBufferManager != nullptr)
        delete TLS_SendBufferManager;

    return 0;
}
