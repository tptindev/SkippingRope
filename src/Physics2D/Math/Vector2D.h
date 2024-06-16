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

inline Vec2 Vec2Zero() { return (Vec2){0.0f, 0.0f}; }

inline Vec2 Vec2Add(Vec2 A, Vec2 B){return (Vec2){ A.x + B.x, A.y + B.y };}

inline Vec2 Vec2Subtract(Vec2 A, Vec2 B){return (Vec2){ A.x - B.x, A.y - B.y };}

inline Vec2 Vec2Multiply(Vec2 A, Vec2 B){return (Vec2){ A.x * B.x, A.y * B.y };}

inline Vec2 Vec2Divide(Vec2 A, Vec2 B){return (Vec2){ A.x / B.x, A.y / B.y };}

inline void Vec2cale(Vec2 *vec, float n){(*vec) = Vec2Divide((*vec), (Vec2){n, n});}

inline float Vec2DotProduct(Vec2 A, Vec2 B){return (A.x * B.x + A.y * B.y);}

inline float Vec2CrossProduct(Vec2 A, Vec2 B){return (A.x * B.y - A.y * B.x);}

inline float Vec2Magnitude(Vec2 vec){return sqrtf(vec.x * vec.x + vec.y * vec.y);}

inline float Vec2Distance(Vec2 A, Vec2 B){ return sqrtf((B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y));}

inline Vec2 Vec2Normalize(Vec2 vec){return (Vec2) { vec.x / Vec2Magnitude(vec), vec.y / Vec2Magnitude(vec) };}

inline bool Vec2Equals(Vec2 A, Vec2 B) { return (A.x == B.x && A.y == B.y); }

inline Vec2 Vec2Transform(Vec2 vec, Trans2D transform)
{
    /**
     * Formula for rotating a vector in 2D
     * x2=cosβx1−sinβy1
     * y2=sinβx1+cosβy1
    `*/

    return (Vec2) {
        (transform.cos * vec.x - transform.sin * vec.y) + transform.x,
        (transform.sin * vec.x + transform.cos * vec.y) + transform.y
    };
}


#endif // Vec2_H
