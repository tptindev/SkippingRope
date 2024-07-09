#include "ecs/entity.h"
#include "Physics2D/World2D.h"

void EarthHealth(void* userdata, World2D* world, Entity** earth, void* heath);
void MoonHealth(void* userdata, World2D* world, Entity** moon, void* heath);
void EnemyHealth(void* userdata, World2D* world, Entity** enemy, void* heath);