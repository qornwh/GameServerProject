#pragma once

namespace GameEngine
{
    class MathUtils
    {
    public:
        static float GetSin(float yaw, float distance = 1)
        {
            return distance * sinf(yaw * 3.14f / 180.f);
        }

        static float GetCos(float yaw, float distance = 1)
        {
            return distance * cosf(yaw * 3.14f / 180.f);
        }

        // 라디안을 각도로 변환하는 함수
        static float RadianToDegree(float radian)
        {
            return radian * 180.0f / 3.14f;
        }
    };
}
