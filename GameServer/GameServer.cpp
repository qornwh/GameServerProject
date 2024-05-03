#include "pch.h"
#include "DBConnectPool.h"
#include "GameSerivce.h"
#include "IRoom.h"
#include "SendBuffer.h"
#include "ThreadManager.h"
#include "GameInit.h"
#include <Windows.h>
#include <DbgHelp.h>
#include "ItemDB.h"

#ifdef IOCPMODE
#include "SocketConfig.h"
#endif
#pragma comment(lib, "Dbghelp.lib")

long WINAPI ExceptionCallBack(EXCEPTION_POINTERS* exception_pointers)
{
    MINIDUMP_EXCEPTION_INFORMATION info = {0};
    info.ThreadId = GetCurrentThreadId(); // Threae ID 설정
    info.ExceptionPointers = exception_pointers; // Exception 정보 설정
    info.ClientPointers = FALSE;

    // 덤프 파일 생성
    std::wstring strtemp(L"GameServer.dmp");
    HANDLE hFile = CreateFile(strtemp.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL, NULL);
    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &info, NULL, NULL);

    return 0L;
}

int main()
{
    SetUnhandledExceptionFilter(ExceptionCallBack);
    std::wcout.imbue(std::locale("korean"));

    // sql 인증
    //const wchar_t* connStr = L"Driver={SQL Server};Server=127.0.0.1;Database=BSGameServerDB;Uid=qornwh;Pwd=123456;";
    // windows 인증
    const wchar_t* connStr = L"Driver={SQL Server};Server=DESKTOP-TFSEO7R\\SQLEXPRESS;Database=BSGameServerDB;Trusted_Connection=Yes;";
    GDBPool->Init(connStr);
    uint16 port = 12128;
    
#ifdef IOCPMODE
    // 게임 정보 초기화
    GameInit gInit;
    GameServiceRef service = std::make_shared<GameService>(port);
    {
        ItemDB idb;
        idb.LoadDB();
    }
    
    if (!SocketConfig::Init())
    {
        assert(-1);
    }
    
    if (!service->Start())
    {
        // crash!!
        return 0;
    }

    GThreadManager->CreateThread([&service]()
    {
        while (true)
        {
            service->task();
            GRoomManger->getRoom(0)->Tick();
        }
    });

    // 메인스레드 초기화
    TLS_ThreadId = 0;
    TLS_SendBufferManager = new SendBufferManager();
    while (true)
    {
        service->task();
        GRoomManger->getRoom(0)->Tick();
    }
    service->run();
    if (TLS_SendBufferManager != nullptr)
        delete TLS_SendBufferManager;
#else
    boost::asio::io_context io_context;
    // 게임 정보 초기화
    GameInit gInit(io_context);
    GameServiceRef service = std::make_shared<GameService>(io_context, port);
    {
        ItemDB idb;
        idb.LoadDB();
    }
    GThreadManager->CreateThread([&service]() { service->run(); });
    GThreadManager->CreateThread([&service]() { service->run(); });
    GThreadManager->CreateThread([&service]() { service->run(); });
    
    if (!service->Start())
    {
        // crash!!
        return 0;
    }

    // 메인스레드 초기화
    TLS_ThreadId = 0;
    TLS_SendBufferManager = new SendBufferManager();
    service->run();
    if (TLS_SendBufferManager != nullptr)
        delete TLS_SendBufferManager;
#endif

    return 0;
}
