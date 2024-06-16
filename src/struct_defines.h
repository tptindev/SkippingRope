#ifndef __GAMEOBJ_H__
#define __GAMEOBJ_H__

#include "Physics2D/Body2D.h"

typedef struct game_obj
{
	Body2D* body;
	void* sprites;
	unsigned int sprite_size;
	bool live;
	bool collided;
} GameObject;

typedef struct level_obj
{
	const char* title;
	void* tilemap;
	float speed;
	int interval; // second
} LevelObject;

#endif // __GAMEOBJ_H__