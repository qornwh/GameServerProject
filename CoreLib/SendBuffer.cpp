#include "pch.h"
#include "SendBuffer.h"

#include <boost/make_shared.hpp>

#include "ThreadManager.h"

SendBuffer::SendBuffer(SendBufferChunkRef sendBufferChunk, uint32 allocSize, BYTE* buffer):
    _buffer(buffer),
    _allocSize(allocSize),
    _sendBufferChunk(sendBufferChunk)
{
}

SendBuffer::~SendBuffer()
{
    // std::cout << "~SendBuffer !!" << std::endl;
}

void SendBuffer::Close(uint32 writeSize)
{
    // crash if _wirtesize + writeSize > _allocSize
    _writeSize = writeSize;
    _sendBufferChunk->Close(writeSize);
}

SendBufferChunk::SendBufferChunk()
{
}

SendBufferChunk::~SendBufferChunk()
{
}

SendBufferRef SendBufferChunk::Open(uint32 size)
{
    if (size > SEND_BUFFER_CHUNK_SIZE)
        return nullptr;
    _use = true;
    _pos = size;

    SendBufferRef sendBuffer = boost::make_shared<SendBuffer>(shared_from_this(), size, &_chunk[0]);
    return sendBuffer;
}

void SendBufferChunk::Close(uint32 size)
{
    // crash _use == true
    Reset();
}

void SendBufferChunk::Reset()
{
    _use = false;
    _pos = 0;
}

SendBufferManager::SendBufferManager()
{
    for (int32 i = 0; i < INIT_CHUNK_SIZE; i++)
        _chunks.push_back(CreateChunk());
}

SendBufferManager::~SendBufferManager()
{
    std::cout << "스레드 소멸 될때 죽어라 제발 !!!!" << std::endl;
}

SendBufferRef SendBufferManager::Open(uint32 size)
{
    SendBufferChunkRef chunk;
    {
        // WriteLockGuard wl(lock, "write SendBufferManager::Open");
        if (_chunks.empty())
        {
            chunk = _chunks.emplace_back();
            _chunks.pop_back();
        }
        else
            chunk = CreateChunk();
    }
    // crash _use == true
    if (chunk)
    {
        chunk->Reset();
        return chunk->Open(size);
    }

    return nullptr;
}

void SendBufferManager::ReleaseBuffer(SendBufferChunk* chunk)
{
    {
        // WriteLockGuard wl(lock, "write SendBufferManager::ReleaseBuffer");
        // 여기서 다시 shared_ptr을 생성한다.
        // TSL마다 버퍼 만들기로 해본다. 그래서 lock이 필요 x
        SendBufferChunkRef sendBufferChunk = SendBufferChunkRef(chunk, ReleaseBuffer);
        TLS_SendBufferManager->_chunks.push_back(sendBufferChunk);
        // std::cout << "chunk ref count : " << sendBufferChunk.use_count() << std::endl;
    }
}

SendBufferChunkRef SendBufferManager::CreateChunk() const
{
    return SendBufferChunkRef(new SendBufferChunk(), ReleaseBuffer);
}
