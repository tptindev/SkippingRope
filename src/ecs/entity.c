#include "entity.h"
#include <stdlib.h>

Entity* CreateEntity(World2D* world)
{
	if (world == NULL) return NULL;
	if (world->objId.last > world->objId.max) { return NULL; }
	Entity* entity = (Entity*)malloc(sizeof(Entity));
	if (entity) {
		entity->id = world->objId.last++;
	}
	return entity;
}

void FreeEntity(Entity* entity)
{
	if (entity->component.position) free(entity->component.position);
	if (entity->component.health) free(entity->component.health);
	free(entity);
}

void AddPositionComponent(Entity* entity, float x, float y)
{
	entity->component.position = (Position*)malloc(sizeof(Position));
	if (entity->component.position != NULL)
	{
		entity->component.position->x = x;
		entity->component.position->y = y;
	}
}

void AddHealthComponent(Entity* entity, float max, float current)
{
	entity->component.health = (Health*)malloc(sizeof(Health));
	if (entity->component.health != NULL)
	{
		entity->component.health->max = max;
		entity->component.health->current = current;
	}
}

void AddSpriteComponent(Entity* entity, const char* source, float degree)
{
	entity->component.sprite = (Sprite*)malloc(sizeof(Sprite));
	if (entity->component.sprite != NULL)
	{
		entity->component.sprite->source = source;
		entity->component.sprite->degree = degree;
	}
}

