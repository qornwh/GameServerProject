#include "pch.h"
#include "Session.h"
#include <boost/asio/buffer.hpp>
#include <boost/asio/write.hpp>

#include "SendBuffer.h"

Session::Session(boost::asio::io_context& io_context,
                 const boost::asio::ip::tcp::endpoint& ep) : _ep(ep), _socket(io_context), _recvBuffer(4096)
{
}

Session::~Session()
{
}

bool Session::AsyncConnect()
{
    _socket.async_connect(_ep, [this](const boost::system::error_code& ec)
    {
        this->OnConnect(ec);
    });
    return true;
}

void Session::OnConnect(const boost::system::error_code& err)
{
    if (err)
    {
        std::cout << "OnConnect error code: " << err.value() << ", msg: " << err.message() << std::endl;
    }
    else
    {
        std::cout << "OnConnect !!!" << std::endl;
        Connect();
        AsyncRead();
    }
}

int32 Session::OnRecv(BYTE* buffer, int32 len)
{
    return len;
}

void Session::AsyncRead()
{
    const uint64 _recvBufferSize = static_cast<uint64>(_recvBuffer.FreeSize());
    BYTE* _buffer = _recvBuffer.ReadPos();
    auto ptr = shared_from_this();
    _socket.async_read_some(
        // 이거 데이터 한계가 있다. => 아니다 size_t 자료형 크기다 ㅅㅂ 와 이거 1시간 날림
        boost::asio::buffer(_buffer, _recvBufferSize),
        [ptr](const boost::system::error_code ec, const size_t bytes_transferred)
        {
            ptr->OnRead(ec, bytes_transferred);
        }
    );
}

void Session::OnRead(const boost::system::error_code err, size_t len)
{
    // 컨텐츠단 구성 필요
    if (!err)
    {
        // TODO : recv 처리
        if (len == 0)
        {
            Disconnect();
        }
        else
        {
            if (!_recvBuffer.OnWrite(static_cast<int32>(len)))
            {
                Disconnect();
                return;
            }

            int32 dataSize = _recvBuffer.DataSize();

            BYTE* buffer = _recvBuffer.ReadPos();
            int32 processLen = OnRecv(buffer, dataSize);
            if (!_recvBuffer.OnRead(processLen) || processLen <= 0 || dataSize < processLen)
            {
                Disconnect();
                return;
            }
            _recvBuffer.Clean();

            AsyncRead();
        }
    }
    else
    {
        // TODO : error 처리
        std::cout << "OnRead error code: " << err.value() << ", msg: " << err.message() << std::endl;
        Disconnect();
    }
}

void Session::AsyncWrite()
{
    std::vector<boost::asio::const_buffer> buffers;
    {
        WriteLockGuard wl(lock, "write");
        for (auto buffer : _sendBuffers)
        {
            buffers.emplace_back(boost::asio::buffer(buffer->Buffer(), buffer->WriteSize()));
        }
    }
    auto ptr = shared_from_this();
    boost::asio::async_write(
        _socket,
        buffers,
        [ptr](const boost::system::error_code& ec, const size_t& bytes_transferred)
        {
            ptr->OnWrite(ec, bytes_transferred);
            WriteLockGuard wl(ptr->lock, "write");
            ptr->_sendBuffers.clear();
            // std::cout << "sendbuffer size !!! : " << bytes_transferred << std::endl;
            // 세션 shared_ptr관리 필요?? disconnected될때 끊길 확률 있다
        });
}

void Session::AsyncWrite(void* data, std::size_t len)
{
    auto ptr = shared_from_this();
    _socket.async_write_some(
        boost::asio::buffer(data, len),
        [ptr](const boost::system::error_code& ec, const size_t& len)
        {
            ptr->OnWrite(ec, len);
        });
}

void Session::AsyncWrite(SendBufferRef sendBuffer)
{
    std::vector<boost::asio::const_buffer> buffers;
    {
        WriteLockGuard wl(lock, "write");
        _sendBuffers.emplace_back(sendBuffer);
        for (auto buffer : _sendBuffers)
        {
            buffers.emplace_back(boost::asio::buffer(buffer->Buffer(), buffer->WriteSize()));
        }
    }
    auto ptr = shared_from_this();
    boost::asio::async_write(
        _socket,
        buffers,
        [ptr](const boost::system::error_code& ec, const size_t& len)
        {
            ptr->OnWrite(ec, len);
            WriteLockGuard wl(ptr->lock, "write");
            ptr->_sendBuffers.clear();
        });
}

void Session::OnWrite(const boost::system::error_code err, const size_t len)
{
    if (!err)
    {
        // TODO : recv 처리
    }
    else
    {
        // TODO : error 처리
        std::cout << "OnWrite error code: " << err.value() << " msg: " << err.message() << std::endl;
    }
}

void Session::Connect()
{
    _connected.store(true);
}

void Session::Disconnect()
{
    _recvBuffer.Clean();
    _sendBuffers.clear();
    _connected.exchange(false);

    if (_serviceRef.lock() != nullptr)
    {
        _serviceRef.lock()->ReleaseSession(shared_from_this());
    }
}

void Session::AddWriteBuffer(SendBufferRef sendBuffer)
{
    WriteLockGuard wl(lock, "write");
    _sendBuffers.emplace_back(sendBuffer);
}
