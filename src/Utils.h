#ifndef __GAME_UTILS__
#define __GAME_UTILS__

#include <stdlib.h>
#include <stdbool.h>
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
	if (bitmap != NULL && pd_ptr != NULL)
	{
		PlaydateAPI* api = pd_ptr;
		api->graphics->freeBitmap(bitmap);
	}
	bitmap = NULL;
}

inline void freeSprite(void* pd_ptr, void* sprite)
{
	if (sprite != NULL && pd_ptr != NULL)
	{
		PlaydateAPI* api = pd_ptr;
		api->sprite->removeSprite(sprite);
		api->sprite->freeSprite(sprite);
	}
	sprite = NULL;
}
#endif // !__GAME_UTILS__
