#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "../Physics2D/World2D.h"
#include "component.h"

typedef struct entity {
	unsigned int id;
	Components component;
} Entity;

Entity* CreateEntity(World2D* world);
void FreeEntity(Entity* entity);
void AddTransformComponent(Entity* entity, float x, float y, float angle, float scale);
void AddHealthComponent(Entity* entity, float max, float last);
void AddRegidbodyComponent(Entity* entity, BodyType type, float mass, float gravity_scale);
void AddKeyInputComponent(Entity* entity, bool left, bool right, bool up, bool down, bool a, bool b, bool crank);
void AddSpriteComponent(Entity* entity, const char* source);
#endif // __ENTITY_H__
