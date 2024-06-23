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
	if (entity->component.transform) free(entity->component.transform);
	if (entity->component.health) free(entity->component.health);
	if (entity->component.body) free(entity->component.body);
	if (entity->component.sprite) free(entity->component.sprite);
	if (entity->component.input) free(entity->component.input);
	free(entity);
}

void AddTransformComponent(Entity* entity, float x, float y, float angle, float scale)
{
	entity->component.transform = (Transform*)malloc(sizeof(Transform));
	if (entity->component.transform != NULL)
	{
		entity->component.transform->x = x;
		entity->component.transform->y = y;
		entity->component.transform->angle = angle;
		entity->component.transform->scale = scale;
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

void AddRegidbodyComponent(Entity* entity, BodyType type, float mass, float gravity_scale)
{
	entity->component.body = (Regidbody*)malloc(sizeof(Regidbody));
	if (entity->component.body != NULL)
	{
		entity->component.body->type = type;
		entity->component.body->mass = mass;
		entity->component.body->gravity_scale = gravity_scale;
	}
}

void AddKeyInputComponent(Entity* entity, bool left, bool right, bool up, bool down, bool a, bool b, bool crank)
{
	entity->component.input = (KeyInput*)malloc(sizeof(KeyInput));
	if (entity->component.input != NULL)
	{
		entity->component.input->left = left;
		entity->component.input->right = right;
		entity->component.input->up = up;
		entity->component.input->down = down;
		entity->component.input->a = a;
		entity->component.input->b = b;
		entity->component.input->crank = crank;
	}
}

void AddSpriteComponent(Entity* entity, const char* source)
{
	entity->component.sprite = (Sprite*)malloc(sizeof(Sprite));
	if (entity->component.sprite != NULL)
	{
		entity->component.sprite->source = source;
	}
}

