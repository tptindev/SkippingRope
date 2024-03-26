#ifndef __CALLBACK_H__
#define __CALLBACK_H__

#include "box2d.h"

bool PreSolveFcn(b2ShapeId shapeIdA, b2ShapeId shapeIdB, b2Manifold* manifold, void* context);


#endif // __CALLBACK_H__