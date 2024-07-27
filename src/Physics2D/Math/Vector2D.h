#ifndef Vec2_H
#define Vec2_H

#include <math.h>
#include <stdbool.h>
#include "Transform2D.h"

typedef struct
{
    float x;
    float y;
} Vec2;

Vec2 Vec2Zero(void);

Vec2 Vec2Add(Vec2 A, Vec2 B);

Vec2 Vec2Subtract(Vec2 A, Vec2 B);

void Vec2cale(Vec2* vec, float n);

float Vec2DotProduct(Vec2 A, Vec2 B);

float Vec2CrossProduct(Vec2 A, Vec2 B);

float Vec2Magnitude(Vec2 vec);

float Vec2Distance(Vec2 A, Vec2 B);

Vec2 Vec2Normalize(Vec2 vec);

bool Vec2Equals(Vec2 A, Vec2 B);

Vec2 Vec2Transform(Vec2 vec, Trans2D transform);


#endif // Vec2_H
