#pragma once

class SocketConfig
{
public:
    static SOCKET CreateSocket();
    static bool SetIoCompletionPort(SOCKET socket, HANDLE iocpHd);
    static bool SocketBind(SOCKET ServerSocket, int32 port);
    static bool SocketListen(SOCKET ServerSocket);

    // 소켓 연결끊기면 남은 버퍼 전달할지 말지
    static bool SetLinger(SOCKET socket, uint16 onoff, uint16 linger);
    // 소켓 꺼지고 바로 연결될수 있도록 설정
    static bool SetReuseAddress(SOCKET socket, int32 opt);
    // ListenSocket의 특성을 ClientSocket에 그대로 이어받음.
    static bool SetUpdateAcceptSocket(SOCKET socket, SOCKET serverSocket);
    // 소켓 초기화, 비동기 함수 등록
    static bool Init();

    static LPFN_ACCEPTEX lpfnAcceptEx;
    static LPFN_CONNECTEX lpfnConnectEx;
};

