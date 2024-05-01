#pragma once
#include "pch.h"
#include "RecvBuffer.h"
#include "Service.h"

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(boost::asio::io_context& ioContext, const boost::asio::ip::tcp::endpoint& ep);
    ~Session();

    bool AsyncConnect();
    void AsyncRead();
    void AsyncWrite();
    void AsyncWrite(void* data, std::size_t len);
    void AsyncWrite(SendBufferRef sendBuffer);
    virtual void OnConnect(const boost::system::error_code& err);
    virtual int32 OnRecv(BYTE* buffer, int32 len);
    void OnRead(const boost::system::error_code err, const size_t len);
    void OnWrite(const boost::system::error_code err, const size_t len);
    void Connect();
    void Disconnect();
    void AddWriteBuffer(SendBufferRef sendBuffer);

    void SetService(ServiceRef service) { _serviceRef = service; }
    ServiceRef GetService() { return _serviceRef.lock(); }
	bool IsConnected() { return _connected; }

    boost::asio::ip::tcp::socket& GetSocket() { return _socket; }
    SessionRef getSharePtr() { return shared_from_this(); }

private:
    boost::asio::ip::tcp::endpoint _ep;
    boost::asio::ip::tcp::socket _socket;

    RecvBuffer _recvBuffer;
    Vector<SendBufferRef> _sendBuffers;
    std::weak_ptr<Service> _serviceRef;

    Lock lock;
	Atomic<bool> _connected{ false };
};
