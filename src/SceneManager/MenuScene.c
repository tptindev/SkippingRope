#include "MenuScene.h"
#include "pd_api.h"
#include "../ecs/entity.h"
#include "../ecs/system.h"
#include "../ecs/EntityIDs.h"
#include "Events/EventIDs.h"
void MenuSceneInit(void* pd_ptr, Scene *scene)
{
    {
        Entity* start_btn = CreateEntity(ENTITY_BTN_START, scene->world, (Vec2) {2.5f, 1.0f}, (Vec2) {0.0f, 0.0f}, (Vec2) {0.0f, 0.0f});
        if (start_btn != NULL)
        {
            SceneAddGameObject(scene, start_btn);
            // Add Components
            AddButtonImageComponent(pd_ptr, start_btn, ACTIVE, EVT_MENU_START, "images/menu/buttons/start", 0.5f, 2);
        }
    }
    {
        Entity* exit_btn = CreateEntity(ENTITY_BTN_EXIT, scene->world, (Vec2) {2.5f, 2.0f}, (Vec2) {0.0f, 0.0f}, (Vec2) {0.0f, 0.0f});
        if (exit_btn != NULL)
        {
            SceneAddGameObject(scene, exit_btn);
            // Add Components
            AddButtonImageComponent(pd_ptr, exit_btn, NORMAL, EVT_MENU_EXIT, "images/menu/buttons/exit", 0.5f, 2);
        }
    }
    {
        Entity* menu_bg = CreateEntity(ENTITY_BG_MENU, scene->world, (Vec2) {0.0f, 0.0f}, (Vec2) {0.0f, 0.0f}, (Vec2) {0.0f, 0.0f});
        if (menu_bg != NULL)
        {
            SceneAddGameObject(scene, menu_bg);
            // Add Components
            AddSpriteComponent(pd_ptr, menu_bg, "images/menu/menu_bg", false, 0.0f, 1);
        }
    }
}

void MenuSceneUpdate(void* pd_ptr, Scene *scene, float dt)
{
    if (scene == NULL || pd_ptr == NULL) return;
    unsigned int tick = ((PlaydateAPI*)pd_ptr)->system->getCurrentTimeMilliseconds();

    Entity* entity = NULL;
    for (size_t i = 0; i < scene->entities->size; i++)
    {
        entity = Array1DItemAtIndex(scene->entities, i);
        if (entity != NULL)
        {
            UpdateScale(entity, 1);
            UpdateRotation(entity);
            UpdateSprite(entity, tick);
            UpdateAnimateSprite(entity, tick);
            UpdateButtonImage(pd_ptr, entity, scene->manager);
        }
    }
}

void MenuSceneRender(void* pd_ptr, Scene *scene)
{
    if (scene == NULL || pd_ptr == NULL) return;
    Entity* entity = NULL;
    for (size_t i = 0; i < scene->entities->size; i++)
    {
        entity = Array1DItemAtIndex(scene->entities, i);
        if (entity != NULL)
        {
            UpdateRenderer(pd_ptr, entity);
        }
    }
}

void MenuSceneEvent(void *pd_ptr, Scene *scene, void* manager)
{
    if (scene == NULL || pd_ptr == NULL) return;
    PlaydateAPI* api = pd_ptr;
    { // system
        float angle = api->system->getCrankAngle();
        int current_idx = ((int)angle % 180) / (180 / (int)(scene->entities->size - 1));
        Entity* entity = NULL;
        for (size_t i = 0; i < scene->entities->size; i++)
        {
            entity = Array1DItemAtIndex(scene->entities, i);
            if (entity == NULL) continue;
            if (i == current_idx)
            {
                if (entity->components.button_img != NULL)
                {
                    entity->components.button_img->state = ACTIVE;
                }
            }
            else
            {
                if (entity->components.button_img != NULL)
                {
                    entity->components.button_img->state = NORMAL;
                }
            }
            entity = NULL;
        }

        PDButtons current;
        PDButtons pushed;
        PDButtons released;
        api->system->getButtonState(&current, &pushed, &released);
        switch (current) {
        case kButtonUp:
        {
            break;
        }
        case kButtonB:
            if (current == pushed)
            {
            }
            break;
        case kButtonA:
        {
            entity = Array1DItemAtIndex(scene->entities, current_idx);
            if (entity != NULL)
            {
                if (entity->components.button_img != NULL)
                {
                    if (current == pushed)
                    {
                        entity->components.button_img->state = PUSHED;
                    }
                    else
                    {
                        entity->components.button_img->state = RELEASE;
                    }
                }
            }
            break;
        }
        default:
            break;
        }
    }
}
