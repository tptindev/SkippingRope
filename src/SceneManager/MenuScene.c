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
            AddButtonImageComponent(pd_ptr, start_btn, ACTIVE, RELEASE, "images/menu/buttons/start", 0.5, 2);
        }
    }
    {
        Entity* exit_btn = CreateEntity(scene->world, (Vec2) {2.5f, 2.0f}, (Vec2) {0.0f, 0.0f}, (Vec2) {0.0f, 0.0f});
        if (exit_btn != NULL)
        {
            SceneAddGameObject(scene, exit_btn);
            // Add Components
            AddButtonImageComponent(pd_ptr, exit_btn, NORMAL, RELEASE, "images/menu/buttons/exit", 0.5, 2);
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
        UpdateButtonImage(pd_ptr, entity);
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
        case kButtonUp:
        {
            Entity* entity = Array1DItemAtIndex(scene->entites, 0);
            entity->components.button_img->status = ACTIVE;
            break;
        }
        case kButtonB:
            SceneManagerTransition(manager, GAME);
            break;
        default:
            break;
        }
    }
}
