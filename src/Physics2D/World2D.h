#ifndef WORLD2D_H
#define WORLD2D_H

#include <stdbool.h>

typedef struct {
	unsigned int max;
	unsigned int last;	
} ObjectId;

typedef struct
{
    float gravity;
    float w;
    float h;
	ObjectId objId;
} World2D;

World2D *CreateWorld(float g, float w, float h);
void DestroyWorld(World2D* world);

#endif // WORLD2D_H
