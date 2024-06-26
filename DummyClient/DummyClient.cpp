﻿#include "pch.h"
#include "DummyService.h"
#include "ThreadManager.h"

using DummyServiceRef = std::shared_ptr<DummyService>;

int main()
{
    boost::asio::io_context io_context;

#if AMODE == 0
    std::string address = "172.30.1.53";
    uint16 port = 12127;
#elif AMODE == 1 
    std::string address = "127.0.0.1";
    uint16 port = 12128;
#endif

    GThreadManager->CreateThread([&io_context, &address, &port]()
        {
            DummyServiceRef service = std::make_shared<DummyService>(io_context, address, port, 100);
            if (!service->Start())
            {
                // crash!!
                return;
            }
            service->run();
        }
    );
    
    // boost::core::detail::Sleep(1000 * 10);
    // DummyServiceRef service = std::make_shared<DummyService>(io_context, address, port, 10);
    //
    // ThreadManager tm;
    // tm.ThreadTLS();
    // if (!service->Start())
    // {
    //     // crash!!
    //     return 0;
    // }
    // service->run();

    while (true)
    {
    }
    // 1. 쓰레드 생성
    // - ip port crud용 클래스 생성
    // 
    // 2. 세션 200개 등록
    // 3. 세션 개수 / 스레드 개수 = n개씩 스레드에 할당. ex)스레드 4개 세션 200개 || 1스레드 (1 ~ 50) 2스레드 (51 ~ 100) 3..
    //
    return 0;
}
