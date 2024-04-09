#ifndef __GAMEOBJ_H__
#define __GAMEOBJ_H__

#include "box2d.h"

b2BodyId create_floor_obj(b2WorldId world);
b2BodyId create_square_box_obj(b2WorldId world);
void get_shape_size(b2ShapeId shapeId, int* width, int* height);

#endif // __GAMEOBJ_H__