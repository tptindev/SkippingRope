#include "MenuScene.h"
#include "pd_api.h"
#include "../ecs/entity.h"
#include "../ecs/system.h"
#include "SceneManager.h"
#include "SceneIDs.h"

void MenuSceneInit(void* pd_ptr, Scene *scene)
{
    {
        Entity* menu_bg = CreateEntity(scene->world, (Vec2) {0.0f, 0.0f}, (Vec2) {0.0f, 0.0f}, (Vec2) {0.0f, 0.0f});
        if (menu_bg != NULL)
        {
            SceneAddGameObject(scene, menu_bg);
            // Add Components
            AddSpriteComponent(pd_ptr, menu_bg, "images/menu/menu_bg", false, 0.0f, 1);
        }
    }
    {
        Entity* start_btn = CreateEntity(scene->world, (Vec2) {2.5f, 1.0f}, (Vec2) {0.0f, 0.0f}, (Vec2) {0.0f, 0.0f});
        if (start_btn != NULL)
        {
            SceneAddGameObject(scene, start_btn);
            // Add Components
            const char* sources[2] = {"images/menu/buttons/start_normal", "images/menu/buttons/start_active"};
            AddSpriteComponent(pd_ptr, start_btn, sources[0], false, 0.5f, 1);
            AddButtonComponent(pd_ptr, start_btn, ACTIVE, RELEASE, sources[0], sources[1]);
        }
    }
    {
        Entity* exit_btn = CreateEntity(scene->world, (Vec2) {2.5f, 2.0f}, (Vec2) {0.0f, 0.0f}, (Vec2) {0.0f, 0.0f});
        if (exit_btn != NULL)
        {
            SceneAddGameObject(scene, exit_btn);
            // Add Components
            const char* sources[2] = {"images/menu/buttons/exit_normal", "images/menu/buttons/exit_active"};
            AddSpriteComponent(pd_ptr, exit_btn, sources[0], false, 0.5f, 1);
            AddButtonComponent(pd_ptr, exit_btn, NORMAL, RELEASE, sources[0], sources[1]);
        }
    }
}

void MenuSceneUpdate(void* pd_ptr, Scene *scene, float dt)
{
    if (scene == NULL || pd_ptr == NULL) return;
    unsigned int tick = ((PlaydateAPI*)pd_ptr)->system->getCurrentTimeMilliseconds();

    Entity* entity = NULL;
    for (size_t i = 0; i < scene->entites->size; i++)
    {
        entity = Array1DItemAtIndex(scene->entites, i);
        UpdateScale(entity, 1);
        UpdateRotation(entity);
        UpdateSprite(entity, tick);
        UpdateAnimateSprite(entity, tick);
    }
}

void MenuSceneRender(void* pd_ptr, Scene *scene)
{
    if (scene == NULL || pd_ptr == NULL) return;
    Entity* entity = NULL;
    for (size_t i = 0; i < scene->entites->size; i++)
    {
        entity = Array1DItemAtIndex(scene->entites, i);
        UpdateRenderer(pd_ptr, entity);
    }
}

void MenuSceneEvent(void *pd_ptr, Scene *scene, void* manager)
{
    if (scene == NULL || pd_ptr == NULL) return;
    PlaydateAPI* api = pd_ptr;
    { // system
        PDButtons current;
        PDButtons pushed;
        PDButtons released;
        api->system->getButtonState(&current, &pushed, &released);
        switch (pushed) {
        case kButtonB:
            SceneManagerTransition(manager, GAME);
            break;
        default:
            break;
        }
    }
}
