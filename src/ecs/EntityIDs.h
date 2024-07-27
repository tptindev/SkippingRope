#ifndef ENTITYIDS_H
#define ENTITYIDS_H

typedef enum
{
    ENTITY_EARTH,
    ENTITY_MOON,
    ENTITY_EARTH_BLOOD,
    ENTITY_MOON_BLOOD,
    ENTITY_BTN_START,
    ENTITY_BTN_EXIT,
    ENTITY_BG_MENU,
    ENTITY_BTN_NEW_GAME,
    ENTITY_BTN_QUIT_GAME,
    ENTITY_BG_GAME_OVER,
    ENTITY_SIDE_BAR,
    ENTITY_ARROW_MINI_MAP,
    ENTITY_ENEMY,
    ENTITY_ENEMY_MAX = ENTITY_ENEMY + 128,
} EntityID;
#endif // ENTITYIDS_H
