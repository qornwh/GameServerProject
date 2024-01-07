#pragma once
#include "pch.h"

namespace BS_Protocol
{
    struct FVector
    {
        int32 X;
        int32 Y;
        int32 Z;
        float Yew;

        uint32 size()
        {
            return sizeof(FVector);
        }
    };
    
    struct P_LOGIN_PAKCET
    {
        uint16 IdLen;
        string Id;
        uint16 Type;

        uint32 Size()
        {
            return sizeof(P_LOGIN_PAKCET);
        }
    };
    
    struct P_MOVE_PACKET
    {
        FVector Position;

        uint32 Size()
        {
            return sizeof(FVector);
        }
    };

    
}
