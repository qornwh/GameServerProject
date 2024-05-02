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

    void Init();

    void SetType(int32 type);
    int GetType()
    {
        return _type;
    }

    void SetSession(SessionRef session);
    SessionRef GetSession()
    {
        return _session;
    }

private:
    WSAOVERLAPPED _overlapped;
    SessionRef _session;
    int _type = Type::CONN;
};

