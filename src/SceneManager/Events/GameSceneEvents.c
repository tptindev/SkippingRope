#include "GameSceneEvents.h"
#include "SceneIDs.h"
#include "../../ecs/EntityIDs.h"
#include "pd_api.h"

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

void EVT_GAME_ENEMY_DEAD_FUNC(SceneManager* manager, Entity* enemy)
{
    enemy->active = false;
    if (manager->current_scene != NULL)
    {
        for (size_t i = 0; i < manager->current_scene->entities_active->size; i++)
        {
            Entity* entity = Array1DItemAtIndex(manager->current_scene->entities_active, i);
            if (entity != NULL)
            {
                if (entity->id == enemy->id)
                {
                    if (entity->active == false)
                    {
                        Array1DDelete(manager->current_scene->entities_active, i);
                    }
                }
            }
        }
    }
//    SceneRemoveGameObject(manager->current_scene, entity->id);
//    DestroyEntity(manager->pd, entity);
}

void EVT_GAME_EARTH_HIT_FUNC(SceneManager* manager, Entity *entity, Entity* blood)
{
    if (entity != NULL && manager != NULL && blood != NULL)
    {
        PlaydateAPI* api = manager->pd;
        (void)api;
        switch (blood->id) {
        case ENTITY_EARTH_BLOOD:
        {
            if (entity->components.health != NULL)
            {
                float current = entity->components.health->current;
                float max = entity->components.health->max;
                if (blood->components.animated_sprite != NULL)
                {
                    if (blood->components.animated_sprite->_ptr != NULL)
                    {
                        int frame_count = blood->components.animated_sprite->frame_count - 1;
                        blood->components.animated_sprite->frame_index = (int)((current / max) * frame_count);
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

void EVT_GAME_MOON_HIT_FUNC(SceneManager* manager, Entity *entity, Entity* other)
{
    if (other != NULL && manager != NULL && other != NULL)
    {
        //        PlaydateAPI* api = manager->pd;
        switch (other->id) {
        case ENTITY_MOON_BLOOD:
        {
            if (entity->components.health != NULL)
            {
                float current = entity->components.health->current;
                float max = entity->components.health->max;
                if (other->components.animated_sprite != NULL)
                {
                    if (other->components.animated_sprite->_ptr != NULL)
                    {
                        int frame_count = other->components.animated_sprite->frame_count - 1;
                        other->components.animated_sprite->frame_index = (int)((current / max) * frame_count);
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
