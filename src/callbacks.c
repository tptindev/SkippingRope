#include "callbacks.h"

bool PreSolveFcn(b2ShapeId shapeIdA, b2ShapeId shapeIdB, b2Manifold* manifold, void* context)
{
    (void)context;
    return false;
}
