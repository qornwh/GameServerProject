#pragma once
#include "pch.h"
#include "Packet.h"
#include "PacketHeader.h"
#include "SendBuffer.h"
#include "ThreadManager.h"

class BufferWrite
{
public:
    BufferWrite()
    {
    }

    BufferWrite(BYTE* buffer, uint16 size, uint16 pos = 0) : _buffer(buffer), _size(size), _pos(pos)
    {
    }

    ~BufferWrite()
    {
    }

public:
    BYTE* Buffer() { return _buffer; }
    uint16 Size() { return _size; }
    uint16 WriteSize() { return _pos; }
    uint16 FreeSize() { return _size - _pos; }

public:
    template <typename T>
    bool Write(T* src) { return Write(src, sizeof(T)); }

    bool Write(void* src, uint16 len)
    {
        if (FreeSize() < len)
            return false;

        memcpy(&_buffer[_pos], src, len);
        _pos += len;
        return true;
    }

private:
    BYTE* _buffer = nullptr;
    uint16 _size = 0;
    uint16 _pos = 0;
};

class PacketHandler
{
public:
    static SendBufferRef MakePacket(BS_Protocol::P_LOGIN_PAKCET& pkt)
    {
        const uint16 dataSize = pkt.Size();
        SendBufferRef sendBuffer = MakeSendBuffer(1, dataSize);

        BufferWrite bw(sendBuffer->Buffer() + sizeof(PacketHeader), dataSize);
        bw.Write(&pkt.Type);
        bw.Write(&pkt.IdLen);
        bw.Write(pkt.Id.data(), pkt.IdLen);

        return sendBuffer;
    }
    
    static SendBufferRef MakePacket(BS_Protocol::P_MOVE_PACKET& pkt)
    {
        const uint16 dataSize = pkt.Size();
        SendBufferRef sendBuffer = MakeSendBuffer(3, dataSize);

        BufferWrite bw(sendBuffer->Buffer() + sizeof(PacketHeader), dataSize);
        bw.Write(&pkt.Position.X);
        bw.Write(&pkt.Position.Y);
        bw.Write(&pkt.Position.Z);
        bw.Write(&pkt.Position.Yaw);

        return sendBuffer;
    }
    
    // 헤더
    static SendBufferRef MakeSendBuffer(uint16 pktId, uint16 dataSize)
    {
        const uint16 packetSize = dataSize + sizeof(PacketHeader);

        SendBufferRef sendBuffer = TLS_SendBufferManager->Open(packetSize);
        PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
        header->id = pktId;
        header->size = packetSize;

        sendBuffer->Close(packetSize);

        return sendBuffer;
    }
};
