#include "HealthHandler.h"
#include "ecs/component.h"
void EarthHealth(void* pd_ptr, World2D* world, Entity** earth, void* heath)
{
    Health* _health = heath;
    _health->current -= 20;

    if (_health->current <= 0)
    {
        // lose
    }
}
void MoonHealth(void* pd_ptr, World2D* world, Entity** moon, void* heath)
{

}

void EnemyHealth(void* pd_ptr, World2D* world, Entity** enemy, void* heath)
{
    Health* _health = heath;
    if (_health->current <= 0)
    {
        DestroyEntity(pd_ptr, enemy, world);
    }
}
