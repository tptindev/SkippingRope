#include "Array2D.h"
#include <stdio.h>
#include <stdlib.h>

Array2D* CreateArray2D(size_t w, size_t h)
{
    Array2D* arr = (Array2D*)malloc(sizeof(Array2D));
    arr->w = w;
    arr->h = h;
    arr->data = malloc(w * h * sizeof(void*));
    return arr;
}



int Array2DIndexAt(Array2D *arr, unsigned int x, unsigned int y)
{
    if (arr == NULL) return -1;
    if (arr->w > x && arr->h > y)
    {
        return (int)((arr->w * y) + x);
    }
    return -1;
}

void *Array2DItemAt(Array2D *arr, unsigned int x, unsigned int y)
{
    int index = Array2DIndexAt(arr, x, y);
    if (index > -1)
    {
        return arr->data[index];
    }
    return NULL;
}

void Array2DClear(Array2D *arr)
{
    if (arr != NULL)
    {
        free(arr->data);
        free(arr);
        arr = NULL;
    }
}

void Array2DInsert(Array2D *arr, int cellIndex, void *el)
{
    Vec2 coord = Array2DCoordIndex(arr, cellIndex);
    void* item = Array2DItemAt(arr, (unsigned int)coord.x, (unsigned int)coord.y);
    if (item == NULL)
    {
        fprintf(stderr, "ERROR: Couldn't realloc memory!\n");
        return;
    }
    item = el;
    (void)item;
}

Vec2 Array2DCoordIndex(Array2D *arr, int cellIndex)
{
    size_t x = (cellIndex % arr->w);
    size_t y = (cellIndex / arr->w);
    return (Vec2) { (float)x, (float)y };
}
