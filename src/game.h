#include "pd_api.h"
#include "model.h"

static const lua_reg game_lib[4];
static int gamelib_newobj(lua_State*);
static int game_initialize(lua_State*);
static int game_update(lua_State*);
static int game_draw(lua_State*);
void register_lua_functions(PlaydateAPI*);
void register_lua_classes(PlaydateAPI*);