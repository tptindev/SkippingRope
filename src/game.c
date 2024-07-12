#include "game.h"
#include "pd_api.h"
#include "CollisionHandler.h"
#include "HealthHandler.h"
#include "SceneManager/SceneManager.h"
#include "SceneManager/Scene.h"
#include "SceneManager/MenuScene.h"
#include "SceneManager/GameScene.h"

float world_scale = 80.0f;
static PlaydateAPI* api = NULL;
static World2D* world = NULL;
static SceneManager* scene_manager = NULL;

typedef enum
{
    MENU,
    GAME,
    GAME_OVER,
} SceneID;

void game_initialize(void* pd_ptr)
{	
    api = pd_ptr;
	world = CreateWorld((Vec2) { 10.0f, 10.0f }, 5.0f, 3.0f);

    scene_manager = CreateSceneManager();

    Scene* menu_scene = NULL;
    Scene* game_scene = NULL;

    menu_scene = CreateScene(MENU, world);
    game_scene = CreateScene(GAME, world);

    SceneManagerAddScene(scene_manager, menu_scene);
    SceneManagerAddScene(scene_manager, game_scene);

    MenuSceneInit(api, menu_scene);
    GameSceneInit(api, game_scene);

    SceneManagerActiveScene(scene_manager, game_scene);

    api->system->setButtonCallback(NULL, scene_manager, 5);
}

void game_update(float dt)
{
    if (scene_manager->current_scene->id == MENU)
    {
        MenuSceneUpdate(api, scene_manager->current_scene, dt);
    }
    else if (scene_manager->current_scene->id == GAME)
    {
        GameSceneUpdate(api, scene_manager->current_scene, dt);
    }
}

void game_draw()
{
    api->graphics->clear(kColorWhite);
    api->graphics->setBackgroundColor(kColorWhite);
	api->sprite->removeAllSprites();
    if (scene_manager->current_scene->id == MENU)
    {
        MenuSceneRender(api, scene_manager->current_scene);
    }
    else if (scene_manager->current_scene->id == GAME)
    {
        GameSceneRender(api, scene_manager->current_scene);
    }
	api->sprite->drawSprites();
}

void game_destroy()
{
    FreeSceneManager(api, scene_manager);
	DestroyWorld(world);
}
