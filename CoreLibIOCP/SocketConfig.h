#pragma once
#include "pch.h"

class SocketConfig
{
public:
    static SOCKET CreateSocket();
    static bool SetIoCompletionPort(SOCKET socket, HANDLE iocpHd);
    static bool SocketBind(SOCKET ServerSocket);
    static bool SocketListen(SOCKET ServerSocket);

    // 소켓 연결끊기면 남은 버퍼 전달할지 말지
    static bool SetLinger(SOCKET socket, unsigned int onoff, unsigned int linger);
    // 소켓 꺼지고 바로 연결될수 있도록 설정
    static bool SetReuseAddress(SOCKET socket, int opt);
    // ListenSocket의 특성을 ClientSocket에 그대로 이어받음.
    static bool SetUpdateAcceptSocket(SOCKET socket, SOCKET serverSocket);
    // 
    static bool Init();

    static LPFN_ACCEPTEX lpfnAcceptEx;
    static LPFN_CONNECTEX lpfnConnectEx;
};

