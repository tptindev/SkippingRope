#ifndef __GAMEOBJ_H__
#define __GAMEOBJ_H__

#include "box2d.h"

typedef struct game_obj
{
	b2BodyId id;
	float x;
	float y;
	float half_width;
	float half_height;
	void* sprites;
	unsigned int sprite_size;
} GameObject;

typedef struct level_obj
{
	const char* title;
	void* tilemap;
	float speed;
} LevelObject;

#endif // __GAMEOBJ_H__