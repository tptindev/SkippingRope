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

void AddAnimatedSpriteComponent(void* userdata, Entity* entity, const char* source, int frame_width, int frame_height, int frame_count, int16_t z_order);
void AddSpriteComponent(void* userdata, Entity* entity, const char* source, bool flip, int16_t z_order);
void AddKeyInputComponent(void* userdata, Entity* entity, bool left, bool right, bool up, bool down, bool a, bool b, bool crank);
void AddCircleColliderComponent(void* userdata, struct QuadTree* tree, Entity* entity, Vec2 offset, float radius);
void AddHealthComponent(void* userdata, Entity* entity, float max);
#endif // __ENTITY_H__
