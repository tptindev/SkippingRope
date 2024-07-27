#ifndef __GAME_H__
#define __GAME_H__

void game_initialize(void* pd_ptr);
void game_update(float deltatime);
void game_draw(void);
void game_destroy(void);

#endif // __GAME_H__
