#ifndef __GAME_H__
#define __GAME_H__

#include "pd_api.h"

void game_initialize(void* userdata);
void game_update(float deltatime);
void game_draw();
void game_destroy();

#endif // __GAME_H__