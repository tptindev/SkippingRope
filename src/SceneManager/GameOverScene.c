#include "GameOverScene.h"
#include "pd_api.h"
#include "../ecs/entity.h"
#include "../ecs/system.h"

void GameOverSceneInit(void *pd_ptr, Scene *scene)
{
    {
        Entity* menu_bg = CreateEntity(scene->world, (Vec2) {0.0f, 0.0f}, (Vec2) {0.0f, 0.0f}, (Vec2) {0.0f, 0.0f});
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
}

void GameOverSceneUpdate(void *pd_ptr, Scene *scene, float dt)
{
    if (scene == NULL || pd_ptr == NULL) return;
    unsigned int tick = ((PlaydateAPI*)pd_ptr)->system->getCurrentTimeMilliseconds();

    Entity* entity = NULL;
    for (size_t i = 0; i < scene->entites->size; i++)
    {
        entity = Array1DItemAtIndex(scene->entites, i);
        if (entity != NULL)
        {
            UpdateScale(entity, 1);
            UpdateRotation(entity);
            UpdateSprite(entity, tick);
            UpdateAnimateSprite(entity, tick);
            UpdateButtonImage(entity, scene->manager);
        }
    }
}

void GameOverSceneRender(void *pd_ptr, Scene *scene)
{
    if (scene == NULL || pd_ptr == NULL) return;
    Entity* entity = NULL;
    for (size_t i = 0; i < scene->entites->size; i++)
    {
        entity = Array1DItemAtIndex(scene->entites, i);
        if (entity != NULL)
        {
            UpdateRenderer(pd_ptr, entity);
        }
    }
}
