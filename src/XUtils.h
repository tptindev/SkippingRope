#ifndef __UTILS__
#define __UTILS__

#include <stdlib.h>
#include <stdbool.h>

int randIntIn(int min, int max);
float randReal(void);
float randRealIn(int min, int max);
bool randBit(void);
void freeObjPtr(void* ptr);
void freeBitmap(void* pd_ptr, void* bitmap);
void freeSprite(void* pd_ptr, void* sprite);
#endif // !__UTILS__
