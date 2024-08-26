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
void fsSize(long *size, const char* filePath);
void fsRead(char *buffer, const char* filePath, long file_size);
void fsWrite(char *buffer, const char* filePath);
#endif // !__UTILS__
