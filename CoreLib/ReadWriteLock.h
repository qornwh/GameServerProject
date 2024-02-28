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
