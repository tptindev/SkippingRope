#ifndef ARRAY1D_H
#define ARRAY1D_H

#include <stdbool.h>
#include <stddef.h>

typedef struct
{
    void* (*array);
    size_t capacity;
    size_t size;
} Array1D;

Array1D* CreateArray1D();
void *Array1DItemAtIndex(Array1D* arr, size_t index);
void Array1DPush(Array1D* arr, void* el);
void Array1DInsert(Array1D* arr, void* el, size_t index);
void Array1DDelete(Array1D* arr, size_t index);
void Array1DTraverse(Array1D* arr, void(*cb)(void*));
void Array1DClear(Array1D* arr);
inline size_t Array1DTotalSize(Array1D* arr) { return arr->size; }

#endif // ARRAY1D_H
