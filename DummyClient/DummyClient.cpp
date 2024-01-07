#include <iostream>
#include <boost/make_shared.hpp>
#include "DummyService.h"
#include "ThreadManager.h"
#include "test.pb.h"
using DummyServiceRef = boost::shared_ptr<DummyService>;

int main()
{
    auto count = std::thread::hardware_concurrency() * 2;
    std::cout << "core : " << count << std::endl;
    boost::asio::io_context io_context;

    std::string address = "172.30.1.53";
    // std::string address = "127.0.0.1";
    uint16 port = 12127;
    // uint16 port = 12128;

    GThreadManager->CreateThread([&io_context, &address, &port]()
        {
            DummyServiceRef service = boost::make_shared<DummyService>(io_context, address, port, 3);
            if (!service->Start())
            {
                // crash!!
                return;
            }
            service->run();
        }
    );
    
    boost::core::detail::Sleep(300 * 4);
    DummyServiceRef service = boost::make_shared<DummyService>(io_context, address, port, 1);

    ThreadManager tm;
    tm.ThreadTLS();
    if (!service->Start())
    {
        // crash!!
        return 0;
    }
    service->run();

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
