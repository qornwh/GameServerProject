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

        void ResetTic(int32 value = -1)
        {
            _tick = value;
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
        // 벡터 내적 공식 사용
        static double DotProduct(float x1, float y1, float x2, float y2)
        {
            return x1 * x2 + y1 * y2;
        }
        // 벡터 외적 공식 사용
        static double CCrossProduct(double x1, double y1, double x2, double y2)
        {
            return x1 * y2 - x2 * y1;
        }
        // 벡터 크기
        static double Magnitude(float x, float y)
        {
            return sqrt(x * x + y * y);
        }
        // 벡터 정규화
        static void NormalizeVector(float &x, float &y)
        {
            float mag = Magnitude(x, y);

            // 벡터의 크기가 0이 아닌 경우에만 정규화 수행
            if (mag != 0.0)
            {
                x /= mag;
                y /= mag;
            }
        }
        // 라디안을 각도로 변환하는 함수
        static double RadianToDegree(double radian)
        {
            return radian * 180.0 / 3.14f;
        }
        // 2점 사이각 구한는 함수
        static float CalculateAngle(float x1, float y1, float x2, float y2)
        {
            float x = x2 - x1;
            float y = y2 - y1;
            float x_forward = 1;
            float y_forward = 0;
            NormalizeVector(x, y);
            double dot = DotProduct(x_forward, y_forward, x, y);
            // float mag1 = magnitude(x1, y1);
            // float mag2 = magnitude(x2, y2);

            // 두 벡터의 내적의 크기와 벡터의 크기를 이용하여 각도 계산
            // float angleCos = dot / (mag1 * mag2);

            // acos 함수를 사용하여 라디안으로 변환된 각도를 계산
            double radian = acos(dot);
            float angleRad = RadianToDegree(radian);

            if (CCrossProduct(x_forward, y_forward, x, y) > 0)
            {
                return angleRad;
            }
            else
            {
                return angleRad * (-1);
            }
        }
    };
}
