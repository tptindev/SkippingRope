#include "game.h"
#include "render.h"
#include "model.h"
#include "platform.h"

static float deltatime = 0.0f;
int gamelib_newobj(lua_State* L)
{
    Character* bot = pd->system->realloc(NULL, sizeof(Character));
    bot->id = "Basic";
    bot->capacity = 2;
    bot->paths = pd->system->realloc(NULL, sizeof(char*) * bot->capacity);
    bot->sprites = pd->system->realloc(NULL, sizeof(LCDSprite*) * bot->capacity);
    pd->lua->pushObject(bot, "c_game", 0);
    return 1;
}

int game_initialize(lua_State* L)
{
    Character* bot = pd->lua->getArgObject(1, "c_game", NULL);
    return 0;
}
int game_update(lua_State* L)
{
    deltatime = pd->lua->getArgFloat(1);
    pd->system->logToConsole("%s deltatime: %f", __FUNCTION__, deltatime);
    return 0;
}

const lua_reg gamelib[4] =
{
    {"game_new", gamelib_newobj},
    {"game_initialize", game_initialize},
    {"game_update", game_update},
    { NULL, NULL }
};

void register_lua_classes(PlaydateAPI* pd)
{
    const char* outerr;
    int status = pd->lua->registerClass("c_game", gamelib, NULL, 0, &outerr);
    if (status == 0)
    {
        pd->system->logToConsole("%s:%i: registerClass failed, %s", __FILE__, __LINE__, outerr);
    }
}

void register_lua_functions(PlaydateAPI* pd)
{
}