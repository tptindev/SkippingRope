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
	if (entity->components.transform) free(entity->components.transform);
	if (entity->components.health) free(entity->components.health);
	if (entity->components.body) free(entity->components.body);
	if (entity->components.sprite) free(entity->components.sprite);
	if (entity->components.input) free(entity->components.input);
	if (entity->components.collider) free(entity->components.collider);
	free(entity);
}

void AddCircleColliderComponent(Entity* entity, struct QuadTree* node, Vec2 center, float radius)
{
	if (entity->components.transform != NULL)
	{
		entity->components.collider = (BoxCollider*)malloc(sizeof(BoxCollider));
		if (entity->components.collider != NULL)
		{
			Circle* circle = CreateCircle(center, radius);
			entity->components.collider->shape.type = CIRCLE;
			entity->components.collider->shape.define = circle;

			GetCircleBoundary(&entity->components.collider->shape.box, circle);
			entity->components.collider->shape.box.x += entity->components.transform->x;
			entity->components.collider->shape.box.y += entity->components.transform->y;
			circle->center.x = entity->components.transform->x + entity->components.collider->shape.box.width/2.0f;
			circle->center.y = entity->components.transform->y + entity->components.collider->shape.box.width/2.0f;

			QuadtreeInsert(node, entity, &entity->components.collider->shape.box);
		}
	}
}

void AddTransformComponent(Entity* entity, float x, float y, float angle, float scale)
{
	entity->components.transform = (Transform*)malloc(sizeof(Transform));
	if (entity->components.transform != NULL)
	{
		entity->components.transform->x = x;
		entity->components.transform->y = y;
		entity->components.transform->angle = angle;
		entity->components.transform->scale = scale;
	}
}

void AddHealthComponent(Entity* entity, float max, float current)
{
	entity->components.health = (Health*)malloc(sizeof(Health));
	if (entity->components.health != NULL)
	{
		entity->components.health->max = max;
		entity->components.health->current = current;
	}
}

void AddRegidbodyComponent(Entity* entity, BodyType type, float mass, float gravity_scale)
{
	entity->components.body = (Regidbody*)malloc(sizeof(Regidbody));
	if (entity->components.body != NULL)
	{
		entity->components.body->type = type;
		entity->components.body->mass = mass;
		entity->components.body->gravity_scale = gravity_scale;
	}
}

void AddKeyInputComponent(Entity* entity, bool left, bool right, bool up, bool down, bool a, bool b, bool crank)
{
	entity->components.input = (KeyInput*)malloc(sizeof(KeyInput));
	if (entity->components.input != NULL)
	{
		entity->components.input->left = left;
		entity->components.input->right = right;
		entity->components.input->up = up;
		entity->components.input->down = down;
		entity->components.input->a = a;
		entity->components.input->b = b;
		entity->components.input->crank = crank;
	}
}

void AddSpriteComponent(Entity* entity, const char* source)
{
	entity->components.sprite = (Sprite*)malloc(sizeof(Sprite));
	if (entity->components.sprite != NULL)
	{
		entity->components.sprite->source = source;
	}
}

