#include "World2D.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

World2D *CreateWorld(float g, float w, float h)
{
    srand((unsigned int)time(NULL));
    World2D* world = (World2D*)malloc(sizeof(World2D));
    if (world == NULL)
    {
        fprintf(stderr, "ERROR: Couldn't realloc memory! line: %d\n", __LINE__);
        return NULL;
    }
    world->gravity = g;
    world->w = w;
    world->h = h;
    world->bodyBitset = (bool*)calloc(MAX_BODY, sizeof(bool));

    printf("Hello World!\n");
    return world;
}

void DestroyWorld(World2D *world)
{
    if (world != NULL)
    {
        free(world->bodyBitset);
        free(world);
        world = NULL;
    }
}

void Update(World2D *world, float deltatime, int totalIterations)
{
    (void)world;
    (void)deltatime;
    (void)totalIterations;
    for (int i = 0; i < totalIterations; ++i) {
        BroadPhase(world);
        NarrowPhase(world);
    }
}

void BroadPhase(World2D* world)
{
    for (unsigned char i = 0; i < (world->h * world->w); ++i) {

    }
}

void NarrowPhase(World2D* world)
{

}
