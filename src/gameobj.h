#ifndef __GAMEOBJ_H__
#define __GAMEOBJ_H__

#include "box2d.h"

b2BodyId create_floor_obj(b2WorldId world);
b2BodyId create_square_box_obj(b2WorldId world);
b2BodyId create_column_obj(b2WorldId world, float x, float y, float haft_width, float haft_height);
b2BodyId* create_rope_obj(b2WorldId world);
#endif // __GAMEOBJ_H__