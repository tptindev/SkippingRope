#include "game.h"
#include "render.h"
#include "model.h"
#include "playbox.h"

PlaydateAPI *pd = NULL;

static const char* sprite_paths[2] = { "images/bunny/0", "images/bunny/1" };
Character bot = {
    "basic",
    sprite_paths,
    NULL,
    0,
    0,
    0,
    200,
    1
};

void game_initialize(PlaydateAPI *pdAPI)
{
    pd = pdAPI;
    pd->display->setRefreshRate(8.0f);
    register_lua_functions();
    register_sprites();
    registerPlaybox();
}

void register_sprites()
{
    // load textures: start
    load_textures(pd, bot.paths, bot.sprites, 2);
    // load textures: end
}

void register_lua_functions()
{
    const char* outerr;
    int status = pd->lua->addFunction(update, "playdate.system.c_update", &outerr);
    if (status == 0)
    {
        pd->system->logToConsole("%s:%i: addFunction failed, %s", __FILE__, __LINE__, outerr);
    }
}

void update(lua_State* state)
{
    update_sprites(pd, bot.sprites, 50, 50, 1, 2, kBitmapUnflipped);
    pd->sprite->updateAndDrawSprites();
}

