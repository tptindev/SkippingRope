#include "GameSceneEvents.h"
#include "SceneIDs.h"

void EVT_GAME_BACK_MENU_FUNC(SceneManager* manager)
{
    SceneManagerTransition(manager, MENU_SCENE);
}

void EVT_GAME_EARTH_COLLIDED_FUNC(Entity* entity, Entity* other)
{
    if (entity != NULL)
    {
        if (entity->components.health != NULL && other->components.strength != NULL)
        {
            entity->components.health->current -= other->components.strength->damage;
        }
    }
}

void EVT_GAME_MOON_COLLIDED_FUNC(Entity* entity, Entity* other)
{
}

void EVT_GAME_ENEMY_COLLIDED_FUNC(Entity* entity, Entity* other)
{
    if (entity != NULL)
    {
        if (entity->components.health != NULL)
        {
            entity->components.health->current = 0;
        }
    }
}

void EVT_GAME_EARTH_DEAD_FUNC(SceneManager* manager)
{
    SceneManagerTransition(manager, GAME_OVER_SCENE);
}

void EVT_GAME_ENEMY_DEAD_FUNC(SceneManager* manager, Entity* entity)
{
    SceneRemoveGameObject(manager->current_scene, entity->id);
    DestroyEntity(manager->pd, entity);
}
