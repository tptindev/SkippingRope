#ifndef GAMESCENEEVENTS_H
#define GAMESCENEEVENTS_H

#include "../SceneManager.h"
#include "../../ecs/entity.h"

void EVT_GAME_BACK_MENU_FUNC(SceneManager* manager);
void EVT_GAME_EARTH_DEAD_FUNC(SceneManager* manager, Entity* enemy);
void EVT_GAME_ENEMY_DEAD_FUNC(SceneManager* manager, Entity* enemy);
void EVT_GAME_EARTH_COLLIDED_FUNC(Entity* entity, Entity* other);
void EVT_GAME_MOON_COLLIDED_FUNC(Entity* entity, Entity* other);
void EVT_GAME_ENEMY_COLLIDED_FUNC(Entity* entity, Entity* other);
void EVT_GAME_EARTH_HIT_FUNC(SceneManager* manager, Entity* entity, Entity* other);
void EVT_GAME_MOON_HIT_FUNC(SceneManager* manager, Entity* entity, Entity* other);
#endif // GAMESCENEEVENTS_H
