#ifndef WORLD2D_H
#define WORLD2D_H

#include <stdbool.h>

#define MIN_BODY_SIZE 0.25f
#define MAX_BODY_SIZE 3.0f

#define MAX_BODY 255
#define MAX_SHAPE 1024

typedef struct
{
    float gravity;
    float w;
    float h;
    bool* bodyBitset;
    void* bodies[MAX_BODY];
} World2D;

World2D *CreateWorld(float g, float w, float h);
void DestroyWorld(World2D* world);

void Update(World2D* world, float deltatime, int totalIterations);
void BroadPhase(World2D* world);
void NarrowPhase(World2D* world);

#endif // WORLD2D_H
