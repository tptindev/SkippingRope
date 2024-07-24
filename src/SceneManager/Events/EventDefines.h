#ifndef EVENTDEFINES_H
#define EVENTDEFINES_H

#include <stdlib.h>
#include "Event.h"
#include "EventIDs.h"
#include "MenuSceneEvents.h"
#include "GameSceneEvents.h"

Event MenuSceneEvents[2 ] = {
    {EVT_MENU_START, EVT_MENU_START_FUNC, NULL},
    {EVT_MENU_EXIT,  EVT_MENU_EXIT_FUNC, NULL},
};

// Trans, Collision, Dead, Trigger
Event GameSceneEvents[9] = {
    {EVT_GAME_BACK_MENU, EVT_GAME_BACK_MENU_FUNC, NULL, NULL, NULL},
    {EVT_GAME_EARTH_DEAD, EVT_GAME_EARTH_DEAD_FUNC, NULL, NULL, NULL},
    {EVT_GAME_MOON_DEAD, NULL, NULL, NULL, NULL},
    {EVT_GAME_ENEMY_DEAD, NULL, NULL, EVT_GAME_ENEMY_DEAD_FUNC, NULL},
    {EVT_GAME_EARTH_COLLIDED, NULL, EVT_GAME_EARTH_COLLIDED_FUNC, NULL, NULL},
    {EVT_GAME_MOON_COLLIDED, NULL, EVT_GAME_MOON_COLLIDED_FUNC, NULL, NULL},
    {EVT_GAME_ENEMY_COLLIDED, NULL, EVT_GAME_ENEMY_COLLIDED_FUNC, NULL, NULL},
    {EVT_GAME_EARTH_HIT, NULL, NULL, NULL, EVT_GAME_EARTH_HIT_FUNC},
    {EVT_GAME_MOON_HIT, NULL, NULL, NULL, EVT_GAME_MOON_HIT_FUNC},
};
#endif // EVENTDEFINES_H
