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

void get_shape_size(b2ShapeId shapeId, float* width, float* height);

#endif // __GAMEOBJ_H__