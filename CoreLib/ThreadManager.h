#pragma once

#include "pch.h"
extern thread_local uint32 TLS_ThreadId;
extern thread_local SendBufferManager* TLS_SendBufferManager;

class ThreadManager
{
public:
    ThreadManager();
    ~ThreadManager();

    void CreateThread(function<void()> callback);
    
    void ThreadTLS();
    void ThreadDestory();
    void ThreadJoinAll();

private:
    boost::asio::thread_pool _threadPool;
};
