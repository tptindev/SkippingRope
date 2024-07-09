#include "HealthHandler.h"
#include "ecs/component.h"
#include "pd_api.h"
void EarthHealth(void* userdata, Entity** earth, void* heath)
{
    Health* _health = heath;
    _health->current -= 20;

    if (_health->current <= 0)
    {
        // lose
    }
}
void MoonHealth(void* userdata, Entity** moon, void* heath)
{

}

void EnemyHealth(void* userdata, Entity** enemy, void* heath)
{
    Health* _health = heath;
    if (_health->current <= 0)
    {
        FreeEntity(userdata, enemy);
    }
}
