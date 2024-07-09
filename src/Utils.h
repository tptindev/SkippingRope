#ifndef __GAME_UTILS__
#define __GAME_UTILS__

#include <stdlib.h>
#include "pd_api.h"

inline void freeObjPtr(void** ptr)
{
	if (ptr != NULL)
	{
		if (*ptr != NULL)
		{
			free(*ptr);
			*ptr = NULL;
		}
	}
}

inline void freeBitmap(void* userdata, void* bitmap)
{
	if (bitmap != NULL && userdata != NULL)
	{
		PlaydateAPI* api = userdata;
		api->graphics->freeBitmap(bitmap);
		bitmap = NULL;
	}
}

inline void freeSprite(void* userdata, void* sprite)
{
	if (sprite != NULL && userdata != NULL)
	{
		PlaydateAPI* api = userdata;
		api->sprite->freeSprite(sprite);
		sprite = NULL;
	}
}
#endif // !__GAME_UTILS__
