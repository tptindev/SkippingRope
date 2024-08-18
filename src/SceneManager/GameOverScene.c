#include "GameOverScene.h"
#include "pd_api.h"
#include "../ecs/entity.h"
#include "../ecs/system.h"
#include "../ecs/EntityIDs.h"
#include "../SceneManager/Events/EventIDs.h"
#include <stdio.h>

void GameOverSceneInit(void *pd_ptr, Scene *scene)
{
    PlaydateAPI* api = pd_ptr;
    {
        Entity* new_game_btn = CreateEntity(ENTITY_BTN_NEW_GAME, scene->world, (Vec2) {1.1f, 2.0f}, (Vec2) {0.0f, 0.0f}, (Vec2) {0.0f, 0.0f});
        if (new_game_btn != NULL) {
            SceneAddGameObject(scene, new_game_btn);
            // Add components
            AddButtonImageComponent(pd_ptr, new_game_btn, ACTIVE, EVT_GAME_NEW_GAME, "images/gameover/buttons/new_game", 0.0f, 2);
        }
    }
    {
        Entity* quit_game_btn = CreateEntity(ENTITY_BTN_QUIT_GAME, scene->world, (Vec2) {3.1f, 2.0f}, (Vec2) {0.0f, 0.0f}, (Vec2) {0.0f, 0.0f});
        if (quit_game_btn != NULL) {
            SceneAddGameObject(scene, quit_game_btn);
            // Add components
            AddButtonImageComponent(pd_ptr, quit_game_btn, NORMAL, EVT_GAME_QUIT_GAME, "images/gameover/buttons/quit_game", 0.0f, 2);
        }
    }
    {
        Entity* score_board = CreateEntity(ENTITY_SCORE_BOARD_VISUAL, scene->world, (Vec2) {2.6f, 1.5f}, (Vec2) {0.0f, 0.0f}, (Vec2) {1.0f, 1.0f});
        if (score_board != NULL)
        {
            SceneAddGameObject(scene, score_board);
            // Add Components
            AddScoreBoardComponent(pd_ptr, score_board);
            AddScoreBoardVisualComponent(pd_ptr, score_board, "images/numbers/white", 2);
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
        int current_idx = ((int)angle % 180) / (180 / 2);
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
            UpdateScoreBoard(pd_ptr, entity);
            UpdateScoreBoardVisual(pd_ptr, entity);
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
