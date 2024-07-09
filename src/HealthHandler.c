#include "HealthHandler.h"
#include "ecs/component.h"
#include "pd_api.h"
void EarthHealth(void* userdata, World2D* world, Entity** earth, void* heath)
{
    Health* _health = heath;
    _health->current -= 20;

    if (_health->current <= 0)
    {
        // lose
    }
}
void MoonHealth(void* userdata, World2D* world, Entity** moon, void* heath)
{

}

void EnemyHealth(void* userdata, World2D* world, Entity** enemy, void* heath)
{
    Health* _health = heath;
    if (_health->current <= 0)
    {
        DestroyEntity(userdata, enemy, world);
    }
}
