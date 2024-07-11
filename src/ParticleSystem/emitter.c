#include "emitter.h"
#include <stdlib.h>

Emitter* CreateEmitter(Vec2 a, Vec2 v, float rate, int life_span)
{
    Emitter* emitter = malloc(sizeof(Emitter));
    if (emitter != NULL)
    {
        emitter->acceleration.x = a.x;
        emitter->acceleration.y = a.y;

        emitter->velocity.x = v.x;
        emitter->velocity.y = v.y;

        emitter->rate = rate;
        emitter->life_span = life_span;
        emitter->particles = CreateArray1D();
    }
    return emitter;
}

void FreeEmitter(Emitter* emitter)
{
    if (emitter != NULL)
    {
        FreeArray1D(emitter->particles);
        free(emitter);
        emitter = NULL;
    }
}

void AddParticle(Emitter* emitter, void* particle)
{
    Array1DPush(emitter->particles, particle);
}

void UpdateEmitter(Emitter* emitter, float dt)
{
}
