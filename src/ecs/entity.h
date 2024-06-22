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
void AddPositionComponent(Entity* entity, float x, float y);
void AddHealthComponent(Entity* entity, float max, float last);
void AddSpriteComponent(Entity* entity, const char* source, float degree);
#endif // __ENTITY_H__
