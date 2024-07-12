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
void DestroyEntity(void* api, Entity** entity, World2D* world);
void FreeEntity(void *api, Entity** entity);

void AddAnimatedSpriteComponent(void* pd_ptr, Entity* entity, const char* source, int frame_width, int frame_height, int frame_count, float offset, int16_t z_order);
void AddSpriteComponent(void* pd_ptr, Entity* entity, const char* source, bool flip, float offset, int16_t z_order);
void AddKeyInputComponent(void* pd_ptr, Entity* entity, bool left, bool right, bool up, bool down, bool a, bool b, bool crank);
void AddCircleColliderComponent(void* pd_ptr, struct QuadTree* tree, Entity* entity, Vec2 offset, float radius);
void AddHealthComponent(void* pd_ptr, Entity* entity, float max);
void AddButtonComponent(void* pd_ptr, Entity* entity, BtnStatus status, BtnState state, const char* normal, const char* active);
#endif // __ENTITY_H__
