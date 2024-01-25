#pragma once
#include <boost/json.hpp>

#include "pch.h"

// 일단 잡다한 클래스 함수 여기에 넣어둔다.

namespace GameUtils
{
    class JsonParser
    {
    public:
        static boost::json::value GetStrParser(string& jsonStr)
        {
            return boost::json::parse(jsonStr);
        }

        static boost::json::value Parser(string key, const boost::json::value& json)
        {
            // 그냥 바로 크래시 낸다 !!!
            assert(!json.at(key).is_null());

            return json.at(key);
        }
    };

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
