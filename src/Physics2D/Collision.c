#include "Collision.h"
#include "Math/Vector2D.h"
#include "Math/Shape2D.h"

bool IsCollisionCircle(Circle* a, Circle* b)
{
    return (Vec2Distance(a->center, b->center) <= (a->radius + b->radius));
}

