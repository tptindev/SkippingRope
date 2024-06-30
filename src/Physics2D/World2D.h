#ifndef WORLD2D_H
#define WORLD2D_H

#include <stdbool.h>
#include "Math//Vector2D.h"

typedef struct {
	unsigned int max;
	unsigned int last;	
} ObjectId;

typedef struct
{
    Vec2 gravity;
    float w;
    float h;
	ObjectId objId;
} World2D;

World2D *CreateWorld(Vec2 gravity, float w, float h);
void DestroyWorld(World2D* world);

#endif // WORLD2D_H
