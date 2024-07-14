#ifndef GAMESCENEEVENTS_H
#define GAMESCENEEVENTS_H

void EVT_GAME_BACK_MENU_FUNC(void* userdata);
void EVT_GAME_EARTH_COLLIDED_FUNC(void* a, void* b);
void EVT_GAME_MOON_COLLIDED_FUNC(void* a, void* b);
void EVT_GAME_ENEMY_COLLIDED_FUNC(void* a, void* b);

#endif // GAMESCENEEVENTS_H
