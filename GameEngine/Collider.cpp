#include "Collider.h"

Collider::Collider(float radius)
{
    _shape = new Circle(radius);
}

Collider::Collider(float width, float height)
{
    _shape = new Rectangle(width, height);
}

Collider::~Collider()
{
    if (_shape != nullptr)
    {
        delete _shape;
    }
}

bool Collider::IsTrigger(Collider& other)
{
    switch (_shape->GetType())
    {
    case ShapeType::CIRCLE:
        {
            Circle* shape = static_cast<Circle*>(_shape);
            if (other.GetShape().GetType() == ShapeType::CIRCLE)
            {
                Circle& otherShape = static_cast<Circle&>(other.GetShape());
                // 원 원
                Vector2 del = other.GetPosition() - _position;
                float dist = Vector2::Magnitude(del);

                if (dist < otherShape.GetRadius() + shape->GetRadius())
                    return true;
            }
            else
            {
                Rectangle& otherShape = static_cast<Rectangle&>(other.GetShape());
                // 원 사각형
                // 기준을 사각형으로 잡는다. 그럼 방향 생각 x가 된다.
                // 원의 중심점은 사각형 중심점을 기준으로 1사분면으로 좌표 변환 한다. 그냥 abs 절대값으로 변경하면 된다.
                // 사각형의 가로 세로 길이에 원의 반지름 더해주고 원의 중심점이 들어가면, 겹친다.
                Vector2 del = (_position + _shape->GetCenter()) - (other.GetPosition() + otherShape.GetCenter());
                float radius = shape->GetRadius();

                float vWidth = otherShape.GetWidth() / 2.f + radius;
                float vHeight = otherShape.GetHeight() / 2.f + radius;

                if (abs(del.X) < vWidth && abs(del.Y) < vHeight)
                    return true;
            }
        }
        break;
    case ShapeType::RECTANGLE:
        {
            Rectangle* shape = static_cast<Rectangle*>(_shape);
            if (other.GetShape().GetType() == ShapeType::CIRCLE)
            {
                Circle& otherShape = static_cast<Circle&>(other.GetShape());
                // 사각형 원
                // 기준을 사각형으로 잡는다. 그럼 방향 생각 x가 된다.
                // 원의 중심점은 사각형 중심점을 기준으로 1사분면으로 좌표 변환 한다. 그냥 abs 절대값으로 변경하면 된다.
                // 사각형의 가로 세로 길이에 원의 반지름 더해주고 원의 중심점이 들어가면, 겹친다.

                Vector2 del = (other.GetPosition() + otherShape.GetCenter()) - (_position + _shape->GetCenter());
                float radius = otherShape.GetRadius();

                float vWidth = shape->GetWidth() / 2.f + radius;
                float vHeight = shape->GetHeight() / 2.f + radius;

                if (abs(del.X) < vWidth && abs(del.Y) < vHeight)
                    return true;
            }
            else
            {
                // 사각형 사각형
                // 1. 각 rect를 중심점을 기준으로 x,y 축을 잡고 하게 되면, 사실상 단위 벡터가 (1, 0) (0, 1)이되므로
                // x좌표의 값, y좌표의 값과 기준이된 width /2, height /2 보다 dist가 작은 지 판단. => 이유는 width, height축으로하면 꼭지점 으로 투영됨.
                // 그래서 그길이 2개를 더해서 2점사이보다. 크면 교차된거임.
                // 2. 그냥 좌표 변환 없이 각 rect 법선벡터 구해서 단위벡터 만들어서 내적으로 구한다. 그후 dist 비교 판단
                // 3. 외적을 절대값으로 하는이유는 음수로 나와선 안된다.

                Vector2 dist = (_position + _shape->GetCenter()) - (other.GetPosition() + other.GetShape().GetCenter());
                std::vector<Collider*> arr = {this, &other};

                for (auto& collider : arr) // 실제론 2개
                {
                    for (auto& vertex : collider->GetShape().GetVertexs(collider->GetRotate())) // 2 * 2 = 4개
                    {
                        float sum = 0.f;
                        Vector2 nomal = Vector2::NormalizeVector(vertex);

                        for (auto collider2 : arr) // 실제론 2개
                        {
                            for (auto vertex2 : collider2->GetShape().GetVertexs(collider2->GetRotate())) // 2 * 2 = 4개
                            {
                                sum += Vector2::AbsDotProduct(vertex2, nomal);
                            }
                        }

                        if (Vector2::AbsDotProduct(dist, nomal) > sum)
                        {
                            return false;
                        }
                    }
                }

                return true;
            }
        }
        break;
    }

    return false;
}

void Collider::SetPosition(float x, float y)
{
    _position.X = x;
    _position.Y = y;
}

void Collider::SetRotate(float rot)
{
    _rotate = rot;
}

void Collider::ResetCollider(float radius)
{
    if (_shape != nullptr)
    {
        delete _shape;
    }
    _shape = new Circle(radius);
}

void Collider::ResetCollider(float width, float height)
{
    if (_shape != nullptr)
    {
        delete _shape;
    }
    _shape = new Rectangle(width, height);
}
