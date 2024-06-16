#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>

struct MemoryManager
{
    size_t allocated;
};

void MemoryManagerTracking(struct MemoryManager *manager, size_t newAlloc)
{
    manager->allocated += newAlloc;
}
#endif // MEMORY_H
