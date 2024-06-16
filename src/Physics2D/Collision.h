#ifndef COLLISION_H
#define COLLISION_H

#include <stddef.h>
#include "Body2D.h"

bool IsCollisionCircle(Body2D *a, Body2D *b, void (*callback)(Body2D *, Body2D *));
#endif // COLLISION_H
