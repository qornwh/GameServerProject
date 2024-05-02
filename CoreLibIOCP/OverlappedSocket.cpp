#include "pch.h"
#include "OverlappedSocket.h"

OverlappedSocket::OverlappedSocket()
{
    ZeroMemory(&_overlapped, sizeof(_overlapped));
}

void OverlappedSocket::SetType(int32 type)
{
    _type = type;
}

void OverlappedSocket::SetSession(SessionRef session)
{
    if (session == nullptr)
    {
        if (_session != nullptr)
        {
            _session.reset();
        }
    }
    else
    {
        _session = session;
    }
}