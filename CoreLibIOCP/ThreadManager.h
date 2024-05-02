#pragma once
#include <thread>

extern thread_local uint32 TLS_ThreadId;
extern thread_local SendBufferManager* TLS_SendBufferManager;

class ThreadManager
{
public:
    ThreadManager();
    ~ThreadManager();

    void CreateThread(Function callback);
    void ThreadTLS();
    void ThreadDestory();
    void ThreadJoinAll();

private:
    Vector<std::thread> _threadPool;
};
