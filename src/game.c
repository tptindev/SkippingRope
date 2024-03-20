#include "game.h"
#include "render.h"
#include "model.h"

PlaydateAPI *api = NULL;

static const char* sprite_paths[2] = { "images/bunny/1", "images/bunny/2" };
static Character bot = {
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
    api = pdAPI;

    callback_register();
    sprite_register();

}

void callback_register()
{
    // Note: If you set an update callback in the kEventInit handler, the system assumes the game is pure C and doesn't run any Lua code in the game
    api->system->setUpdateCallback(update, NULL);
    api->system->setButtonCallback(buttonCbFunc, NULL, 2);
}

void sprite_register()
{

    // load textures: start
    load_textures(api, bot.paths, bot.sprites, sizeof(sprite_paths) / sizeof(char*));
    // load textures: end
}

int update(void *userdata)
{
    update_sprites(userdata, bot.sprites, 50, 50, 0, kBitmapUnflipped, sizeof(sprite_paths) / sizeof(char *));

    api->sprite->drawSprites();

    return 1;
}

int buttonCbFunc(PDButtons button, int down, uint32_t when, void *userdata)
{
    switch (button)
    {
    case kButtonUp:
    {
        break;
    }
    default:
        break;
    }
}