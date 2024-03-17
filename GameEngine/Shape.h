#pragma once
#include <vector>

#include "GameEngine.h"
#include "Vector2.h"

enum ShapeType
{
    CIRCLE,
    RECTANGLE
};

class Shape
{
public:
    Shape();
    Shape(float x, float y);
    virtual ~Shape() {}

    Vector2& GetCenter() { return _center; }
    virtual ShapeType GetType() { return _type; }

    virtual void Update()
    {
    }


    virtual std::vector<Vector2>& GetVertexs(float rot) { return _vertexs; }
protected:
    ShapeType _type = ShapeType::CIRCLE;
    std::vector<Vector2> _vertexs;
    Vector2 _center{0.f, 0.f};
};

class Circle : public Shape
{
public:
    Circle(float radius);
    Circle(float x, float y, float radius);

    ~Circle()
    {
    }

    float GetRadius() { return _radius; }
    void Update();

private:
    float _radius = 0.3f;
};

class Rectangle : public Shape
{
public:
    Rectangle(float width, float height);
    Rectangle(float x, float y, float width, float height);

    ~Rectangle()
    {
    }
    // 직사각형인 경우 정점이 4개지만 축에 정렬해 2번만 해도됨!!!

    float GetWidth() { return _width; }
    float GetHeight() { return _height; }
    void Update();

    std::vector<Vector2>& GetVertexs(float rot) override
    {
        _vertexs[0].X = _width * GameEngine::MathUtils::GetCos(rot) / 2;
        _vertexs[0].Y = _width * GameEngine::MathUtils::GetSin(rot) / 2;

        _vertexs[1].X = _height * GameEngine::MathUtils::GetCos(rot - 90) / 2;
        _vertexs[1].Y = _height * GameEngine::MathUtils::GetSin(rot - 90) / 2;

        return _vertexs;
    }

private:
    float _width = 0.f;
    float _height = 0.f;
};
