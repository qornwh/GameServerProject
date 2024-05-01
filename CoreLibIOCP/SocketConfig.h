#pragma once
#include "pch.h"

class SocketConfig
{
public:
    static SOCKET CreateSocket();
    static bool SetIoCompletionPort(SOCKET socket, HANDLE iocpHd);
    static bool SocketBind(SOCKET ServerSocket);
    static bool SocketListen(SOCKET ServerSocket);

    // ���� �������� ���� ���� �������� ����
    static bool SetLinger(SOCKET socket, unsigned int onoff, unsigned int linger);
    // ���� ������ �ٷ� ����ɼ� �ֵ��� ����
    static bool SetReuseAddress(SOCKET socket, int opt);
    // ListenSocket�� Ư���� ClientSocket�� �״�� �̾����.
    static bool SetUpdateAcceptSocket(SOCKET socket, SOCKET serverSocket);
    // 
    static bool Init();

    static LPFN_ACCEPTEX lpfnAcceptEx;
    static LPFN_CONNECTEX lpfnConnectEx;
};

