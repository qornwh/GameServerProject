#include "pch.h"
#include "ReadWriteLock.h"

/*
#include "LockUtils.h"
#include "CoreTLS.h"

void Lock::ReadLock()
{
    // 같은 스레드일때 동기적이므로 통과
    const uint32_t lockThreadId = (_lockFlag & WRITE_THREAD_MASK) >> 16;
    if (LThreadId == lockThreadId)
    {
        _lockFlag.fetch_add(1);
        return;
    }

    // 다른 스레드
    while (true)
    {
        for (int i = 0; i < MAX_SPIN_COUNT; i++)
        {
            uint32_t expected = _lockFlag.load() & READ_COUNT_MASK;
            if (_lockFlag.compare_exchange_strong(expected, expected + 1))
            {
                std::cout << "read Clear" << endl;
                return;
            }
        }

        // To-do crash 타임아웃

        this_thread::yield();
    }
}

void Lock::ReadUnLock()
{
    if ((_lockFlag.fetch_sub(1) & READ_COUNT_MASK) == 0)
        // CRASH("MULTIPLE_UNLOCK");
        std::cout << "CRASH : MULTIPLE_UNLOCK" << std::endl;
}

void Lock::WriteLock()
{
    // 같은 스레드일때 동기적이므로 통과
    const uint32_t lockThreadId = (_lockFlag & WRITE_THREAD_MASK) >> 16;
    if (LThreadId == lockThreadId)
    {
        _writeCount++;
        return;
    }

    // 다른 스레드일때 동기화
    const uint32_t desired = ((LThreadId << 16) & WRITE_THREAD_MASK);
    while (true)
    {
        for (int i = 0; i < MAX_SPIN_COUNT; i++)
        {
            uint32_t expected = EMPTY_FLAG;
            if (_lockFlag.compare_exchange_strong(expected, desired))
            {
                std::cout << "write Clear" << endl;
                _writeCount++;
                return;
            }
        }

        // To-do crash 타임아웃

        this_thread::yield();
    }
}

void Lock::WriteUnLock()
{
    // ReadLock 다 풀기 전에는 WriteUnlock 불가능.
    if ((_lockFlag.load() & READ_COUNT_MASK) != 0)
        // CRASH("INVALID_UNLOCK_ORDER");
        std::cout << "CRASH : INVALID_UNLOCK_ORDER" << std::endl;

    const int32_t lockCount = --_writeCount;
    if (lockCount == 0)
        _lockFlag.store(EMPTY_FLAG);
}
*/

