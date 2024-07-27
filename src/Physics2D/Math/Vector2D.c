#include "Vector2D.h"
Vec2 Vec2Zero(void) { return (Vec2){0.0f, 0.0f}; }

Vec2 Vec2Add(Vec2 A, Vec2 B){ return (Vec2){ A.x + B.x, A.y + B.y }; }

Vec2 Vec2Subtract(Vec2 A, Vec2 B){ return (Vec2){ A.x - B.x, A.y - B.y }; }

Vec2 Vec2Transform(Vec2 vec, Trans2D transform)
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

bool Vec2Equals(Vec2 A, Vec2 B) { return (A.x == B.x && A.y == B.y); }

Vec2 Vec2Normalize(Vec2 vec){return (Vec2) { vec.x / Vec2Magnitude(vec), vec.y / Vec2Magnitude(vec) }; }

float Vec2Distance(Vec2 A, Vec2 B) { return Vec2Magnitude((Vec2) { (B.x - A.x), (B.y - A.y) }); }

float Vec2Magnitude(Vec2 vec){ return sqrtf(vec.x * vec.x + vec.y * vec.y); }

float Vec2CrossProduct(Vec2 A, Vec2 B){ return (A.x * B.y - A.y * B.x); }

float Vec2DotProduct(Vec2 A, Vec2 B){ return (A.x * B.x + A.y * B.y); }

void Vec2cale(Vec2 *vec, float n) { (*vec) = (Vec2){ vec->x * n, vec->y * n }; }
