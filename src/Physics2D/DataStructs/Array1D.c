#include "Array1D.h"
#include <stdio.h>
#include <stdlib.h>

Array1D *CreateArray1D()
{
    Array1D* arr = (Array1D*)malloc(sizeof(Array1D));
    arr->size = 0;
    arr->capacity = 5;
    arr->array = malloc(sizeof(void*) * arr->capacity);
    return arr;
}

void Array1DPush(Array1D *arr, void *el)
{
    if (arr->size >= arr->capacity)
    {
        arr->capacity += 5;
        void** temp = realloc(arr->array, (sizeof(void*) * arr->capacity));
        if (!temp)
        {
            printf("ERROR: Couldn't realloc memory!\n");
            return;
        }
        arr->array  =  temp;
    }

    arr->array[arr->size] = el;
    arr->size++;
}

void Array1DInsert(Array1D *arr, void *el, size_t index)
{
    if (index < arr->size)
    {
        for (size_t i = arr->capacity; i > index; i--) {
            arr->array[i] = arr->array[i - 1];
        }
        arr->array[index] = el;
        arr->size++;
    }
    else // greater or equals  array size
    {
        Array1DPush(arr, el);
    }
}

void Array1DDelete(Array1D *arr, size_t index)
{
    for (size_t i = index; i < arr->capacity; ++i) {
        arr->array[i] = arr->array[i+1];
    }

    arr->size--;
    arr->capacity--;

    arr->array = (void*)realloc(arr->array, (sizeof(void*) * arr->capacity));
}

void Array1DTraverse(Array1D *arr, void (*cb)(void *))
{
    for (size_t i = 0; i < arr->size; ++i) {
        cb(arr->array[i]);
    }
}

void Array1DClear(Array1D *arr)
{
    if (arr != NULL)
    {
        free(arr->array);
        free(arr);
        arr = NULL;
    }
}

void *Array1DItemAtIndex(Array1D *arr, size_t index)
{
    if (index < arr->size)
    {
        return arr->array[index];
    }
    return NULL;
}
