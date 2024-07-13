#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "../Physics2D/DataStructs/QuadTree.h"
#include "../Physics2D/World2D.h"
#include "entity.h"

void UpdateScale(Entity* entity, float scale);
void UpdateRotation(Entity* entity);
void UpdateMovement(Entity* entity, float dt);
void UpdatePosition(Entity* entity, Vec2 buffer, float dt);
void UpdateCollider(Entity* entity, struct QuadTree* tree);
void UpdateCollision(Entity* entity, struct QuadTree* tree, void (*callback)(Entity* a, Entity* b));
void UpdateSprite(Entity* entity, unsigned int tick);
void UpdateAnimateSprite(Entity* entity, unsigned int tick);
void UpdateButtonImage(Entity* entity, void* userdata);
void UpdateHealth(void* pd_ptr, World2D* world, Entity** entity, void (*callback)(void* api, World2D* world, Entity** entity, void* health));
void UpdateRenderer(void* pd_ptr, Entity* entity);

#endif // __SYSTEM_H__
