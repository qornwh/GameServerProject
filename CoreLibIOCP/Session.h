#pragma once
#include "RecvBuffer.h"
#include "OverlappedSocket.h"
#include "Service.h"

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(EndPointUtil& ep);
    ~Session();

    void Init();
    void Connect();
    void AsyncConnect(OverlappedSocket* overlappedPtr);
    void OnConnect();
    void AsyncRead();
    void OnRead(int32 len);
    virtual int32 OnRecv(BYTE* buffer, int32 len);
    void AsyncWrite(SendBufferRef sendBuffer);
    void OnWrite(int32 len);
    void Disconnect();
    void AddWriteBuffer(SendBufferRef sendBuffer);
    static void ErrorCode(int32 errorCode);

    void SetService(ServiceRef service) { _serviceRef = service; }
    ServiceRef GetService() { return _serviceRef.lock(); }
	bool IsConnected() { return _connected; }
    SessionRef getSharePtr() { return shared_from_this(); }
    SOCKET GetSocket() { return _socket; }

private:
    SOCKET _socket = INVALID_SOCKET;
    RecvBuffer _recvBuffer;
    Vector<SendBufferRef> _waitBuffers; // 대기용 sendBuffer
    Vector<SendBufferRef> _sendBuffers; // 보내지고 있는 sendBuffer
    std::weak_ptr<Service> _serviceRef;

    Lock lock;
	Atomic<bool> _connected{ false };
    SOCKADDR_IN _serverAddr;

    OverlappedSocket _recvOLS;
    OverlappedSocket _sendOLS;
    EndPointUtil _ep;
};
