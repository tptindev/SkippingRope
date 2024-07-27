#include "GameOverScene.h"
#include "pd_api.h"
#include "../ecs/entity.h"
#include "../ecs/system.h"
#include "../ecs/EntityIDs.h"
#include "../SceneManager/Events/EventIDs.h"

void GameOverSceneInit(void *pd_ptr, Scene *scene)
{
    {
        Entity* new_game_btn = CreateEntity(ENTITY_BTN_NEW_GAME, scene->world, (Vec2) {1.1f, 2.0f}, (Vec2) {0.0f, 0.0f}, (Vec2) {0.0f, 0.0f});
        if (new_game_btn != NULL) {
            SceneAddGameObject(scene, new_game_btn);
            // Add components
            AddButtonImageComponent(pd_ptr, new_game_btn, ACTIVE, EVT_GAME_NEW_GAME, "image/gameover/buttons/new_game", 0.0f, 2);
        }
    }
    {
        Entity* quit_game_btn = CreateEntity(ENTITY_BTN_NEW_GAME, scene->world, (Vec2) {3.1f, 2.0f}, (Vec2) {0.0f, 0.0f}, (Vec2) {0.0f, 0.0f});
        if (quit_game_btn != NULL) {
            SceneAddGameObject(scene, quit_game_btn);
            // Add components
            AddButtonImageComponent(pd_ptr, quit_game_btn, NORMAL, EVT_GAME_QUIT_GAME, "image/gameover/buttons/quit_game", 0.0f, 2);
        }
    }
    {
        Entity* menu_bg = CreateEntity(ENTITY_BG_GAME_OVER, scene->world, (Vec2) {0.0f, 0.0f}, (Vec2) {0.0f, 0.0f}, (Vec2) {0.0f, 0.0f});
        if (menu_bg != NULL)
        {
            SceneAddGameObject(scene, menu_bg);
            // Add Components
            AddSpriteComponent(pd_ptr, menu_bg, "images/gameover/game_over_bg", false, 0.0f, 1);
        }
    }
}

void GameOverSceneEvent(void *pd_ptr, Scene *scene, void *manager)
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

void GameOverSceneUpdate(void *pd_ptr, Scene *scene, float dt)
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

void GameOverSceneRender(void *pd_ptr, Scene *scene)
{
    if (scene == NULL || pd_ptr == NULL) return;
    if (scene->active == false) return;
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
