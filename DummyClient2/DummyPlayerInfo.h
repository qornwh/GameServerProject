#pragma once
#include "pch.h"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/smart_ptr/enable_shared_from_this.hpp>

static boost::random::mt19937 rng;
static boost::random::uniform_int_distribution<> dstX{2000, 5000};
static boost::random::uniform_int_distribution<> dstY{2000, 5000};
static boost::random::uniform_real_distribution<> rot{-1.f, 1.f};

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

    bool IsUse() const { return _use; }
    void Start();

    void UpdateMapRange();
    bool IsMapRange(int32 mapNum);

protected:
    Vector _position{0, 0, 0, 0};

    float _scaleValue = 1.f;

private:
    bool _use = false;
    MapRange _monsterMap{0, 0, 0, 0};
    MapRange _bossmap{0, 0, 0, 0};
};
