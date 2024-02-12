#include "Shape.h"

Shape::Shape()
{
}

Shape::Shape(float x, float y)
{
    _center.X = x;
    _center.Y = y;
}

Circle::Circle(float radius) : _radius(radius)
{
    _type = ShapeType::CIRCLE;
}

Circle::Circle(float x, float y, float radius) : Shape(x, y), _radius(radius)
{
    _type = ShapeType::CIRCLE;
}

void Circle::Update()
{
    Shape::Update();
}

Rectangle::Rectangle(float width, float height) : _width(width), _height(height)
{
    _type = ShapeType::RECTANGLE;
    Rectangle::Update();

    _vertexs.emplace_back(Vector2{_width / 2, _width / 2});
    _vertexs.emplace_back(Vector2{_height / 2, _height / 2});
}

Rectangle::Rectangle(float x, float y, float width, float height) : Shape(x, y), _width(width), _height(height)
{
    _type = ShapeType::RECTANGLE;
    Rectangle::Update();

    _vertexs.emplace_back(Vector2{_width / 2, _width / 2});
    _vertexs.emplace_back(Vector2{_height / 2, _height / 2});
}

void Rectangle::Update()
{
}
