﻿#include <boost/make_shared.hpp>

#include "DBConnectPool.h"
#include "pch.h"
#include "GameSerivce.h"
#include "IRoom.h"
#include "SendBuffer.h"
#include "ThreadManager.h"
#include "GameInit.h"

#ifdef _WIN32
#include <Windows.h>
#include <DbgHelp.h>
#include "ItemDB.h"
#pragma comment(lib, "Dbghelp.lib")
long WINAPI ExceptionCallBack(EXCEPTION_POINTERS* exception_pointers)
{
    MINIDUMP_EXCEPTION_INFORMATION info = {0};
    info.ThreadId = ::GetCurrentThreadId(); // Threae ID 설정
    info.ExceptionPointers = exception_pointers; // Exception 정보 설정
    info.ClientPointers = FALSE;

    // 덤프 파일 생성
    std::wstring strtemp(L"GameServer.dmp");
    HANDLE hFile = CreateFile(strtemp.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL, NULL);
    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &info, NULL, NULL);

    return 0L;
}
#endif

int main()
{
#ifdef _WIN32
    SetUnhandledExceptionFilter(ExceptionCallBack);

    std::wcout.imbue(std::locale("kor"));
    std::wcin.imbue(std::locale("kor"));
#endif

    // sql 인증
    //const wchar_t* connStr = L"Driver={SQL Server};Server=127.0.0.1;Database=BSGameServerDB;Uid=qornwh;Pwd=123456;";
    // windows 인증
    const wchar_t* connStr = L"Driver={SQL Server};Server=DESKTOP-TFSEO7R\\SQLEXPRESS;Database=BSGameServerDB;Trusted_Connection=Yes;";
    GDBPool->Init(connStr);

    auto count = std::thread::hardware_concurrency() * 2;
    std::cout << "core : " << count << std::endl;
    boost::asio::io_context io_context;
    uint16 port = 12128;

    // 게임 정보 초기화
    GameInit gInit(io_context);
    GameServiceRef service = std::make_shared<GameService>(io_context, port);
    {
        ItemDB idb;
        idb.LoadDB();
    }

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
