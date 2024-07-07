#ifndef __GAME_UTILS__
#define __GAME_UTILS__

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

inline void freeAndNullify(void** ptr)
{
	if (ptr != NULL && *ptr != NULL)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

inline void freeBitmap(void* userdata, void* bitmap)
{
	PlaydateAPI* api = userdata;
	api->graphics->freeBitmap(bitmap);
	bitmap = NULL;
}

inline void freeSprite(void* userdata, void* sprite)
{
	PlaydateAPI* api = userdata;
	api->sprite->freeSprite(sprite);
	sprite = NULL;
}
#endif // !__GAME_UTILS__
