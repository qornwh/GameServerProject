#pragma once
#include "pch.h"

#include "RecvBuffer.h"
#include "Service.h"

class Session : public boost::enable_shared_from_this<Session>
{
public:
    Session(boost::asio::io_context& io_context, const boost::asio::ip::tcp::endpoint& ep);
    ~Session();

    bool AsyncConnect();
    void AsyncRead();
    void AsyncWrite();
    void AsyncWrite(void* data, std::size_t size_in_bytes);
    void AsyncWrite(SendBufferRef sendBuffer);
    virtual void OnConnect(const boost::system::error_code& err);
    virtual int32 OnRecv(BYTE* buffer, int32 len);
    void OnRead(const boost::system::error_code err, const size_t bytes_transferred);
    void OnWrite(const boost::system::error_code err, const size_t bytes_transferred);
    void Connect();
    void Disconnect();
    void AddWriteBuffer(SendBufferRef sendBuffer);

    void SetService(ServiceRef service) { _serviceRef = service; }
    ServiceRef GetService() { return _serviceRef.lock(); }
	bool IsConnected() { return _connected; }

    boost::asio::ip::tcp::socket& GetSocket() { return _socket; }
    boost::shared_ptr<Session> getSharePtr() { return shared_from_this(); }

private:
    boost::asio::ip::tcp::endpoint _ep;
    boost::asio::ip::tcp::socket _socket;

    RecvBuffer _recvBuffer;
    vector<SendBufferRef> _sendBuffers;

    boost::weak_ptr<Service> _serviceRef;

    Lock lock;

    char _TESTrecvBuffer[4096];
	Atomic<bool> _connected{ false };
};
