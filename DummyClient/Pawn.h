#pragma once
#include "pch.h"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/smart_ptr/enable_shared_from_this.hpp>

static boost::random::mt19937 rng;
static boost::random::uniform_int_distribution<> dstX{-2000, 2000};
static boost::random::uniform_int_distribution<> dstY{-2000, 2000};

static boost::random::uniform_real_distribution<> rot{0, 360};

struct Vector
{
    double X;
    double Y;
    double Z;

    Vector& operator=(const Vector& ref)
    {
        X = ref.X;
        Y = ref.Y;
        Z = ref.Z;
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
    Vector& GetRotate();
    void SetRotate(Vector vec);
    void SetRandRotate();

    bool IsUse() const { return _use; }
    void Start();

    bool IsMapRange();

protected:
    Vector _position{0, 0, 0};
    Vector _rotate{0, 0, 0};

    float _scaleValue = 1.f;

private:
    bool _use = false;
    MapRange _map{-2400, -2400, 2400, 2400};
};
