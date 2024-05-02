#pragma once
#include "pch.h"

class Session;

class OverlappedSocket
{
public:
    enum Type
    {
        ACCP,
        CONN,
        READ,
        SEND,
        DISC,
    };

    OverlappedSocket();
    void SetType(int32 type);
    void SetSession(SessionRef session);
    int GetType() { return _type; }
    SessionRef GetSession() { return _session; }

private:
    WSAOVERLAPPED _overlapped;
    SessionRef _session;
    int _type = Type::CONN;
};

