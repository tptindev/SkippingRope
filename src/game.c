#include "game.h"
#include "render.h"
#include "model.h"
#include "platform.h"

static float deltatime = 0.0f;
int gamelib_newobj(lua_State* L)
{
    int results = 0; 
    { 
        Character* bot = pd->system->realloc(NULL, sizeof(Character));
        bot->id = "Basic";
        bot->capacity = 2;
        bot->paths = pd->system->realloc(NULL, sizeof(char*) * bot->capacity);
        bot->sprites = pd->system->realloc(NULL, sizeof(LCDSprite*) * bot->capacity);
        *(bot->paths + 0) = "images/bunny/0";
        *(bot->paths + 1) = "images/bunny/1";
        *(bot->sprites + 0) = pd->sprite->newSprite();
        *(bot->sprites + 1) = pd->sprite->newSprite();
        LuaUDObject* ud = pd->lua->pushObject(bot, "c_game", 0);
        if (ud != NULL) results += 1;
    }

    {

    }

    return 1;
}

int game_initialize(lua_State* L)
{
    pd->display->setRefreshRate(8.0f);
    Character* bot = pd->lua->getArgObject(1, "c_game", NULL);
    load_textures(pd, bot->paths, bot->sprites, bot->capacity);
    return 0;
}
int game_update(lua_State* L)
{
    deltatime = pd->lua->getArgFloat(1);

    {
        Character* bot = pd->lua->getArgObject(1, "c_game", NULL);
        update_sprites(pd, bot->sprites, 50, 50, 1, bot->capacity, kBitmapUnflipped);
    }

    return 0;
}

int game_draw(lua_State* L)
{
    return 0;
}

const lua_reg gamelib[5] =
{
    {"game_new", gamelib_newobj},
    {"game_initialize", game_initialize},
    {"game_update", game_update},
    {"game_draw", game_draw},
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