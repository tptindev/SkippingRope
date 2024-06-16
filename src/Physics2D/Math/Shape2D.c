#include "Shape2D.h"
#include <stdlib.h>

Polygon *CreatePolygon(Vec2 *vertices, size_t count)
{
    Polygon *shape = (Polygon*)malloc(sizeof(Polygon));
    shape->points = (Vec2*)malloc(count * sizeof(Vec2));
    for (size_t i = 0; i < count; ++i) {
        shape->points[i] = vertices[i];
    }
    shape->count = count;
    return shape;
}

Circle *CreateCircle(Vec2 center, float radius)
{
    Circle *shape = (Circle*)malloc(sizeof(Circle));
    shape->center = center;
    shape->radius = radius;
    return shape;
}

Ellipse *CreateEllipse(Vec2 center, float a, float b)
{
    Ellipse *shape = (Ellipse*)malloc(sizeof(Ellipse));
    shape->center   = center;
    shape->a        = a;
    shape->b        = b;
    return shape;
}

Capsule *CreateCapsule(Vec2 center, float height, float radius)
{
    Capsule *shape = (Capsule*)malloc(sizeof(Capsule));
    shape->center  = center;
    shape->height  = height;
    shape->radius  = radius;
    return shape;
}

float GetShapeArea(const Shape2D *shape)
{
    float area = 0.0f;
    switch (shape->type) {
    case CIRCLE:
    {
        Circle *circle = (Circle*)shape->define;
        area = (PI * (circle->radius * circle->radius));
        break;
    }
    case ELLIPSE:
    {
        Ellipse *ellipse = (Ellipse*)shape->define;
        area = (PI * (ellipse->a * ellipse->b));
        break;
    }
    case CAPSULE:
    {
        Capsule *capsule = (Capsule*)shape->define;
        float circle_area = (PI * (capsule->radius * capsule->radius));
        float rectangle_area = (2 * capsule->radius) * (2 * capsule->radius) * (capsule->height - 1);
        area = circle_area + rectangle_area;
        break;
    }
    case POLYGON:
    {
        Polygon *poly = (Polygon*)shape->define;
        int j = (int)(poly->count) - 1;
        for (int i = 0; i < (int)(poly->count); ++i) {
            area += (poly->points[j].x + poly->points[i].x) * (poly->points[j].y - poly->points[i].y);
            j = i;
        }

        area *= 0.5f;
        break;
    }
    default:
        break;
    }
    return fabsf(area);
}

void GetCircleBoundary(Rect2D *rect, Circle *circle)
{
    rect->x = circle->center.x - circle->radius;
    rect->y = circle->center.y - circle->radius;
    rect->width = circle->radius * 2.0f;
    rect->height = circle->radius * 2.0f;
}

void GetPolygonBoundary(Rect2D *rect, Polygon *polygon)
{
    float minX = polygon->points[0].x;
    float minY = polygon->points[0].y;
    float maxX = polygon->points[0].x;
    float maxY = polygon->points[0].y;

    for (size_t i = 1; i < polygon->count; ++i) {
        if (minX > polygon->points[i].x)
        {
            minX = polygon->points[i].x;
        }
        if (maxX < polygon->points[i].x)
        {
            maxX = polygon->points[i].x;
        }
        if (minY > polygon->points[i].y)
        {
            minY = polygon->points[i].y;
        }
        if (maxY < polygon->points[i].y)
        {
            maxY = polygon->points[i].y;
        }
    }

    rect->x = minX;
    rect->y = minY;
    rect->width = maxX - minX;
    rect->height = maxY - minY;
}

void GetEllipseBoundary(Rect2D *rect, Ellipse *ellipse)
{
    rect->x = ellipse->center.x - ellipse->a;
    rect->y = ellipse->center.y - ellipse->b;
    rect->width = ellipse->a * 2.0f;
    rect->height = ellipse->b * 2.0f;
}

void GetCapsuleBoundary(Rect2D *rect, Capsule *capsule)
{
    /**
     * Need fix
     **/
    rect->x = capsule->center.x - capsule->radius;
    rect->y = capsule->center.y - capsule->height / 2.0f;
    rect->width = capsule->radius * 2.0f;
    rect->height = capsule->height + capsule->radius * 2.0f;
}
