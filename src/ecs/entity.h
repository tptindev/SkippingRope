#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "../Physics2D/World2D.h"
#include "../Physics2D/Math/Vector2D.h"
#include "../Physics2D/DataStructs/QuadTree.h"
#include "component.h"

typedef struct entity {
	unsigned int id;
	Components components;
} Entity;

Entity* CreateEntity(World2D* world, Vec2 position, Vec2 rotation, Vec2 scale);
void FreeEntity(Entity* entity);

#endif // __ENTITY_H__
