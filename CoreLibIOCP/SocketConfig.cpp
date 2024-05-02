#include "pch.h"
#include "SocketConfig.h"

LPFN_ACCEPTEX SocketConfig::lpfnAcceptEx = nullptr;
LPFN_CONNECTEX SocketConfig::lpfnConnectEx = nullptr;

SOCKET SocketConfig::CreateSocket()
{
    return WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

bool SocketConfig::SetIoCompletionPort(SOCKET socket, HANDLE iocpHd)
{
    HANDLE socketIocp = CreateIoCompletionPort(reinterpret_cast<HANDLE>(socket), iocpHd, (u_long)0, 0);
    if (socketIocp == nullptr)
    {
        closesocket(socket);
        WSACleanup();
        return false;
    }
    return true;
}

bool SocketConfig::SocketBind(SOCKET ServerSocket, int32 port)
{
    SOCKADDR_IN socketAddr;
    socketAddr.sin_family = AF_INET;
    socketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    socketAddr.sin_port = htons(static_cast<u_short>(port));

    if (bind(ServerSocket, reinterpret_cast<const SOCKADDR*>(&socketAddr), sizeof(socketAddr)) == SOCKET_ERROR)
    {
        closesocket(ServerSocket);
        WSACleanup();
        return false;
    }
    return true;
}

bool SocketConfig::SocketListen(SOCKET ServerSocket)
{
    if (listen(ServerSocket, SOMAXCONN) == SOCKET_ERROR)
        return false;
    return true;
}

bool SocketConfig::SetLinger(SOCKET socket, uint16 onoff, uint16 linger)
{
    LINGER option;
    option.l_onoff = onoff;
    option.l_linger = linger;
    return setsockopt(socket, SOL_SOCKET, SO_LINGER, reinterpret_cast<char*>(&option), sizeof(option));
}

bool SocketConfig::SetReuseAddress(SOCKET socket, int32 opt)
{
    return setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&opt), sizeof(opt));
}

bool SocketConfig::SetUpdateAcceptSocket(SOCKET socket, SOCKET serverSocket)
{
    return setsockopt(socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, reinterpret_cast<char*>(&serverSocket), sizeof(SOCKET));
}

bool SocketConfig::Init()
{
    WSADATA wsaData;
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
    {
        wprintf(L"Error at WSAStartup\n");
        return false;
    }

    SOCKET dummySocket = CreateSocket();
    DWORD dwBytes;
    GUID guid = WSAID_ACCEPTEX;
    if (WSAIoctl(dummySocket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &lpfnAcceptEx, sizeof(lpfnAcceptEx), &dwBytes, nullptr, nullptr) == SOCKET_ERROR)
    {
        printf("AcceptEx 등록 실패 : %u\n", WSAGetLastError());
        closesocket(dummySocket);
        WSACleanup();
        return false;
    }
    if (WSAIoctl(dummySocket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), &lpfnConnectEx, sizeof(lpfnConnectEx), &dwBytes, nullptr, nullptr) == SOCKET_ERROR)
    {
        printf("ConnectEx 등록 실패 : % u\n", WSAGetLastError());
        closesocket(dummySocket);
        WSACleanup();
        return false;
    }

    closesocket(dummySocket);
    return true;
}

