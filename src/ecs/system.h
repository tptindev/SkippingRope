#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "../Physics2D/DataStructs/QuadTree.h"
#include "../Physics2D/World2D.h"
#include "component.h"
#include "entity.h"

void UpdateScale(Entity* entity, float scale);
void UpdateRotation(Entity* entity, float angle);
void UpdateMovement(Entity* entity, float dt);
void UpdatePosition(Entity* entity, Vec2 buffer, float dt);
void UpdateCollider(Entity* entity, struct QuadTree* tree);
void UpdateCollision(Entity* entity, struct QuadTree* tree, void (*callback)(Entity* a, Entity* b));
void UpdateSprite(Entity* entity, unsigned int tick);
void UpdateAnimateSprite(Entity* entity, unsigned int tick);
void UpdateHealth(void* userdata, World2D* world, Entity** entity, void (*callback)(void* api, World2D* world, Entity** entity, void* health));
void UpdateRenderer(void* userdata, Entity* entity);
void UpdateInput(void* userdata, Entity* entity);

#endif // __SYSTEM_H__
