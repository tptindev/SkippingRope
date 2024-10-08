#include "pd_api.h"
#include "game.h"
#include <time.h>

static unsigned int last_time;
static unsigned int current_time;
static int update(void* pd_ptr);

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t arg)
{
	(void)arg; // arg is currently only used for event = kEventKeyPressed

	if ( event == kEventInit )
	{
        srand(time(0));
		last_time = pd->system->getCurrentTimeMilliseconds();
        game_initialize(pd);

		// Note: If you set an update callback in the kEventInit handler, the system assumes the game is pure C and doesn't run any Lua code in the game
		pd->system->setUpdateCallback(update, pd);
	}
	
	return 0;
}


static int update(void* pd_ptr)
{
	PlaydateAPI* api = pd_ptr;
	current_time = api->system->getCurrentTimeMilliseconds();
	float dt = (float)(current_time - last_time)/1000.0f;
	game_update(dt);
	game_draw();
	last_time = current_time;
	return 1;
}
