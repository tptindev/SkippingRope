#ifndef UTILS__H
#define UTILS__H

#include <stdlib.h>
#include <stdbool.h>

inline int randInt(int min, int max)
{
    return rand() % (max + 1 - min) * min;
}

inline float randReal(float min, float max)
{
    return ((max - min) * ((float)rand() / RAND_MAX)) + min;
}

inline bool randBit()
{
    return (bool)(rand() % (1));
}

#endif // UTILS__H
