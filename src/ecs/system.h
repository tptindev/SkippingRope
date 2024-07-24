#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "../Physics2D/DataStructs/QuadTree.h"
#include "entity.h"

void UpdateScale(Entity* entity, float scale);
void UpdateRotation(Entity* entity);
void UpdateMovement(Entity* entity, float dt);
void UpdatePosition(Entity* entity, Vec2 buffer, float dt);
void UpdateCollider(Entity* entity, struct QuadTree* tree);
void UpdateCollisionDetection(Entity* entity, struct QuadTree* tree);
void UpdateSprite(Entity* entity, unsigned int tick);
void UpdateAnimateSprite(Entity* entity, unsigned int tick);
void UpdateButtonImage(void* pd_ptr, Entity* entity, void* userdata);
void UpdateHealth(void* pd_ptr, void* scene_ptr, Entity* entity);
void UpdateBinding(void* scene_ptr, Entity* entity);
void UpdateRenderer(void* pd_ptr, Entity* entity);
void UpdateSpawn(void* pd_ptr, void* scene_ptr);
#endif // __SYSTEM_H__
