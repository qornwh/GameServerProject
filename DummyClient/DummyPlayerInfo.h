#pragma once
#include "pch.h"

struct Vector
{
    Vector(float X, float Y, float Z, float Yaw) : X(X), Y(Y), Z(Z), Yaw(Yaw)
    {
    }
    
    float X;
    float Y;
    float Z;
    float Yaw;

    Vector& operator=(const Vector& ref)
    {
        X = ref.X;
        Y = ref.Y;
        Z = ref.Z;
        Yaw = ref.Yaw;
        return *this;
    }
};

struct MapRange
{
    int32 StartX;
    int32 StartY;
    int32 EndX;
    int32 EndY;
};

class DummyPlayerInfo : public boost::enable_shared_from_this<DummyPlayerInfo>
{
public:
    DummyPlayerInfo();
    ~DummyPlayerInfo();

    Vector& GetPostion();
    void SetPostion(Vector vec);
    void UpdateRotate();

    bool IsUse() const { return _use; }
    void Start();

    void UpdateMapRange();
    bool IsMapRange(int32 mapNum);
    void updatePosition();

protected:
    Vector _position{0, 0, 0, 0};

private:
    bool _use = false;
    MapRange _map{0, 0, 0, 0};

    float _increaseX = 0.f;
    float _increaseY = 0.f;
    float _speed = 5.f;
};
