#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "../Physics2D/DataStructs/QuadTree.h"
#include "component.h"
#include "entity.h"

void UpdateScale(Entity* entity, float scale);
void UpdateRotation(Entity* entity, float angle);
void UpdateMovement(Entity* entity, Vec2 acceleration, float dt);
void UpdatePosition(Entity* entity, Vec2 to, float dt);
void UpdateCollider(void* userdata, Entity* entity, QuadTree* tree);
void UpdateCollision(void* userdata, Collider* collider, QuadTree* origin);
void UpdateInput(void* userdata, Entity* entity);
#endif // __SYSTEM_H__
