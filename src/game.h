#include "pd_api.h"
#include "model.h"

void game_initialize(PlaydateAPI* pdAPI);
void register_sprites();
void register_lua_functions();
static void update(lua_State* state);