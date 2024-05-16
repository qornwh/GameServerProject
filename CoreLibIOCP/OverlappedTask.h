#pragma once
// 일감 처리용 OVERLAPPED
class OverlappedTask 
{
public:
    OverlappedTask();
    void Execute();
    // 일감 처리용 function pointer
    Function f = nullptr;
private:
    WSAOVERLAPPED _overlapped;
};
