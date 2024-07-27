#ifndef EVENTIDS_H
#define EVENTIDS_H

typedef enum {
    EVT_MENU_MIN = 0,
    EVT_MENU_START,
    EVT_MENU_EXIT,
    EVT_MENU_MAX
} MenuSceneEventIDs;

typedef enum {
    EVT_GAME_MIN = EVT_MENU_MAX,
    EVT_GAME_BACK_MENU,
    EVT_GAME_EARTH_DEAD,
    EVT_GAME_EARTH_HIT,
    EVT_GAME_EARTH_COLLIDED,
    EVT_GAME_MOON_DEAD,
    EVT_GAME_MOON_HIT,
    EVT_GAME_MOON_COLLIDED,
    EVT_GAME_ENEMY_DEAD,
    EVT_GAME_ENEMY_COLLIDED,
    EVT_GAME_MINI_MAP_CHANGED,
    EVT_GAME_NEW_GAME,
    EVT_GAME_QUIT_GAME,
    EVT_GAME_MAX,
} GameSceneEventIDs;

#endif // EVENTIDS_H
