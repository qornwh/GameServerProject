#pragma once
#include "Shape.h"

// 교차 구현 2d로만 구현

// 분리축을 사용한다. 사각형만을 사용한다.
// A, B 2개의 사용한다고 치면. A를 중점(0, 0)으로, B를 A에 맞게 좌표변환부터 시작한다.
// 그려면 각 A, B 중점 centerA, centerB의 스칼라를 구하고.
// A는 A.x/2, A.y/2축 평행하므로 처럼 값이 되고
// b는 축이랑 제일 가까운 정점을 가져와서 중점이랑 내적한다.
// 후 x, y축 둘다. A B중점이랑 내적된크기보다 크면 교차된것으로 친다.  

class Collider
{
public:
    Collider(float radius);
    Collider(float width, float height);

    ~Collider();

    bool IsTrigger(Collider& other);
    Shape& GetShape() { return *_shape; }

    void SetPosition(float x, float y);
    Vector2& GetPosition() { return _position; }

    void SetRotate(float rot);
    float GetRotate() { return _rotate; }

    void ResetCollider(float radius);
    void ResetCollider(float width, float height);

private:
    Shape* _shape;

    Vector2 _position{0.f, 0.f};
    float _rotate = 0.f;
};
