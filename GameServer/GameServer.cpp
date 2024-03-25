#include <boost/make_shared.hpp>

#include "GameGlobal.h"
#include "GameInit.h"
#include "GameRoomManager.h"
#include "pch.h"
#include "GameSerivce.h"
#include "IRoom.h"
#include "SendBuffer.h"
#include "ThreadManager.h"

#ifdef _WIN32
#include <Windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "Dbghelp.lib")
long WINAPI ExceptionCallBack(EXCEPTION_POINTERS* exception_pointers)
{
    MINIDUMP_EXCEPTION_INFORMATION info = { 0 };
    info.ThreadId = ::GetCurrentThreadId(); // Threae ID 설정
    info.ExceptionPointers = exception_pointers; // Exception 정보 설정
    info.ClientPointers = FALSE;

    // 덤프 파일 생성
    std::wstring strtemp(L"GameServer.dmp");
    HANDLE hFile = CreateFile(strtemp.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &info, NULL, NULL);

    return 0L;
}
#endif

void test()
{
    char* ptr = nullptr;
    ptr[5] = 'a';
}

int main()
{
#ifdef _WIN32
    SetUnhandledExceptionFilter(ExceptionCallBack);
#endif

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
