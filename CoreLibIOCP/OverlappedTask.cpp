#include "pch.h"
#include "OverlappedTask.h"

OverlappedTask::OverlappedTask()
{
    ZeroMemory(&_overlapped, sizeof(_overlapped));
}

void OverlappedTask::Execute()
{
    CrashFunc(f != nullptr);
    f(); 
}
