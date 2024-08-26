#include "XUtils.h"
#include "pd_api.h"
#include <stdio.h>

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

void fsRead(char *buffer, const char *filePath, long file_size)
{
    errno_t err;
    FILE* filePtr = NULL;
    err = fopen_s(&filePtr, filePath, "r");
    if (err == 0)
    {
        buffer[file_size] = '\0'; // Null-terminate the buffer
        fgets(buffer, file_size, filePtr);
        fclose(filePtr);
    }
}

void fsWrite(char *buffer, const char *filePath)
{
    errno_t err;
    FILE* filePtr = NULL;
    err = fopen_s(&filePtr, filePath, "w+");
    if (err == 0)
    {
        fprintf(filePtr, "%s", buffer);
        fclose(filePtr);
    }
}


void fsSize(long *size, const char *filePath)
{
    errno_t err;
    FILE* filePtr = NULL;
    err = fopen_s(&filePtr, filePath, "r");
    if (err == 0)
    {
        fseek(filePtr, 0, SEEK_END);
        *size = ftell(filePtr);
        rewind(filePtr);
        fclose(filePtr);
    }
}
