#include <iostream>
#include <boost/make_shared.hpp>
#include "DummyService2.h"
#include "ThreadManager.h"
using DummyServiceRef = boost::shared_ptr<DummyService>;

int main()
{
    auto count = std::thread::hardware_concurrency() * 2;
    std::cout << "core : " << count << std::endl;
    boost::asio::io_context io_context;

    std::string address = "127.0.0.1";
    uint16 port = 12128;

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
    DummyServiceRef service = boost::make_shared<DummyService>(io_context, address, port, 3);

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
    return 0;
}
