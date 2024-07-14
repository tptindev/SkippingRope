#ifndef EVENTDEFINES_H
#define EVENTDEFINES_H

#include <stdlib.h>
#include "Event.h"
#include "EventIDs.h"
#include "MenuSceneEvents.h"
#include "GameSceneEvents.h"

Event MenuSceneEvents[2 ] = {
    {EVT_MENU_START, EVT_MENU_START_FUNC},
    {EVT_MENU_EXIT,  EVT_MENU_EXIT_FUNC},
};

Event GameSceneEvents[1] = {
    {EVT_GAME_BACK_MENU, EVT_GAME_BACK_MENU_FUNC},
};

CollisionEvent CollisionEvents[3] = {
    {EVT_GAME_EARTH_COLLIDED, EVT_GAME_EARTH_COLLIDED_FUNC},
    {EVT_GAME_MOON_COLLIDED, EVT_GAME_MOON_COLLIDED_FUNC},
    {EVT_GAME_ENEMY_COLLIDED, EVT_GAME_ENEMY_COLLIDED_FUNC},
};
#endif // EVENTDEFINES_H
