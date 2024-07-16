#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "../Physics2D/World2D.h"
#include "../Physics2D/Math/Vector2D.h"
#include "../Physics2D/DataStructs/QuadTree.h"
#include "component.h"

typedef struct entity {
	unsigned int id;
    void* world;
	Components components;
} Entity;

Entity* CreateEntity(World2D* world, Vec2 position, Vec2 rotation, Vec2 scale);
void DestroyEntity(void* api, Entity* entity);
void FreeEntity(void *api, Entity* entity);

void AddAnimatedSpriteComponent(void* pd_ptr, Entity* entity, const char* source, int frame_width, int frame_height, int frame_count, float offset, int16_t z_order);
void AddSpriteComponent(void* pd_ptr, Entity* entity, const char* source, bool flip, float offset, int16_t z_order);
void AddKeyInputComponent(void* pd_ptr, Entity* entity, bool left, bool right, bool up, bool down, bool a, bool b, bool crank);
void AddCircleColliderComponent(void* pd_ptr, struct QuadTree* tree, Entity* entity, Vec2 offset, float radius, int event_id);
void AddHealthComponent(void* pd_ptr, Entity* entity, float max, int event_id);
void AddButtonImageComponent(void* pd_ptr, Entity* entity, BtnStatus status, int event_id, const char* imgdir, float offset, int16_t z_order);
#endif // __ENTITY_H__
