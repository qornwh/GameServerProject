#pragma once
#include <boost/asio/buffer.hpp>
#include <google/protobuf/message.h>

#include "PacketHeader.h"
#include "SendBuffer.h"
#include "ThreadManager.h"
#include "Types.h"

class GamePacketHandler
{
public:
    static bool CheckPacketHeader(const boost::asio::mutable_buffer& buffer, PacketHeader* header, int32 offset,
                                  int32 len)
    {
        if (header == nullptr)
            return false;

        int32 dataSize = len - offset;

        if (dataSize < sizeof(PacketHeader))
            return false;

        if (dataSize < header->size)
        {
            cout << " header code : " << header->id << " header size : " << header->size << " data size : " << dataSize
                << endl;
            return false;
        }
        return true;
    }

    static bool ParsePacketHandler(google::protobuf::Message& pkt, const boost::asio::mutable_buffer& buffer,
                                   const int32 dataSize,
                                   int32& offset)
    {
        const BYTE* payloadPtr = static_cast<BYTE*>(buffer.data()) + offset;
        const bool parseResult = pkt.ParseFromArray(payloadPtr, dataSize);
        if (parseResult)
        {
            offset += static_cast<int32>(pkt.ByteSizeLong());
            return true;
        }
        else
            return false;
    }

    static SendBufferRef MakePacketHandler(google::protobuf::Message& pkt, uint16 pktId)
    {
        uint16 pktSize = pkt.ByteSizeLong();

        SendBufferRef sendBuffer = MakeHeaderPacketHandler(pktId, pktSize);

        if (pktSize > 0)
            pkt.SerializeToArray(&sendBuffer->Buffer()[sizeof(PacketHeader)], pktSize);

        return sendBuffer;
    }

    // 헤더
    static SendBufferRef MakeHeaderPacketHandler(uint16 pktId, uint16 pktSize)
    {
        const uint16 packetSize = pktSize + sizeof(PacketHeader);

        SendBufferRef sendBuffer = TLS_SendBufferManager->Open(packetSize);
        PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
        header->id = pktId;
        header->size = packetSize;

        sendBuffer->Close(packetSize);

        return sendBuffer;
    }
};
