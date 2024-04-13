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
} GameObject;

void get_shape_size(b2ShapeId shapeId, float* width, float* height);

#endif // __GAMEOBJ_H__