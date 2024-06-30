#include "World2D.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

World2D *CreateWorld(Vec2 gravity, float w, float h)
{
    srand((unsigned int)time(NULL));
    World2D* world = (World2D*)malloc(sizeof(World2D));
    if (world == NULL)
    {
        fprintf(stderr, "ERROR: Couldn't realloc memory! line: %d\n", __LINE__);
        return NULL;
    }
    printf("Hello World!\n");
    world->gravity.x = gravity.x;
    world->gravity.y = gravity.y;
    world->w = w;
    world->h = h;
	world->objId.max = 32;
	world->objId.last = 0;
    return world;
}

void DestroyWorld(World2D *world)
{
    if (world != NULL)
    {
        free(world);
        world = NULL;
    }
}

