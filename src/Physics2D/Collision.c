#include "Collision.h"
#include "Math/Vector2D.h"
#include "Math/Shape2D.h"

bool IsCollisionCircle(Body2D *a, Body2D *b, void (*callback)(Body2D *, Body2D *))
{
    Circle* c1 = (Circle*)a->shape.define;
    Circle* c2 = (Circle*)b->shape.define;
    bool result = (Vec2Distance(a->position, b->position) <= (c1->radius + c2->radius));
    if (result)
    {
        callback(a, b);
    }

    return result;
}
