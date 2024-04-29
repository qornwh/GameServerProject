#include "pch.h"
#include "ThreadManager.h"
#include "SendBuffer.h"

thread_local uint32 TLS_ThreadId = 0;
thread_local SendBufferManager* TLS_SendBufferManager = nullptr;

ThreadManager::ThreadManager()
{
}

ThreadManager::~ThreadManager()
{
    ThreadJoinAll();
}

void ThreadManager::CreateThread(Function callback)
{
    boost::asio::post(_threadPool, [=]()
    {
        ThreadTLS();
        callback();
        ThreadDestory();
    });
}

void ThreadManager::ThreadTLS()
{
    static Atomic<uint32> SThreadId = 1;
    TLS_ThreadId = SThreadId.fetch_add(1);
    TLS_SendBufferManager = new SendBufferManager();
}

void ThreadManager::ThreadDestory()
{
    if (TLS_SendBufferManager != nullptr)
        delete TLS_SendBufferManager;
}

void ThreadManager::ThreadJoinAll()
{
    _threadPool.join();
}
