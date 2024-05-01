#include "pch.h"
#include "OverlappedSocket.h"

void OverlappedSocket::Init()
{
    ZeroMemory(&_overlapped, sizeof(_overlapped));
}

void OverlappedSocket::SetType(int type)
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