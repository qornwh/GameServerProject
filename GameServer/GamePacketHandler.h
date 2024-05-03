#pragma once
#include <google/protobuf/message.h>
#include "PacketHeader.h"
#include "SendBuffer.h"
#include "ThreadManager.h"
#include "Types.h"

class GamePacketHandler
{
public:
    static bool CheckPacketHeader(PacketHeader* header, int32 offset, int32 len)
    {
        if (header == nullptr)
            return false;

        int32 dataSize = len - offset;

        if (dataSize < sizeof(PacketHeader))
            return false;
        if (dataSize < header->size)
            return false;
        
        return true;
    }

    static bool ParsePacketHandler(google::protobuf::Message& pkt, BYTE* buffer, const int32 dataSize, int32& offset)
    {
        const BYTE* payloadPtr = &buffer[offset];
        const bool parseResult = pkt.ParseFromArray(payloadPtr, dataSize);
        if (parseResult)
        {
            offset += static_cast<int32>(pkt.ByteSizeLong());
            return true;
        }
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
