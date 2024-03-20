#include "pd_api.h"

extern PlaydateAPI *pd;
void game_initialize(PlaydateAPI* pdAPI);
void callback_register();
void sprite_register();
static int update(void* userdata);
static int buttonCbFunc(PDButtons button, int down, uint32_t when, void* userdata);