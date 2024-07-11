#include "ecs/entity.h"
#include "Physics2D/World2D.h"

void EarthHealth(void* pd_ptr, World2D* world, Entity** earth, void* heath);
void MoonHealth(void* pd_ptr, World2D* world, Entity** moon, void* heath);
void EnemyHealth(void* pd_ptr, World2D* world, Entity** enemy, void* heath);