#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>
#include "pd_api.h"

inline void freeObjPtr(void* ptr)
{
	if (ptr != NULL)
	{
		free(ptr);
		ptr = NULL;
	}
}

inline void freeBitmap(void* pd_ptr, void* bitmap)
{
	PlaydateAPI* api = pd_ptr;
	api->graphics->freeBitmap(bitmap);
	bitmap = NULL;
}

inline void freeSprite(void* pd_ptr, void* sprite)
{
	PlaydateAPI* api = pd_ptr;
	api->sprite->freeSprite(sprite);
	sprite = NULL;
}
#endif // MEMORY_H
