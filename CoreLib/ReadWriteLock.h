#pragma once
#include <shared_mutex>

class Lock : public std::shared_mutex
{
public:
private:
};

class ReadLockGuard
{
public:
    ReadLockGuard(Lock& lock, const char* name) : _readLock(lock), _name(name)
    {
    }

    ReadLockGuard(std::shared_mutex& lock, const char* name) : _readLock(lock), _name(name)
    {
    }

    ~ReadLockGuard()
    {
    }

private:
    std::shared_lock<std::shared_mutex> _readLock;
    const char* _name;
};

class WriteLockGuard
{
public:
    WriteLockGuard(Lock& lock, const char* name) : _writeLock(lock), _name(name)
    {
    }

    WriteLockGuard(std::shared_mutex& lock, const char* name) : _writeLock(lock), _name(name)
    {
    }

    ~WriteLockGuard()
    {
    }

private:
    std::unique_lock<std::shared_mutex> _writeLock;
    const char* _name;
};

/*
// 강좌 락
#include <atomic>

using namespace std;

class Lock
{
    enum : uint32_t
    {
        MAX_SPIN_COUNT = 5000,
        WRITE_THREAD_MASK = 0xFFFF'0000,
        READ_COUNT_MASK = 0x0000'FFFF,
        EMPTY_FLAG = 0x0000'0000
    };
    
public:
    void ReadLock();
    void ReadUnLock();
    void WriteLock();
    void WriteUnLock();

private:
    atomic<int> readCount;
    uint16_t _writeCount;
    atomic<uint32_t> _lockFlag;
};

// ---------------------
//    LockGuards
// --------------------- 

class ReadLockGuard
{
public:
    ReadLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name)
    {
        _lock.ReadLock();
    }
    ~ReadLockGuard()
    {
        _lock.ReadUnLock();
    }
private:
    Lock& _lock;
    const char* _name;
};

class WriteLockGuard
{
public:
    WriteLockGuard(Lock& lock, const char* name) : _lock(lock), _name(name) { _lock.WriteLock(); }
    ~WriteLockGuard() { _lock.WriteUnLock(); }
private:
    Lock& _lock;
    const char* _name;
};
*/