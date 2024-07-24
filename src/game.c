#include "game.h"
#include "pd_api.h"
#include "SceneManager/SceneManager.h"
#include "SceneManager/Scene.h"
#include "SceneManager/MenuScene.h"
#include "SceneManager/GameScene.h"
#include "SceneManager/GameOverScene.h"
#include "SceneManager/Events/SceneIDs.h"
float world_scale = 80.0f;
static PlaydateAPI* api = NULL;
static World2D* world = NULL;
static SceneManager* scene_manager = NULL;


void game_initialize(void* pd_ptr)
{
    api = pd_ptr;
	world = CreateWorld((Vec2) { 10.0f, 10.0f }, 5.0f, 3.0f);

    scene_manager = CreateSceneManager(api);

    Scene* menu_scene = NULL;
    Scene* game_scene = NULL;
    Scene* game_over_scene = NULL;
    
    menu_scene = CreateScene(MENU_SCENE, world);
    game_scene = CreateScene(GAME_SCENE, world);
    game_over_scene = CreateScene(GAME_OVER_SCENE, world);

    SceneManagerAddScene(scene_manager, menu_scene);
    SceneManagerAddScene(scene_manager, game_scene);
    SceneManagerAddScene(scene_manager, game_over_scene);

    MenuSceneInit(api, menu_scene);
    GameSceneInit(api, game_scene);
    GameOverSceneInit(api, game_over_scene);

    SceneManagerActiveScene(scene_manager, menu_scene);
}

void game_update(float dt)
{
    if (scene_manager->current_scene->id == MENU_SCENE)
    {
        MenuSceneEvent(api, scene_manager->current_scene, scene_manager);
        MenuSceneUpdate(api, scene_manager->current_scene, dt);
    }
    else if (scene_manager->current_scene->id == GAME_SCENE)
    {
        GameSceneEvent(api, scene_manager->current_scene, scene_manager);
        GameSceneUpdate(api, scene_manager->current_scene, dt);
    }
    else if (scene_manager->current_scene->id == GAME_OVER_SCENE)
    {
        GameOverSceneEvent(api, scene_manager->current_scene, scene_manager);
        GameOverSceneUpdate(api, scene_manager->current_scene, dt);
    }
}

void game_draw()
{
    api->graphics->clear(kColorWhite);
    api->graphics->setBackgroundColor(kColorWhite);
    api->sprite->removeAllSprites();
    if (scene_manager->current_scene->id == MENU_SCENE)
    {
        MenuSceneRender(api, scene_manager->current_scene);
    }
    else if (scene_manager->current_scene->id == GAME_SCENE)
    {
        GameSceneRender(api, scene_manager->current_scene);
    }
    else if (scene_manager->current_scene->id == GAME_OVER_SCENE)
    {
        GameOverSceneRender(api, scene_manager->current_scene);
    }
    api->sprite->drawSprites();
}

void game_destroy()
{
    FreeSceneManager(api, scene_manager);
	DestroyWorld(world);
}
