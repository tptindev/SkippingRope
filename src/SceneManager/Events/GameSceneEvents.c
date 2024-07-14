#include "GameSceneEvents.h"
#include "../SceneManager.h"
#include "SceneIDs.h"
#include "../../ecs/entity.h"

void EVT_GAME_BACK_MENU_FUNC(void *userdata)
{
    Scene* scene = userdata;
    SceneManager* manager = scene->manager;
    SceneManagerTransition(manager, MENU_SCENE);
}

void EVT_GAME_EARTH_COLLIDED_FUNC(void* a, void* b)
{
    Entity* earth = a;
    Entity* enemy = b;
    (void)earth;
    (void)enemy;
    if (earth != NULL)
    {
        if (earth->components.health != NULL)
        {
            earth->components.health->current-=20;
        }
    }
}

void EVT_GAME_MOON_COLLIDED_FUNC(void* a, void* b)
{
    Entity* moon = a;
    Entity* enemy = b;
    (void)moon;
    (void)enemy;
}

void EVT_GAME_ENEMY_COLLIDED_FUNC(void* a, void* b)
{
    Entity* enemy = a;
    Entity* other = b;
    (void)enemy;
    (void)other;
    if (enemy != NULL)
    {
        if (enemy->components.health != NULL)
        {
            enemy->components.health->current = 0;
        }
    }
}
