#include "Body2D.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Math/Shape2D.h"

Body2D* CreateBody(World2D *w, Body2D* define)
{
    for (int i = 0; i < MAX_BODY; ++i) {
        if (w->bodyBitset[i] == 0)
        {
            w->bodyBitset[i] = 1;
            Body2D* body = (Body2D*)malloc(sizeof(Body2D));
            if (body == NULL)
            {
                fprintf(stderr, "ERROR: Couldn't realloc memory! line: %d\n", __LINE__);
                return NULL;
            }
            body->index                     = i;
            body->position                  = define->position;
            body->density                   = define->density;
            body->linearVelocity            = define->linearVelocity;
            body->rotationVelocity          = define->rotationVelocity;
            body->rotation                  = define->rotation;
            body->restitution               = define->restitution;
            body->mass                      = define->mass;
            body->type                      = define->type;
            body->shape                     = define->shape;
            w->bodies[i] = body;
            return body;
        }
    }
    return NULL;
}

Shape2D *BodyCreateShape(Body2D* target, ShapeType type, void* define)
{
    switch (type) {
    case CIRCLE:
        GetCircleBoundary(&target->shape.box, (Circle*)define);
        break;
    case POLYGON:
        GetPolygonBoundary(&target->shape.box, (Polygon*)define);
        break;
    case ELLIPSE:
        GetEllipseBoundary(&target->shape.box, (Ellipse*)define);
        break;
    case CAPSULE:
        GetCapsuleBoundary(&target->shape.box, (Capsule*)define);
        break;
    default:
        break;
    }

    target->shape.box.x += target->position.x;
    target->shape.box.y += target->position.y;

    target->shape.define = define;
    target->shape.type = type;

    return &target->shape;

}

void DestroyBody(World2D* world, Body2D* body)
{
    world->bodyBitset[body->index] = 0;
    world->bodies[body->index] = NULL;
    free(body);
    body = NULL;
}

Trans2D* BodyGetTransform(Body2D* target)
{
    return &target->transform;
}

void BodySetTransform(Body2D* target, float x, float y, float angle)
{
    target->transform.x   = x;
    target->transform.y   = y;
    target->transform.cos = cosf(angle);
    target->transform.sin = sinf(angle);
}

Shape2D* BodyGetShape(Body2D* body)
{
    return &body->shape;
}
