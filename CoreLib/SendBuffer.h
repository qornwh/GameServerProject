#pragma once
#include "pch.h"
#include "ReadWriteLock.h"

class SendBuffer
{
public:
    SendBuffer(SendBufferChunkRef sendBufferChunk, uint32 allocSize, BYTE* buffer);
    ~SendBuffer();

    BYTE* Buffer() { return _buffer; }
    BYTE* GetData() { return &_buffer[_writeSize]; }
    uint32 AllocSize() { return _allocSize; }
    uint32 WriteSize() { return _writeSize; }
    void Close(uint32 writeSize);

private:
    BYTE* _buffer = nullptr;
    uint32 _writeSize = 0;
    uint32 _allocSize = 0;
    // 이거 물어둬야지 레퍼런스카운트 유지되서 소멸안됨. 안하면 매니저에서 pop하면 소멸됨
    SendBufferChunkRef _sendBufferChunk;
};

class SendBufferChunk : public std::enable_shared_from_this<SendBufferChunk>
{
    enum
    {
        SEND_BUFFER_CHUNK_SIZE = 4096 // 일단 패킷 예상되는 최대 크기 생각하기. 아님 chunk를 이어붙여서 쓰면 이상하겠지?? 언제쯤 해제될지 모르니
    };

public:
    SendBufferChunk();
    ~SendBufferChunk();

    SendBufferRef Open(uint32 size);
    void Close(uint32 size);

    void Reset();

private:
    boost::array<BYTE, SEND_BUFFER_CHUNK_SIZE> _chunk = {};
    bool _use = false;
    uint32 _pos = 0;
};

class SendBufferManager
{
    enum
    {
        INIT_CHUNK_SIZE = 100
    };

public:
    SendBufferManager();
    ~SendBufferManager();

    SendBufferRef Open(uint32 size);
    static void ReleaseBuffer(SendBufferChunk* chunk);

    // 뒤에 const 붙이는 차이 알아내기!!
    SendBufferChunkRef CreateChunk() const;

private:
    Vector<SendBufferChunkRef> _chunks;
    Lock lock;
};

/*
바이트 배열을 미리 할당하여 풀링 기능을 사용한다.
바이트 배열을 동적 할당하여 사용한다.

* 정책
* 1 : SendBufferChunck를 make_shared 생성은 스레드가 시작되면 만든다.
* 2 : SendBufferChunck를 메모리 할당 해제(소멸)은 스레드가 종료될때 한다. 그외에는 절대 소멸되지 않게 한다.

1. 일단 처음에 동적 할당하고 할당된 메모리를 삭제 안하고 풀링으로 처리, 단 일정 이상 메모리에 차면 처음으로 돌아가서 처리

사용
1. 먼저 청크에서 _chunk에서 하나 할당받는다. => SendbufferRef make_shared된다.
2. 패킷 만들고 보낸뒤에는 당연히 스마트포인터 레퍼런스 카운트가 0가 되게 만들고 자동으로 소멸되도록 하게 무조건 해.

*/
