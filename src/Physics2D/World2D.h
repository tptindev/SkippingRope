#ifndef WORLD2D_H
#define WORLD2D_H

#include <stdbool.h>
#include "Math//Vector2D.h"

typedef struct
{
    Vec2 gravity;
    float w;
    float h;
} World2D;

World2D *CreateWorld(Vec2 gravity, float w, float h);
void DestroyWorld(World2D* world);

#endif // WORLD2D_H
