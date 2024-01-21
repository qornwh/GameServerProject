#pragma once
#include "pch.h"

// 일단 잡다한 클래스 함수 여기에 넣어둔다.

namespace GameUtils
{
    class TickCounter
    {
    public:
        TickCounter(int32 value) : _value(value)
        {
        }

        ~TickCounter()
        {
        }

        int32 Add(int32 value = 1)
        {
            _tick = (_tick + value) % _value;
            return _tick;
        }

        int32 GetTick()
        {
            return _tick;
        }

        int32 GetTickValue()
        {
            return _value;
        }

    private:
        int32 _value;
        int32 _tick = 0;
    };

    class MathUtils
    {
    public:
        static float GetSin(float yaw, float distance = 1)
        {
            return distance * sinf(yaw * 3.14 / 180);
        }

        static float GetCos(float yaw, float distance = 1)
        {
            return distance * cosf(yaw * 3.14 / 180);
        }
    };
}
