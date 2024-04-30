#pragma once
#include "pch.h"

struct Vector3d
{
    Vector3d(float X, float Y, float Z, float Yaw) : X(X), Y(Y), Z(Z), Yaw(Yaw)
    {
    }
    
    float X;
    float Y;
    float Z;
    float Yaw;

    Vector3d& operator=(const Vector3d& ref)
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

class DummyPlayerInfo : public std::enable_shared_from_this<DummyPlayerInfo>
{
public:
    DummyPlayerInfo();
    ~DummyPlayerInfo();

    Vector3d& GetPostion();
    void SetPostion(Vector3d vec);
    void UpdateRotate();

    bool IsUse() const { return _use; }
    void Start();

    void UpdateMapRange();
    bool IsMapRange(int32 mapNum);
    void updatePosition();

protected:
    Vector3d _position{0, 0, 0, 0};

private:
    bool _use = false;
    MapRange _map{0, 0, 0, 0};

    float _increaseX = 0.f;
    float _increaseY = 0.f;
    float _speed = 5.f;
};
