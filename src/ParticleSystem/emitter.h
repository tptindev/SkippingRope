#ifndef __EMITTER_H__
#define __EMITTER_H__

#include "../Physics2D/Math/Vector2D.h"
#include "../Physics2D/DataStructs/Array1D.h"

typedef struct
{
    Vec2 acceleration;
    Vec2 velocity;
    float rate;
    int life_span;
    Array1D *particles; // list of particle
} Emitter;

Emitter* CreateEmitter(Vec2 a, Vec2 v, float rate, int life_span);
void FreeEmitter(Emitter* emitter);
void AddParticle(Emitter* emitter, void* particle);
#endif // __EMITTER_H__