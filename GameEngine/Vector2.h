#pragma once
#include <complex>
#include <iostream>

#include "GameEngine.h"

struct Vector2
{
    Vector2(float X, float Y) : X(X), Y(Y)
    {
    }

    float X;
    float Y;

    Vector2& operator=(const Vector2& ref)
    {
        X = ref.X;
        Y = ref.Y;
        return *this;
    }

    Vector2 operator+(const Vector2& ref)
    {
        return Vector2(X + ref.X, Y + ref.Y);
    }

    Vector2 operator-(const Vector2& ref)
    {
        return Vector2(X - ref.X, Y - ref.Y);
    }

    static float DotProduct(Vector2 a, Vector2 b)
    {
        return a.X * b.X + a.Y * b.Y;
    }

    static float AbsDotProduct(Vector2 a, Vector2 b)
    {
        return abs(a.X * b.X + a.Y * b.Y);
    }

    // 벡터 외적 공식 사용
    static float crossProductAngle(Vector2 a, Vector2 b)
    {
        return a.X * b.Y - b.X * a.Y;
    }

    // 벡터 크기
    static float Magnitude(Vector2 vector)
    {
        return static_cast<float>(std::sqrt(pow(vector.X, 2) + pow(vector.Y, 2)));
    }

    static Vector2 NormalizeVector(Vector2 vector)
    {
        float mag = Magnitude(vector);

        if (mag != 0.0)
            return Vector2(vector.X / mag, vector.Y / mag);
        else
            return Vector2(0, 0);
    }

    // 벡터 외적 공식 사용
    static float CrossProduct(float x1, float y1, float x2, float y2)
    {
        return x1 * y2 - x2 * y1;
    }
    
    // 2점 사이각 구한는 함수
    static float CalculateAngle(Vector2& a, Vector2& b)
    {
        Vector2 sub = b - a;
        //정면벡터를 쓴 이유는 기준으로 잡기 위해 마지막에 나온 각도를 더하는게 아닌 set을 하기때문
        Vector2 forward(1, 0);
        float dot = DotProduct(NormalizeVector(sub), forward);
        // float mag1 = magnitude(x1, y1);
        // float mag2 = magnitude(x2, y2);

        // 두 벡터의 내적의 크기와 벡터의 크기를 이용하여 각도 계산
        // float angleCos = dot / (mag1 * mag2);

        // acos 함수를 사용하여 라디안으로 변환된 각도를 계산
        float radian = acos(dot);
        float angleRad = GameEngine::MathUtils::RadianToDegree(radian);

        if (crossProductAngle(forward, sub) > 0)
        {
            return angleRad;
        }
        else
        {
            return angleRad * (-1);
        }
    }
};
