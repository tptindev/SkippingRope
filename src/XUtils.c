#include "XUtils.h"
#include "pd_api.h"

int randIntIn(int min, int max)
{
    return rand() % (max + 1 - min) + min;
}

float randReal(void)
{
    return (float)(rand()) / (float)(RAND_MAX);
}

float randRealIn(int min, int max)
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

bool randBit(void)
{
    return (bool)(rand() % (1));
}

void freeObjPtr(void* ptr)
{
    if (ptr != NULL)
    {
        free(ptr);
        ptr = NULL;
    }
}

void freeBitmap(void* pd_ptr, void* bitmap)
{
    if (bitmap != NULL && pd_ptr != NULL)
    {
        PlaydateAPI* api = pd_ptr;
        api->graphics->freeBitmap(bitmap);
    }
    bitmap = NULL;
}

void freeSprite(void* pd_ptr, void* sprite)
{
    if (sprite != NULL && pd_ptr != NULL)
    {
        PlaydateAPI* api = pd_ptr;
        api->sprite->removeSprite(sprite);
        api->sprite->freeSprite(sprite);
    }
    sprite = NULL;
}
