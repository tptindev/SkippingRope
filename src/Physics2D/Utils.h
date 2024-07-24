#ifndef UTILS__H
#define UTILS__H

#include <stdlib.h>
#include <stdbool.h>

inline int randIntIn(int min, int max)
{
    return rand() % (max + 1 - min) + min;
}

inline float randReal()
{
    return (float)(rand()) / (float)(RAND_MAX);
}

inline float randRealIn(int min, int max)
{
    int r = randIntIn(min, max);
    if (r == max)
    {
        r = max;
    }
    else if (r == min)
    {
        r = min;
    }
    return (float)r + randReal();
}

inline bool randBit()
{
    return (bool)(rand() % (1));
}

#endif // UTILS__H
