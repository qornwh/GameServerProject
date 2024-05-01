#include "pch.h"
#include "SendBuffer.h"
#include "ThreadManager.h"

SendBuffer::SendBuffer(SendBufferChunkRef sendBufferChunk, uint32 allocSize, BYTE* buffer):
    _buffer(buffer),
    _allocSize(allocSize),
    _sendBufferChunk(sendBufferChunk)
{
}

SendBuffer::~SendBuffer()
{
}

void SendBuffer::Close(uint32 writeSize)
{
    CrashFunc(_writeSize + writeSize <= _allocSize);
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

    SendBufferRef sendBuffer = std::make_shared<SendBuffer>(shared_from_this(), size, &_chunk[0]);
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
}

SendBufferRef SendBufferManager::Open(uint32 size)
{
    SendBufferChunkRef chunk;
    {
        if (_chunks.empty())
        {
            chunk = _chunks.emplace_back();
            _chunks.pop_back();
        }
        else
            chunk = CreateChunk();
    }

    if (chunk)
    {
        chunk->Reset();
        return chunk->Open(size);
    }

    return nullptr;
}

void SendBufferManager::ReleaseBuffer(SendBufferChunk* chunk)
{
    SendBufferChunkRef sendBufferChunk = SendBufferChunkRef(chunk, ReleaseBuffer);
    TLS_SendBufferManager->_chunks.push_back(sendBufferChunk);
}

SendBufferChunkRef SendBufferManager::CreateChunk() const
{
    return SendBufferChunkRef(new SendBufferChunk(), ReleaseBuffer);
}
